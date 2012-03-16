What is Gavin?
--------------
In 2004 Gavin Barraclough created multitasking operating system for x86 with GUI, filesystem and mouse support (the OS has no name, so I will call it Gavin).
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

In 2011 I deobfuscated and commented this code. For example, this is same code:

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
This package consists of all information about Gavin you want to know, namely:

* Some files which can help you to run original Gavin. They are in the directory `orig`
* Deobfuscated Gavin. It is in directory `deobfuscated`


How to run original Gavin?
--------------------------
Download [it from the IOCCC site](http://www.ioccc.org/2004/2004.tar.gz) (there are also other winners of the IOCCC in this archive).

Then follow instructions in files `gavin.hint` and `gavin_install.txt` (or use russian translate `orig/gavin_ru.hint` and `orig/gavin_install_ru.txt` from this package), but note following:

* If you want to build only Gavin, type `make gavin`
* The most simple way to run Gavin is Qemu. At first try to use Qemu, then -- other methods
* To boot from Qemu type `qemu -kernel /path/to/kernel -initrd /path/to/fs.tar /dev/null` (of course, you need to build Gavin first)
* You will get strange colors in Qemu
* LILO is obsolete. You don't need files `lilo.conf` and `boot.b`. Use any modern Linux loader. I would recommend GRUB 2. If you have GRUB 2, press "c" while booting and type:

	linux16 /path/to/kernel
	initrd16 /path/to/fs.tar
	boot

* Floppy disks are obsolete. You can boot Gavin from any disk
* You need very old gcc 3.x
* If you cannot compile Gavin use precompiled `orig/kernel` and `orig/fs.tar` from this package for real hardware and `orig/qemu-kernel` and `orig/qemu-fs.tar` for Qemu
* This package content source of `prim` utility in `orig/prim.c` (thanks to IOCCC judges)

How to run deobfuscated Gavin (based on Gavin's comments)?
----------------------------------------------------------
* First, build the kernel and filesystem (just `make`).
(You can skip this step because the package contents precompiled `deobfuscated/kernel` and `deobfuscated/fs.tar` for real hardware and `deobfuscated/qemu-kernel` and
`deobfuscated/qemu-fs.tar` for Qemu.)
In short, you need to build this on an x86 GNU/Linux machine.
If you wish to try to compile on an OS other than GNU/Linux you may need modify the Makefile to compile `sh`, `vi` and `prim` using a cross-compiler that produces ELF binaries --
i.e. a cross-compiler targeting x86 GNU/Linux. Also, you need very old version of gcc -- 3.x.
* Next, find a suitable machine to run the OS on. It should be perfectly safe, and since it does not directly attempt to access any hard disk drives
(only a ramdisk loaded by the bootloader) it should do no damage to your existing setup, BUT I ACCEPT NO LIABILITY FOR ANY DAMAGE DONE BY THIS PROGRAM.
Run it at your own risk. The OS has been tested on half a dozen machines or so, and worked on all but one. The failure came in the form of the machine rebooting itself midway
through loading the OS, with no nasty side-effects.
* Hardware requirements -- i386 or better processor, but the GUI is surprisingly power-hungry, so I would recommend a fast processor (has been tested on Pentium 3/Pentium 4/Athlon machines).
Minimum RAM is something like 32MB. PS2 mouse & keyboard required -- no USB I'm afraid. Uses Vesa VBE 3.0, so should work on any modern
graphics card (has been tested on Nvidia, Matrox, and SiS cards).
* Now you need to use virtual machine (this is simplest way) or a bootloader to load the OS.

 * I recommend you Qemu. Just type command `qemu -kernel /path/to/kernel -initrd /path/to/fs.tar /dev/null`. But in this case you must compile Gavin by command `make QEMU=1`.
 * If you run GRUB 2, then at the boot menu simply hit "c" to get a console,
then type `linux16 (hd0,1)/<PATH>/kernel` (substituting appropriate harddrive/partition numbers & path), then `initrd16 (hd0,1)/<PATH>/fs.tar`, and finally `boot`.

* Assuming the OS has booted, what can you do now? Well, start by trying `sh`, to open another shell. Then, try `vi mkkernel.c`, to open up the kernel source in a text file viewer
(up/down or PgUp/PgDown to scroll). Also try `prim`. Note that all provided applications have windows of the same size, and that open in the top left corner of the screen,
so you may need drag the windows around a bit to tell them apart.
* To shut down -- just hit the power button. :-)


How deobfuscated Gavin works?
-----------------------------
See `deobfuscated/README-EN`.


Links
-----
* http://pdos.csail.mit.edu/6.828/2011/xv6.html - XV6, simple UNIX-like OS. Its source has size 277 kilobytes! It is Gavin x 78!
* http://wiki.osdev.org - wiki about OS development


Authors
-------
Gavin Barraclough, Flat 20, 83 Newton Street, Manchester, M1 1EP, U.K. -- creation of OS

IOCCC 2004 judges -- writing `prim` program

Askar Safin <safinaskar@mail.ru>, Russia -- deobfuscation

Site: https://github.com/safinaskar/gavin


Copyright
---------
All sources are in public domain. Docs use following copyright:

Copyright (C) 2004, Landon Curt Noll, Simon Cooper, Peter Seebach
and Leonid A. Broukhis. All Rights Reserved. Permission for personal,
educational or non-profit use is granted provided this copyright and
notice are included in its entirety and remains unaltered. All other
uses must receive prior permission from the contest judges.
