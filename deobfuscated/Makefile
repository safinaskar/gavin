CC = cc
LD = ld

# Older versions of GCC have a bug in -O2 optimization, hence -O1
CFLAGS = -O1

QEMU =
MY_FLAGS = -std=c99 -DQEMU=$(QEMU)
FS_TAR = sh vi prim mkkernel.c sh.c common.h common.c

####

all: kernel fs.tar

####

kernel: mkkernel
	./mkkernel > kernel
mkkernel: mkkernel.o common.o
	$(CC) -o mkkernel mkkernel.o common.o
mkkernel.o: mkkernel.c
	$(CC) -c $(CFLAGS) $(MY_FLAGS) mkkernel.c

####

fs.tar: $(FS_TAR)
	tar -cf fs.tar $(FS_TAR)
vi: sh
	cp sh vi
sh: sh.o common.o
	$(LD) -s -o sh sh.o common.o
sh.o:
	$(CC) -c $(CFLAGS) $(MY_FLAGS) sh.c

####

common.o:
	$(CC) -c $(CFLAGS) $(MY_FLAGS) common.c

####

clean:
	rm -f *.o mkkernel kernel sh vi fs.tar
