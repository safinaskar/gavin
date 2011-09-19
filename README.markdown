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
Download [original Gavin from the IOCCC site](http://www.ioccc.org/2004/2004.tar.gz) (this is winners of IOCCC 2004, so it content not only Gavin, but also other programs).

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
* If you cannot compile Gavin use precompiled `orig/kernel` and `orig/fs.tar` from this package for real hardware and orig/qemu-kernel and orig/qemu-fs.tar for Qemu


How to run deobfuscated Gavin (based on Gavin's comments)?
----------------------------------------------------------
* First, build the kernel and filesystem (just `make`).
(You can skip this step because the package contents precompiled `deobfuscated/kernel` and `deobfuscated/fs.tar` for real hardware and deobfuscated/qemu-kernel and
deobfuscated/qemu-fs.tar for Qemu.)
In short, you need to build this on an x86 Linux machine.
If you wish to try to compile on an OS other than Linux you may need modify the makefile to compile `sh` using a cross-compiler that produces elf binaries --
i.e. a cross-compiler targeting x86 Linux. In short, you need to build this on an x86 Linux machine. Also, you need very old version of gcc -- 3.3.5 or similar
* Next, find a suitable machine to run the OS on. It should be perfectly safe, and since it does not directly attempt to access any hard disk drives
(only a ramdisk loaded by the bootloader) it should do no damage to your existing setup, BUT I ACCEPT NO LIABILITY FOR ANY DAMAGE DONE BY THIS PROGRAM.
Run it at your own risk. The OS has been tested on half a dozen machines or so, and worked on all but one. The failure came in the form of the machine rebooting itself midway
through loading the OS, with no nasty side-effects.
* Hardware requirements - i386 or better processor, but the GUI is surprisingly power-hungry, so I would recommend a fast processor (has been tested on p3/p4/athlon machines).
Minimum RAM is something like 32MB. PS2 mouse & keyboard required - no USB I'm afraid, and the keyboard map is for a UK keyboard. Uses Vesa VBE 3.0, so should work on any modern
graphics card (has been tested on Nvidia, Matrox, and SiS cards).
* Now you need to use virtual machine (this is simplest way) or a bootloader to load the OS.
** I recommend you Qemu. Just type command `qemu -kernel /path/to/kernel -initrd /path/to/fs.tar /dev/null`. But in this case you must compile package with `make QEMU=1`
** If you run GRUB 2, then at the boot menu simply hit "c" to get a console,
then type `linux16 (hd0,1)/<PATH>/kernel` (substituting appropriate harddrive/partition numbers & path), then `initrd16 (hd0,1)/<PATH>/fs.tar`, and finally `boot`.
* Assuming the OS has booted, what can you do now? Well, start by trying `sh`, to open another shell. Then, try `vi mkkernel.c`, to open up the kernel source in a text file viewer
(up/down or pgup/pgdown to scroll). Also try `prim`, it is written by the IOCCC judges. [Note that the provided applications, `sh` and `vi` both have windows of the same size, and that open in the top left corner of the screen,
so you may need drag the windows around a bit to tell them apart.]
* To shut down - just hit the power button. :-)


How deobfuscated Gavin works (based..?
-----------------------------
This is a 32-bit multitasking operating system for x86 computers,
with GUI and filesystem, support for loading and executing user
applications in elf binary format, with ps2 mouse and keyboard drivers,
and vesa graphics.  And a command shell.  And an application -
a simple text-file viewer.

The OS has ended up as a traditional monolith with one entry point
for syscalls, with apps treated as cooperative-multitasking tasks
that can be sent different messages (e.g. "initialize", "key hit",
"render to buffer") through their main entry point, and which will
return control to the OS having performed the necessary work.
Applications are are passed a pointer to their task structure,
which is partially defined by the OS, and partially for
the application's own use.

The program compiles into a tool to build a kernel image,
so having built the program, the makefile will run it,
piping the output into a file called `kernel`.
The makefile will then proceed to build a root filesystem image.
This involves rebuilding the program with different compiler flags,
then building a tar file containing the resulting programs
(the filesystem format supported by the OS is the tar-file format).

For further usage information see `gavin_install.txt`.

The filenames `vi` and `sh` are significant, and should not be changed.

Known "features".

Known issues are really too plentiful to list.

If the mouse pointer goes off the left hand side of the screen
it will reappear on the right, and vice-versa.
If it goes off the top or bottom, it will go and corrupt some memory.

The file system is kinda optimistic about matching names, so,
for example if you type the command `shell` into a command-line
it will execute the program `sh` - close enough a match for it.

The elf binaries are not loaded at the correct address,
and their entry point must be the start address of the text segment.

The keyboard driver can cope with the basic alpha-numeric keys,
but gets confused by fancy things like "shift" or "backspace".

In the text-file viewer, `vi`, the up/down and pgup/pgdn keys
scroll up or down by one line.  There is nothing to stop you
[from] scrolling above the top of the file, and pressing any
other keys may have an undefined effect.

The x86 is bootstrapped into 32bit mode in 6 instructions,
with 4 more to set up data/stack segments and a stack-pointer
to allow C code to be run.  On top of this there are also about
a dozen instructions to switch the video card into graphics mode.
All in all, a relatively tiny number of instructions next to
the size of the C program.  Also, the string is mostly composed
of data - a Linux-esque kernel header for the bootloader,
protected mode descriptor tables, keyboard maps, etc.
(I should also mention that it contains mini functions
to perform an x86 "in" and "out" instruction - to allow
the keyboard & mouse to be driven from C code).

Porting to another architecture should be relatively easy* -
the string simply needs be replaced with one containing
data & code suitable for the new target platform.
Accesses to data in the string are made relative to the define `V`,
so these may need updating as appropriate (0x90200 is the address
at which a Linux bootloader loads an x86 kernel image).

* ;-)





links




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
