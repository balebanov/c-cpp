#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "inc/E-1608.c"
#include "inc/ethernet.c"

#include <fcntl.h>
#include <math.h>
#include <ctype.h>

int main(int argc, char**argv)
{
  DeviceInfo_E1608 device_info;
  uint16_t value;
  uint8_t channel = 0;
  uint8_t range = 0;

  device_info.device.connectCode = 0x0;   // default connect code
  device_info.device.frameID = 0;         // zero out the frameID
  device_info.queue[0] = 0;               // set count in gain queue to zero

  if (argc == 2) {
    printf("E-1608 IP address = %s\n", argv[1]);
    if (inet_aton(argv[1], &device_info.device.Address.sin_addr) == 0) {
      printf("Improper destination address.\n");
      return -1;
    }
  } else if (discoverDevice(&device_info.device, E1608_PID) <= 0) {
    printf("No device found.\n");
    return -1;
  }

  if ((device_info.device.sock = openDevice(inet_addr(inet_ntoa(device_info.device.Address.sin_addr)),
					    device_info.device.connectCode)) < 0) {
    printf("Error opening socket\n");
    return -1;
  }

  buildGainTableAIn_E1608(&device_info);
  buildGainTableAOut_E1608(&device_info);

  while(1) {
    printf("\nE-1608 Testing\n");
    printf("----------------\n");
     
	while(1) {
	  AIn_E1608(&device_info, channel, range, &value);
	  double Volts = volts_E1608(value, range);
	  //printf("Range %d  Channel %d   Sample[%d] = %#x Volts = %lf\n", range, channel,  i, value, volts_E1608(value, range));
	  printf("Напряжение - %lf\n", Volts);
	  char charVolts[sizeof(Volts)];
	  memcpy(&charVolts, &Volts, sizeof(Volts));
	  //sending = sendall(new_sock, charVolts, sizeof(charVolts), 0);
	  sleep(1);	  
	}
	
  }
  return 0;
}

