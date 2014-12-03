obj-m := horse_dev.o

KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

.PHONY: clean
clean:
	rm .horse_dev.ko.cmd
	rm .horse_dev.mod.o.cmd
	rm .horse_dev.o.cmd
	rm -r .tmp_versions/
	rm Module.symvers
	rm horse_dev.ko
	rm horse_dev.mod.c
	rm horse_dev.mod.o
	rm horse_dev.o
	rm modules.order
