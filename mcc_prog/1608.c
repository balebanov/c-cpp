/*
	Программа для работы с устройством MCC E-1608. Запись данных с аналоговых входов в файл
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "inc/E-1608.h"
#include "inc/E-1608.c"
#include "inc/ethernet.h"
#include "inc/ethernet.c"

#include <iostream>
#include <fstream>
#include <string>

#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

const std::string currentTime(){
	char fmt[64], buf[64];
	struct timeval tv;
	struct tm *tm;
	
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S.%%06u", tm);
	snprintf(buf, sizeof(buf), fmt, tv.tv_usec);
	return buf;
}

int main(int argc, char **argv){
    DeviceInfo_E1608 device_info;
    double frequency;
    int i, j, k;
    uint16_t dataIn[512];
    uint32_t count;
    int corrected_data;

    uint8_t options;
    uint8_t channel;
    uint8_t range;
    uint8_t nchan;
    std::ofstream myFile;

	std::cout << currentTime() << std::endl;
	
	myFile.open("files/1_new_values.csv");
	myFile << "Test\n";
	
	if (discoverDevice(&device_info.device, E1608_PID) <= 0) {
        printf("Устройство не найдено\n");
        return -1;
    }

    device_info.device.connectCode = 0x0;   // default connect code
    device_info.device.frameID = 0;         // zero out the frameID
    device_info.queue[0] = 0;               // set count in gain queue to zero
    device_info.timeout = 1000;             // set default timeout to 1000 ms.

    if ((device_info.device.sock = openDevice(inet_addr(inet_ntoa(device_info.device.Address.sin_addr)),
					    device_info.device.connectCode)) < 0) {
        printf("Ошибка сокета\n");
        return -1;
    }

    buildGainTableAIn_E1608(&device_info);
	//вывод калибровочной информации	
/*    for (i = 0; i < NGAINS; i++) {
        printf("Calibration Table (Differential): Range = %d Slope = %f  Intercept = %f\n",
	   i, device_info.table_AInDF[i].slope, device_info.table_AInDF[i].intercept);
    }
    for (i = 0; i < NGAINS; i++) {
        printf("Calibration Table (Single Ended): Range = %d Slope = %f  Intercept = %f\n",
	   i, device_info.table_AInSE[i].slope, device_info.table_AInSE[i].intercept);
    }
*/

    AInScanStop_E1608(&device_info, 0);  //Stop the scan if running.
    printf("Введите частоту дискретизации (250 kHz max) [Гц]: ");
    scanf("%lf", &frequency);
    printf("Число выборок < 512 : ");
    scanf("%d", &count);
    if(count >= 512){
		printf("less 512\n");
		return -1;
	}
    printf("Число каналов для сканирования [1-8]: ");
    scanf("%hhd", &nchan);
    if (nchan < 1 || nchan > 8) {
        printf("Число каналов должно быть от 1 до 8\n");
        return -1;
    }
	//таблица усиления
    device_info.queue[0] = nchan;
	for (i = 0; i < nchan; i++) {
		printf("Введите %d канал в очереди усиления [0-11]: ", i+1);
		scanf("%hhd", &device_info.queue[2*i+1]);
		printf("Диапазон [0-3]: \n");
		printf("  0 - +/-10В\n");
		printf("  1 - +/- 5В\n");
		printf("  2 - +/- 2В\n");
		printf("  3 - +/- 1В\n");
		scanf("%hhd", &device_info.queue[2*i+2]);
	}
    AInQueueW_E1608(&device_info);
    AInQueueR_E1608(&device_info);
    for (i = 0; i < device_info.queue[0]; i++) {
        printf("%d.  channel = %hhd   range = %hhd\n", i+1, device_info.queue[2*i+1], device_info.queue[2*i+2]);
    }

    options = 0x0;
    AInScanStart_E1608(&device_info, count, frequency, options);
    AInScanRead_E1608(&device_info, count, nchan, dataIn);
    close(device_info.device.scan_sock);

    std::string arr_data[nchan+1][count];

    for (i = 0; i < count; i++) {    // выборка
        for (j = 0; j < nchan; j++) {   // канал
            k = i*nchan + j;  //номер выборки
            channel = device_info.queue[2*j+1];  
            range = device_info.queue[2*j+2];  
            if (channel < DF) {  
                corrected_data = rint(dataIn[k]*device_info.table_AInSE[range].slope + device_info.table_AInSE[range].intercept);
            } else {  
                corrected_data = rint(dataIn[k]*device_info.table_AInDF[range].slope + device_info.table_AInDF[range].intercept);
            }
            if (corrected_data > 65536) {
                corrected_data = 65535;
            } else if (corrected_data < 0) {
                corrected_data = 0;
            }
            printf("Диапазон %d Канал %d  Выборка %d Напряжение = %lf\n", range, channel, k+1, volts_E1608(corrected_data, range));
            
			std::string str_volts = std::to_string(volts_E1608(corrected_data, range));
			arr_data[0][i] = currentTime();
			arr_data[j+1][i] = str_volts;						
        }
        printf("\n");
    }
    
    myFile << "\n\n";    
    myFile << "Time;";
    
    for(i = 0; i < nchan; i++){
		myFile << "Channel " << i+1 << ";";
	}
	myFile << "\n";
    
    for(i = 0; i < count; i++){
		myFile << arr_data[0][i] << ";";
		for(j = 0; j < nchan; j++){
			myFile << arr_data[j+1][i] << ";";
		}
			myFile << "\n";			
	}
	
	myFile.close();
	return 0;
}
