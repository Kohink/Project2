obj-m += my_timer.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=/usr/src/my_timer modules
	#sudo insmod my_timer.ko
	#cat /proc/timer
	#sleep 1
	#cat /proc/timer
	#sleep 3
	#cat /proc/timer
	#sleep 5
	#cat /proc/timer
	#sudo rmmod my_timer
clean:
	make -C /lib/modules/$(shell uname -r)/build M=/usr/src/my_timer clean