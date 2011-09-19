![screenshot.png](https://github.com/safinaskar/gavin/raw/master/screenshot.png)

Introduction
------------

In 2004 Gavin Barraclough created multitasking operating system with GUI, filesystem and mouse support.
It is written in C, it has only one source file and this file has size 3.5 kilobytes (his OS has no name, so I will call it "Gavin")!
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

In 2011 I deobfuscated this code, added to it comments and put to this site. For example, this is same code:

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

How to download and compile deobfuscated Gavin?
-----------------------------------
Click button "Download". You don't need to compile Gavin. Archive already content files `kernel` and `fs.tar`, which are Gavin's kernel and initramfs disk.


How to download original Gavin?
-------------------------------
Download [Original Gavin](http://www.ioccc.org/2004/2004.tar.gz)

This is winners of IOCCC 2004, so it content not only Gavin, but also other programs.
To compile you need 32-bit x86 GNU/Linux and very old gcc 3.3.5 (or similar).
cd to directory with Gavin and type `make`.

How to run Gavin?
-----------------
You need any loader which can load Linux kernel. For example, if you have GRUB 2, press "c" while booting and type:
	linux16 /path/to/kernel
	initrd16 /path/to/fs.tar

Also (and this is the simplest way) you can run Gavin in Qemu. Type following command:
	qemu -kernel /path/to/kernel -initrd /path/to/fs.tar /dev/null


-----
Copyright (C) 2004, Landon Curt Noll, Simon Cooper, Peter Seebach
and Leonid A. Broukhis. All Rights Reserved. Permission for personal,
educational or non-profit use is granted provided this copyright and
notice are included in its entirety and remains unaltered.  All other
uses must receive prior permission from the contest judges.
