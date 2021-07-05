#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define DIR "/sys/class/gpio/export"



int main(){
	int fd, len;
	char buf[BUFSIZ];
	int gpio_pin = 18;

	fd = open("/sys/class/gpio/export",O_WRONLY);
	if(fd == -1){
		printf("error: not open export\n");
		return 1;
	}
	sprintf(buf,"%d", gpio_pin);  
	if(write(fd, buf, sizeof(buf)) == -1){
		printf("error: fail to write gpio18");
		return 1;
	}
	close(fd);


	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio_pin);
	fd = open(buf, O_WRONLY);
	if(fd  == -1){
		printf("error: not open direction\n");
		return 1;
	}
	if(write(fd, "out", sizeof(4))== -1){
		printf("error: not open out\n");
		return 1;
	}
	close(fd);
	

	snprintf(buf, 200, "/sys/class/gpio/gpio%d/value", gpio_pin);
	fd = open(buf, O_WRONLY);
	if(fd == -1){
		printf("error: not open 1");
		return 1;
	}
	while(1){
		if(write(fd, "1", sizeof(2)) == -1){
			printf("error: not write high");
			return 1;
		}
		sleep(1);
		if(write(fd, "0", sizeof(2)) == -1){
			printf("error: not write low");
			return 1;
		}
		sleep(1);
	}
	close(fd);


	snprintf(buf, sizeof(buf), "/sys/class/gpio/unexport");
	fd = open(buf, O_WRONLY);
	if(fd == -1){
		printf("error : not open unexport");
		return 1;
	}
	snprintf(buf, sizeof(buf), "%d", gpio_pin);
	if(write(fd, buf, sizeof(buf))==-1){
		printf("error : not write gpio_pin");
		return 1;
	}
	close(fd);

	return 0;
}

