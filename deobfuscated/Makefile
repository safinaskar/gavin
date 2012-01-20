CC = gcc
LD = ld

# Older versions of GCC have a bug in -O2 optimization, hence -O1
CFLAGS = -O1 -W -Wall -Wno-unused-parameter -pedantic

QEMU = 0
MY_FLAGS = -std=c99 -DQEMU=$(QEMU)
FS_TAR = sh vi prim $(shell echo *.c *.h)

####

all: kernel fs.tar

####

kernel: mkkernel
	./mkkernel > kernel
mkkernel: mkkernel.o common.o
	$(CC) -o mkkernel mkkernel.o common.o
mkkernel.o: mkkernel.c common.h
	$(CC) -c $(CFLAGS) $(MY_FLAGS) mkkernel.c

####

fs.tar: $(FS_TAR)
	tar -cf fs.tar $(FS_TAR)

sh: sh.o user.o common.o
	$(LD) -s -o sh sh.o user.o common.o
vi: vi.o user.o common.o
	$(LD) -s -o vi vi.o user.o common.o
prim: prim.o user.o common.o
	$(LD) -s -o prim prim.o user.o common.o

sh.o: sh.c user.h common.h
	$(CC) -c $(CFLAGS) $(MY_FLAGS) sh.c
vi.o: vi.c user.h common.h
	$(CC) -c $(CFLAGS) $(MY_FLAGS) vi.c
prim.o: prim.c user.h common.h
	$(CC) -c $(CFLAGS) $(MY_FLAGS) prim.c

user.o: user.c user.h common.h
	$(CC) -c $(CFLAGS) $(MY_FLAGS) user.c

####

common.o: common.c common.h
	$(CC) -c $(CFLAGS) $(MY_FLAGS) common.c

####

clean:
	rm -f kernel mkkernel fs.tar sh vi prim *.o
