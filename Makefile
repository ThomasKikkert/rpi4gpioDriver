obj-m += rpi4gpioDriver.o
rpi4gpioDriver-objs += ./chardev.o
KVERSION = $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

install:
	insmod rpi4gpioDriver.ko

uninstall:
	rmmod  rpi4gpioDriver