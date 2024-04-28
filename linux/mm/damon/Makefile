# SPDX-License-Identifier: GPL-2.0

EXTRAVERSION = +
PWD   := $(shell pwd)
obj-y				:= core.o
obj-$(CONFIG_DAMON_VADDR)	+= ops-common.o vaddr.o idt.o idt-utils.o idt-region.o idt-promote.o
obj-$(CONFIG_DAMON_PADDR)	+= ops-common.o paddr.o
obj-$(CONFIG_DAMON_SYSFS)	+= sysfs.o
obj-$(CONFIG_DAMON_DBGFS)	+= dbgfs.o
obj-$(CONFIG_DAMON_RECLAIM)	+= reclaim.o
obj-$(CONFIG_DAMON_LRU_SORT)	+= lru_sort.o
obj-m += idt_kmodule.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
install:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	sudo rm -r -f damon.data damon.data.old
