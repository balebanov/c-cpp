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

#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

int main(int argc, char **argv){
    struct tm *date;
    struct timeval tval;
    time_t t;
    //struct in_addr network[3];
    DeviceInfo_E1608 device_info;
    double frequency;
    //double volts;
    int i, j, k;
    //uint16_t status;
    //uint16_t value;
    uint16_t dataIn[512];
    //uint32_t counter;
    uint32_t count;
    //int flag;
    //int ch;
    int corrected_data;

    uint8_t options;
    //uint8_t input;
    uint8_t channel;
    uint8_t range;
    uint8_t nchan;
    //int temp;

/*	FILE *ptrFile = fopen("files/_values.ods", "w");
	if(ptrFile == NULL){
		printf("Error opening file!");
		return -1;
	}
*/	
	
	std::ofstream myFile;
	myFile.open("files/1_new_values.csv");
	myFile << "Test\n";
	//myFile.close();
	

    if (argc == 2) {
        printf("E-1608 IP адрес = %s\n", argv[1]);
        device_info.device.Address.sin_family = AF_INET;
        device_info.device.Address.sin_port = htons(COMMAND_PORT);
        device_info.device.Address.sin_addr.s_addr = INADDR_ANY;
        if (inet_aton(argv[1], &device_info.device.Address.sin_addr) == 0) {
            printf("Неверный адрес\n");
            return -1;
        }
    } else if (discoverDevice(&device_info.device, E1608_PID) <= 0) {
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
/*    for (i = 0; i < NGAINS; i++) {
        printf("Calibration Table (Differential): Range = %d Slope = %f  Intercept = %f\n",
	   i, device_info.table_AInDF[i].slope, device_info.table_AInDF[i].intercept);
    }
    for (i = 0; i < NGAINS; i++) {
        printf("Calibration Table (Single Ended): Range = %d Slope = %f  Intercept = %f\n",
	   i, device_info.table_AInSE[i].slope, device_info.table_AInSE[i].intercept);
    }
*/
    buildGainTableAOut_E1608(&device_info);
/*    for (i = 0; i < NCHAN_AOUT; i++) {
        printf("Calibration Table Analog Output: Channel = %d Slope = %f  Intercept = %f\n",
	   i, device_info.table_AOut[i].slope, device_info.table_AOut[i].intercept);
    }
*/
    //printf("Testing E-1608 Analog Input Scan.\n");
    AInScanStop_E1608(&device_info, 0);  //Stop the scan if running.
    printf("Введите частоту дискретизации [Гц]: ");
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
		// set up the gain queue
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

    double array[nchan][count];

    for (i = 0; i < count; i++) {    // scan count
        for (j = 0; j < nchan; j++) {   // channel count
            k = i*nchan + j;  // sample number
            channel = device_info.queue[2*j+1];  // channel
            range = device_info.queue[2*j+2];    // range value
            if (channel < DF) {  // single ended
                corrected_data = rint(dataIn[k]*device_info.table_AInSE[range].slope + device_info.table_AInSE[range].intercept);
            } else {  // differential
                corrected_data = rint(dataIn[k]*device_info.table_AInDF[range].slope + device_info.table_AInDF[range].intercept);
            }
            if (corrected_data > 65536) {
                corrected_data = 65535;  // max value
            } else if (corrected_data < 0) {
                corrected_data = 0;
            }
            //printf("Range %d Channel %d  Sample[%d] = %#x Volts = %lf\n", range, channel,
                //k, corrected_data, volts_E1608(corrected_data, range));
/*            if(writeData("values.txt", volts_E1608(corrected_data, range)) == false){
                printf("Ошибка writeData\n");
                return -1;
            }
*/
            printf("Диапазон %d Канал %d  Выборка %d Напряжение = %lf\n", range, channel, k+1, volts_E1608(corrected_data, range));

			array[j][i] = volts_E1608(corrected_data, range);
        }
        printf("\n");
    }
    
/*    t = time(NULL);
    date = localtime(&t);
    printf("Time: %s\n", asctime(date));
*/    

    
    myFile << "\n\n";
    //myFile << "Time: " << date->tm_hour << ":" << date->tm_min << ":" << date->tm_sec << "." << tval.tv_usec << ";\n\n";
    
    myFile << "Time;";
    
    for(i = 0; i < nchan; i++){
		myFile << "Channel " << i+1 << ";";
	}
	myFile << "\n";
    
    for(i = 0; i < count; i++){
		gettimeofday(&tval, 0);
		date = gmtime((time_t*)&tval.tv_sec);
		myFile << date->tm_hour << ":" << date->tm_min << ":" << date->tm_sec << "." << tval.tv_usec << ";";
		for(j = 0; j < nchan; j++){
/*			char buf[10];
			sprintf(buf, "%f;", array[j][i]);
			fputs(buf, ptrFile);
			fseek(ptrFile, 0, SEEK_END);
*/	

			myFile << array[j][i] << ";";
		}
			myFile << "\n";
			
	}
}
