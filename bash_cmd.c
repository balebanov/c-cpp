#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	char *cmd = "lsusb";
	char buf[BUFSIZ];
	FILE *ptr;
	
	cmd = argv[1];
	
	if((ptr = popen(cmd, "r")) != NULL){
		while(fgets(buf, BUFSIZ, ptr) != NULL){
			(void)printf("%s", buf);
		}
		(void)pclose(ptr);
	}
	return 0;
}
