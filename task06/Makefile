KDIR := /lib/modules/$(shell uname -r)/build
FNAME=hello_dev
obj-m += $(FNAME).o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

insert:
	sudo insmod $(FNAME).ko

remove:
	sudo rmmod $(FNAME)
