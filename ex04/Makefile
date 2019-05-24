obj-m += main.o

all: modules

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	sudo cp -v $(PWD)/main.ko /lib/modules/$(shell uname -r)
	sudo depmod -a

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

.PHONY: all modules install clean
