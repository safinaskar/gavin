![screenshot.png](https://github.com/safinaskar/gavin/raw/master/deobfuscated/screenshot.png)

What is Gavin?
--------------
In 2004 Gavin Barraclough created multitasking operating system with GUI, filesystem and mouse support (the OS has no name, so I will call it "Gavin").
It is written in C, it has only one source file and this file has size 3.5 kilobytes!
Then Gavin sent OS to [IOCCC](http://www.ioccc.org/) (the International Obfuscated C Code Contest) and won. Of course, code is very obfuscated, for example:

	F
	U (Y (t + 28) + 1536) |=
	62 & -n;
	M
	U (d + D) =
	X (D, Y (t + 12) + 26628, 412162) ? X (D, Y (t + 12) + 27653,
						410112) ? 31 : 0 : U (d + D);
	for (; j < 12800; j += 8)
		P (d + 27653 + Y (t + 12) + ' ' * (j & ~511) + j % 512,
			U (Y (t + 28) + j / 8 + 64 * Y (t + 20)), 0);
	}

In 2011 I deobfuscated this code and added to it comments. For example, this is same code:

	case msg_render:
		/* This code draws content of window (black frame, white area and text) */
		{
			char *dest = (char *)arg;
			if(shell){
				(*content)[(LINES - 1) * COLUMNS] = '>';
			}
			/* Frame */
			draw_rectangle(dest + my_task->begin, TITLE_HEIGHT + 8, 4,            LINES * SYMBOL_HEIGHT + 3, COLUMNS * SYMBOL_WIDTH + 2, BLACK);
			/* Area */
			draw_rectangle(dest + my_task->begin, CONTENT_TOP,      CONTENT_LEFT, LINES * SYMBOL_HEIGHT + 1, COLUMNS * SYMBOL_WIDTH,     WHITE);
			/* Text */
			for(int i = 0; i != LINES; ++i){
				for(int j = 0; j != COLUMNS; ++j){
					draw_symbol(dest + my_task->begin + (CONTENT_TOP + i * SYMBOL_HEIGHT) * SCREEN_WIDTH + CONTENT_LEFT + j * SYMBOL_WIDTH, (*content)[(*line + i) * COLUMNS + j], BLACK);
				}
			}
		}
		break;


What is this package?
---------------------
This package contents two thing:
* Some files which can help you to run original Gavin. They is in directory `orig`.
* Deobfuscated Gavin. It is in directory `deobfuscated`

How to run original Gavin?
--------------------------
Download [Original Gavin from the IOCCC site](http://www.ioccc.org/2004/2004.tar.gz) (this is winners of IOCCC 2004, so it content not only Gavin, but also other programs).

Then follow instructions in files `gavin.hint` and `gavin_install.txt`, but note following:

* The most simple way to run Gavin is Qemu. At first try to use Qemu, then -- other methods
* To boot from Qemu type `qemu -kernel /path/to/kernel -initrd /path/to/fs.tar /dev/null`
* You will get strange colors in Qemu
* LILO is obsolete. You don't need files `lilo.conf` and `boot.b`. Use any modern Linux loader. I recommend you GRUB 2. If you have GRUB 2, press "c" while booting and type:

	linux16 /path/to/kernel
	initrd16 /path/to/fs.tar
	boot

* Floppy disks are obsolete. You can boot Gavin from any disk
* You need very old gcc 3.3.5
* If you cannot compile Gavin use precompiled orig/kernel and orig/fs.tar from this package for real hardware and orig/qemu-kernel and orig/qemu-fs.tar for Qemu





>>>>Also, You don't need to compile Gavin. Archive already content files `kernel` and `fs.tar`, which are Gavin's kernel and initramfs disk.


How to run Gavin?
-----------------
You need any loader which can load Linux kernel (also you need PS/2 keyboard and mouse). For example, if you have GRUB 2, p....

Also (and this is the simplest way) you can use any virtual machine. If you have Qemu, you need to use files `qemu-kernel` and `qemu-fs.tar`, type the following command:

	qemu -kernel /path/to/qemu-kernel -initrd /path/to/qemu-fs.tar /dev/null


And what to do after boot?
--------------------------
You have shell opened. Type `sh` to open another shell. Move windows. Type `vi mkkernel.c` (or `vi gavin.c` in original version) to view kernel code.
Use keys "Up", "Down", "PgUp" and "PgDown". Type `prim` to shell to get program which generates prime numbers.


Where can I find additional information?
----------------------------------------
See files `gavin.hint` and `gavin_install.txt`.

>>>>>>

Authors
-------
Gavin Barraclough
Flat 20, 83 Newton Street,
Manchester,
M1 1EP,
U.K.

Askar Safin <safinaskar@mail.ru>
Russia


Copyright
---------
Copyright (C) 2004, Landon Curt Noll, Simon Cooper, Peter Seebach
and Leonid A. Broukhis. All Rights Reserved. Permission for personal,
educational or non-profit use is granted provided this copyright and
notice are included in its entirety and remains unaltered.  All other
uses must receive prior permission from the contest judges.
