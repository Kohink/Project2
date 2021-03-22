#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <elevator.c>
#define START_ELEVATOR_ 335
#define ISSUE_REQUEST_ 336 
#define STOP_ELEVATOR_ 337

int start_call(int ele1) {
	return syscall(START_ELEVATOR_, ele1);
}

int issue_call(int req) {
	return syscall(ISSUE_REQUEST_, req);
}

int stop_call(int ele2) {
	return syscall(STOP_ELEVATOR_, ele2);
}

int main(int argc, char **argv) 
{
	if (argc != 2) {
		printf("wrong number of args\n");
		return -1;
	}
	
	int test = atoi(argv[1]);
	long ret = start_call(test);

	printf("sending this: %d\n", test);

	if (ret < 0)
		perror("system call error");
	else
		printf("Function successful. passed in: %d, returned %ld\n", test, ret);
	
	printf("Returned value: %ld\n", ret);	

	return 0;
}

