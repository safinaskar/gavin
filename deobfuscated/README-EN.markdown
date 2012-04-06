Introduction (based on Gavin's comments)
----------------------------------------
This is a 32-bit multitasking operating system for x86 computers,
with GUI and filesystem, support for loading and executing user
applications in elf binary format, with ps2 mouse and keyboard drivers,
and vesa graphics. And a command shell. And an application -
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
The makefile will then proceed to build a root filesystem image - a tar file
(the filesystem format supported by the OS is the tar-file format).

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
but gets confused by fancy things like `Shift` or `Backspace`.

In the text-file viewer, `vi`, the up/down and pgup/pgdn keys
scroll up or down by one line or by one screen. There is nothing to stop you
from scrolling above the top of the file, and pressing any
other keys may have an undefined effect.

The x86 is bootstrapped into 32bit mode in 6 instructions,
with 4 more to set up data/stack segments and a stack-pointer
to allow C code to be run. On top of this there are also about
a dozen instructions to switch the video card into graphics mode.
All in all, a relatively tiny number of instructions next to
the size of the C program. Also, the string is mostly composed
of data - a Linux-esque kernel header for the bootloader,
protected mode descriptor tables, keyboard maps, etc.
(I should also mention that it contains mini functions
to perform an x86 `in` and `out` instruction - to allow
the keyboard & mouse to be driven from C code).

Porting to another architecture should be relatively easy* -
the string simply needs be replaced with one containing
data & code suitable for the new target platform.
Accesses to data in the string are made relative to the define `START`,
so these may need updating as appropriate (`0x90200` is the address
at which a Linux bootloader loads an x86 kernel image).

* ;-)


Kernel and file system
----------------------
OS consists of two components:

* Kernel
* File system

OS supports only one file system type: tar archive, which is loaded as initramfs. Gavin cannot access to hard disk.
Kernel places in the file `kernel`, file system - in `fs.tar`. `fs.tar` contents `sh` (shell), `vi` (text viewer), `prim` (prime number generator) and some text files, which can be browsed by `vi`.

Originally OS had only one source file - `prog.c`. Then IOCCC judges renamed it to `gavin.c`. Then it was splited into following files:

* `common.h` and `common.c` - some common code
* `mkkernel.c` - kernel
* `user.h` and `user.c` - common code for user application
* `sh.c` - shell
* `vi.c` - text viewer
* `prim.c` - prime numbers generator

This is stages of building Gavin:

* `cc -c common.c`
* `cc -c mkkernel.c`
* `cc -c user.c`
* `cc -c sh.c`
* `cc -c vi.c`
* `cc -c prim.c`

* `cc -o mkkernel mkkernel.c common.o`. Now `mkkernel` is typical GNU/Linux application. It depends on libc
* `./mkkernel > kernel`. Now `kernel` is kernel of Gavin
* `ld -s -o sh sh.o user.o common.o`. Now `sh` is Gavin application. It doesn't depend on libc
* `ld -s -o vi vi.o user.o common.o`.
* `ld -s -o prim prim.o user.o common.o`.
* `tar -cf fs.tar sh vi prim ...`

`mkkernel` (of course) has entry point `_start` (which placed in libc). `_start` permorms some initialization, for example. it opens `stdout`. Then it call `main` (`main` placed in `mkkernel.c`).
`main` checks `argc == 0`. If `argc != 0` we are in GNU/Linux. In this case we write kernel code to stdout. At first we write some machine code and other data which placed in string
called "huge string". Then we write `mkkernel` code directly from memory. So `kernel` will content a large piece of code from `mkkernel`. This means that kernel will content same `main` function.

Let's assume we are booting kernel. At first kernel executes the machine code. Then it executes the code from `main` (now `main` is entry point to C code, not `_start`).
And it again checks `argc == 0`. But now we are on a real hardware, so `argc` really equals to zero. Therefore we perform kernel booting sequence.

`START` is address at which a Linux bootloader loads a kernel. So, we can access huge string by writing expressions like `*(char *)(START + 131)`.


System calls and messages
-------------------------
If kernel wants to say something to user application, it will send message to it. If application wants, it will send system call to kernel.

Kernel has function `syscall`. At first kernel takes address of this function and puts it to first 4 bytes from START. If application need syscall it will get value from this 4 bytes,
convert it to function pointer and call.

Every user application has only one function - `_start`. This is entry point. But `_start` isn't executed from the start of application to its end.
Instead, kernel call `_start` several times: at the start of application, at the key press, etc. So, `_start` is not similar to usual "main" function in typical programs. `_start` is
message handler. If kernel need to say something to program, it calls `_start` of this program with some arguments.

The second argument of `syscall` function is syscall number. There are 4 syscall numbers: SYS_malloc, SYS_file_size, SYS_create_process and SYS_open (see their meaning in the code).

The second argument of application's `_start` is message number. There are 3 message numbers: msg_init, msg_render and msg_key (see code, too).


`task_t` structure
------------------
Kernel uses C structure `task_t` to represent all information about task. Also, applications can use part of the structure (field `data`) to save its own data.
There is no another way to store static information between different `_start` calls. There is single-linked list of this structures,
and kernel's variable `head_task` is begin of this list. `task_t` has field `handler`, which is pointer to `_start` of process. If kernel calls `handler`, it will pass pointer to task
structure as the first argument.


Video memory, coordinates and arrays
------------------------------------
There is no 2-dimentional arrays in Gavin. All 2-dimentional data is represented as linear arrays. For example, if you want to get item j from row i of window's content,
you need to write: `(*content)[i * COLUMNS + j]`. This is why if you move mouse (or a window) to the left, it will appear in the right. Task structure has field `begin` which is
coordinate of begin of window, i. e. top left corner. Field `size` is size of a window, i. e. difference between top left corner and next pixel to bottom right corner (next in row).
So, if window corners has coordinates (X1, Y1) and (X2, Y2), we will have following formulas:

	begin = Y1 * SCREEN_WIDTH + X1
	size = (Y2 - Y1) * SCREEN_WIDTH + X2 - X1 + 1


Other notes
-----------
* Sometimes if I add some variable to kernel code, the OS will not work. But if I tell to compiler to optimize harder, it will work again. I think kernel's static memory is limited
* Deobfuscated Gavin is binary compatible with original. So you can run original `fs.tar` on top of the deobfuscated `kernel` (and vice-versa)
* `rendering` in code means rendering to a temporary buffer and `drawing` meand real drawing to a screen
* I'm not sure OS works with LILO
* Run `make QEMU=1` if you want to get right colors in Qemu
* Unused memory contents random data (not zeros)
* You cannot use C string constants, for example `"string"`


Bug fixes
---------
* PgUp and PgDn do right function
* You don't need to move the mouse to trigger the initial screen update in Qemu
* vi doesn't fail because of non-ASCII charasters
* vi works correctly with big files
* You will get right colors in Qemu
* You can name `sh` and `vi` as you want. Names are not significant.
