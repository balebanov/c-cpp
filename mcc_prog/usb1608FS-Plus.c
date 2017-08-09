#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include "pmd.h"
#include "usb-1608FS-Plus.h"

#include <iostream>
#include <fstream>

#define MAX_COUNT     (0xffff)
#define FALSE 0
#define TRUE 1

const std::string currentTime(){ //get current time
	char fmt[64], buf[64];
	struct timeval tv;
	struct tm *tm;
	
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S.%%06u", tm);
	snprintf(buf, sizeof(buf), fmt, tv.tv_usec);
	return buf;
}

int main (int argc, char **argv)
{
  libusb_device_handle *udev = NULL;
  struct tm calDate;

  float table_AIN[NGAINS_USB1608FS_PLUS][NCHAN_USB1608FS_PLUS][2];

  int ch;
  int i, j, k, m;
  uint8_t input;
  int temp;
  uint8_t options;
  char serial[9];
  uint8_t channel, channels;
  uint8_t range;
  uint8_t ranges[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint16_t value;
  uint32_t count;
  double frequency;
  int ret;
  uint16_t sdataIn[8*512]; // holds 16 bit unsigned analog input data
  uint16_t dataC[256][8];  // corrected data
  uint16_t data;
  int nchan, repeats, scan;
  int flag;
  
  std::ofstream file;
  file.open("files/usb_voltage.csv");
  file << "Test\n\n";

  ret = libusb_init(NULL);
  if (ret < 0) {
    perror("usb_device_find_USB_MCC: Failed to initialize libusb");
    exit(1);
  }

  if ((udev = usb_device_find_USB_MCC(USB1608FS_PLUS_PID, NULL))) {
  	printf("Success, found a USB 1608FS-Plus!\n");
  } else {
  	printf("Failure, did not find a USB 1608FS-Plus!\n");
  	return 0;
  }

  // some initialization
  //print out the wMaxPacketSize.  Should be 64.
 printf("wMaxPacketSize = %d\n", usb_get_max_packet_size(udev,0));

 usbBuildGainTable_USB1608FS_Plus(udev, table_AIN);
/*  for (i = 0; i < NGAINS_USB1608FS_PLUS; i++ ) {
    for (j = 0; j < NCHAN_USB1608FS_PLUS; j++) {
      printf("Calibration Table: Range = %d Channel = %d Slope = %f   Offset = %f\n", 
	     i, j, table_AIN[i][j][0], table_AIN[i][j][1]);
    }
  }
*/
 	printf("Testing USB-1608FS_Plus Mult-Channel Analog Input Scan.\n");
	usbAInScanStop_USB1608FS_Plus(udev);
	printf("enter number of channels (1-8) :");
	scanf("%d", &nchan);
	printf("Enter number of scans (less than 512): ");
    scanf("%d", &count);
    printf("Enter sampling frequency (100 KHz max): ");
    scanf("%lf", &frequency);
    // Build bitmap for the first nchan in channels.
    channels = 0;
    file << "Date;";
    for (i = 0; i < nchan; i++) {
		channels |= (1 << i);
		file << "Channel" << i << ";";
	}
	file << "\n";
        printf ("channels: %02X   count:%d\n", channels, count);
        // Always use BP_10V to make it easy (BP_10V is 0...)
        range = 0;
        memset(ranges, range, sizeof(ranges));
        usbAInScanConfig_USB1608FS_Plus(udev, ranges);
        // Run a loop for the specified number of repeats and
        // show the results...
		std::string arr_data[nchan+1][count];
			printf("\n\n---------------------------------------");
			usbAInScanStart_USB1608FS_Plus(udev, count, frequency, channels, 0);
			ret = usbAInScanRead_USB1608FS_Plus(udev, count, nchan, sdataIn, 0);
			printf("Number samples read = %d\n", ret/2);
			if (ret != count * nchan * 2) {
				printf("***ERROR***  ret = %d   count = %d  nchan = %d\n", ret, count, nchan);
			} /* if (ret != count * nchan * 2) */
			for (i = 0; i < count; i++) {
				printf("%6d", i);
				for (j = 0; j < nchan; j++)	{
					k = i*nchan + j;
					data = rint(sdataIn[k]*table_AIN[range][j][0] + table_AIN[range][j][1]);
					
					std::string str_volts = std::to_string(volts_USB1608FS_Plus(data, range));
					printf(", %8.6f", volts_USB1608FS_Plus(data, range));
					arr_data[0][i] = currentTime();
					arr_data[j+1][i] = str_volts;					
					
				} /* for (j - 0; j < 8, j++) */
			printf("\n");
			} /* for (i = 0; i < count; i++) */
	printf("\n\n---------------------------------------\n");

    for(i = 0; i < count; i++){
		file << arr_data[0][i] << ";";
		for(j = 0; j < nchan; j++){
			file << arr_data[j+1][i] << ";";
		}
			file << "\n";			
	}

	return 0;
}
