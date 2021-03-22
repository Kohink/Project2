obj-y :=test_call.o
obj-m :=syscallModule.o

PWD := $(shell pwd)
KDIR := /lib/modules/$(shell uname-r)/build

default:
	$(MAKE) -C $(KDIR) SUBDIRS= $(PWD) modules

clean:
	rm –f *.o *.ko *.mod.* Module.* modules.*