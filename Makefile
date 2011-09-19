# Older versions of GCC have a bug in -O2 optimization, hence -O1
CFLAGS = -O3
QEMU =
FLAGS = -std=c99 -DQEMU=$(QEMU)

all:
	cc -c $(CFLAGS) $(FLAGS) common.c
	cc $(CFLAGS) $(FLAGS) -o mkkernel mkkernel.c common.o # Order is important
	./mkkernel > kernel
	cc -c $(CFLAGS) $(FLAGS) sh.c
	ld -s -o sh sh.o common.o # Order is important
	cp sh vi
	tar -cf fs.tar sh vi prim *.c *.h
clean:
	rm -f *.o mkkernel kernel sh vi fs.tar
