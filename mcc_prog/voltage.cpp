#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "/home/pi/prog/MCC/inc/E-1608.c"
#include "/home/pi/prog/MCC/inc/ethernet.c"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
using namespace std;

DeviceInfo_E1608 device_info;
uint16_t value;
const uint8_t channelZero = 0;
const uint8_t channelOne = 1;
const uint8_t channelTwo = 2;
const uint8_t channelThree = 3;
const uint8_t range = 0;

void startProc(){
  device_info.device.connectCode = 0x0;   // default connect code
  device_info.device.frameID = 0;         // zero out the frameID
  device_info.queue[0] = 0;               // set count in gain queue to zero
  
  if (discoverDevice(&device_info.device, E1608_PID) <= 0) {
    cerr << "No device found" << endl;
    exit(-1);
  }

  if ((device_info.device.sock = openDevice(inet_addr(inet_ntoa(device_info.device.Address.sin_addr)), device_info.device.connectCode)) < 0) {
    cerr << "Ошибка сокета Е-1608" << endl;
    exit(-1);
  }

  buildGainTableAIn_E1608(&device_info);
  buildGainTableAOut_E1608(&device_info);
}

double getVoltageChannelZero(){
	AIn_E1608(&device_info, channelZero, range, &value);
	double Volts = volts_E1608(value, range);
	return Volts;
	sleep(1);	
}

double getVoltageChannelOne(){
	AIn_E1608(&device_info, channelOne, range, &value);
	double Volts = volts_E1608(value, range);
	return Volts;
	sleep(1);	
}

double getVoltageChannelTwo(){
	AIn_E1608(&device_info, channelTwo, range, &value);
	double Volts = volts_E1608(value, range);
	return Volts;
	sleep(1);
}

double getVoltageChannelThree(){
	AIn_E1608(&device_info, channelThree, range, &value);
	double Volts = volts_E1608(value, range);
	return Volts;
	sleep(1);	
}

int main(){
	startProc();
	
	int fd;
	FILE *stream;
	
	fd = open("voltage.txt", O_RDWR);
	if(fd == 1){
		cerr << "Ошибка открытия файла" << endl;
		return -1;
	} else{
		cout << "Файл открыт!" << endl;
	}
	
	stream = fdopen(fd, "w");
	if(!stream) {
		cerr << "Ошибка fdopen()" << endl;
		return -1;
	}
	
	if(fputs("Hello", stream) == EOF){
		cerr << "Test error" << endl;
		return -1;
	}
	
	while(stream != NULL) {
		double voltage_zero = getVoltageChannelZero();
		char buff_zero[10];
		sprintf(buff_zero, "%f;", voltage_zero);
		
		double voltage_one = getVoltageChannelOne();
		char buff_one[10];
		sprintf(buff_one, "%f;", voltage_one);
		
		double voltage_two = getVoltageChannelTwo();
		char buff_two[10];
		sprintf(buff_two, "%f;", voltage_two);
		
		double voltage_three = getVoltageChannelThree();
		char buff_three[10];
		sprintf(buff_three, "%f;", voltage_three);
		
		//запись данных в файл
		ssize_t wrt_pos0, wrt_pos1, wrt_pos2, wrt_pos3;
		wrt_pos0 = pwrite(fd, buff_zero, sizeof(buff_zero), 0);
		if(wrt_pos0 == (int )-1){
			cerr << "Ошибка pwrite0" << endl;
			break;
		}
		wrt_pos1 = pwrite(fd, buff_one, sizeof(buff_one), (off_t )strlen(buff_zero));
		if(wrt_pos1 == (int )-1){
			cerr << "Ошибка pwrite1" << endl;
			break;
		}
		wrt_pos2 = pwrite(fd, buff_two, sizeof(buff_two), (off_t )strlen(buff_zero) + (off_t )strlen(buff_one));
		if(wrt_pos2 == (int )-1){
			cerr << "Ошибка pwrite2" << endl;
			break;
		}
		wrt_pos3 = pwrite(fd, buff_three, sizeof(buff_three), (off_t )strlen(buff_zero) + (off_t )strlen(buff_one) + (off_t )strlen(buff_two));
		if(wrt_pos3 == (int )-1){
			cerr << "Ошибка pwrite3" << endl;
			break;
		}	
		sleep(1);
		
		//обновление данных, установка курсора
		off_t cursor;
		size_t sumLen = strlen(buff_zero) + strlen(buff_one) + strlen(buff_two) + strlen(buff_three);
		cursor = lseek(fd, 0, SEEK_CUR);
		if(cursor == (off_t )-1){
			cerr << "Ошибка lseek()" << endl;
			break;
		}
		
		//урезать данные
		int ftr;
		ftr = ftruncate(fd, (off_t )sumLen);
		if(ftr == -1){
			cerr << "Ошибка ftruncate" << endl;
			break;
		}
	}
	//закрыть файл
	if(fclose(stream) == EOF) {
		cerr << "Ошибка fclose()" << endl;
		return 1;
	}
			
	return 0;
}
