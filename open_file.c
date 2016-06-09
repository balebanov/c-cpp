#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int fd;
	fd = creat("test.txt", 0644);
	if(fd == -1){
		perror("Ошибка создания файла!");
		exit(-1);
	}

	const char *buf = "Тестовый код для файла";
	ssize_t wd;
	wd = write(fd, buf, strlen(buf));
	if(wd == -1){
		perror("Ошибка записи");
		exit(-1);
	}

	return 0;
}