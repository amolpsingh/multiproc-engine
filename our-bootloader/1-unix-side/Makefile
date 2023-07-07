PROG_SRC = myprogram.c
SRC = simple-boot.c put-get.c dependancies.c
CFLAGS += -I../2-pi-side

include $(CS140E_FINALPROJ_PATH)/libunix/mk/Makefile.unix

trace:
	./my-install --trace-control hello.bin

trace-all:
	./my-install --trace-all hello.bin

.PHONEY: trace trace-all