obj-m += led.o

all:
	make ARCH=arm LOCALVERSION=-vybrid-armv7-r1 -C  /home/cedrus/wp/k70/armv7_devel/KERNEL M=$(PWD) modules

clean:
	make ARCH=arm LOCALVERSION=-vybrid-armv7-r1 -C  /home/cedrus/wp/k70/armv7_devel/KERNEL M=$(PWD) clean
