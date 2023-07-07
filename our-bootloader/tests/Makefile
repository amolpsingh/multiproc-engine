## uncomment COMMON_SRC and comment out STAFF_OBJS
## COMMON_SRC += mbr.c
#STAFF_OBJS +=  staff-mbr.o
#
## uncomment COMMON_SRC and comment out STAFF_OBJS
## COMMON_SRC += fat32.c
#STAFF_OBJS +=  staff-fat32.o
#
## the tests in decreasing order of difficulty.
#PROGS := $(wildcard ./test-basic/*.c)
#
#CFLAGS_EXTRA  = -Iexternal-code
#
## a list of all of your object files.
#COMMON_SRC := a.c b.c c.c hello.c
#
## external-code/bzt-sd.c 
#
#TTYUSB = 
#BOOTLOADER = my-install
#RUN = 1
#
#EXCLUDE ?= grep -v simple_boot
#GREP_STR := 'HASH:\|ERROR:\|PANIC:\|PASS:\|TEST:'
#include $(CS140E_2023_PATH)/libpi/mk/Makefile.template-v2
#
#clean::
#	rm -f tests/*.list tests/*.o tests/*.elf tests/*~ tests/*.bin
#	make -C external-code clean
PROGS := $(wildcard ./test-basic/*.c) 

# shared code linked with all driver programs.

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = pi-install

# uncomment if you want ita to automatically run.
RUN=0

include $(CS140E_FINALPROJ_PATH)/libpi/mk/Makefile.template
