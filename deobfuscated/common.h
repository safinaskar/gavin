/* common.h -- common header for all files */

/* Address at which a Linux bootloader loads an x86 kernel image. Also this is pointer to some symbol in the huge string */
#define START 0x90200

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024

/* Height of title of window */
#define TITLE_HEIGHT 18

#define SYMBOL_HEIGHT 16
#define SYMBOL_WIDTH 8

#ifndef QEMU
# define BLACK      0
# define DARK_GRAY  20
# define GRAY       24
# define LIGHT_GRAY 28
# define WHITE      31

# define CYAN       125
# define BLUE       176
#else
# define BLACK      0
# define DARK_GRAY  0x38
# define GRAY       7
# define LIGHT_GRAY 63 /* Light gray is white in QEMU mode, sorry */
# define WHITE      63

# define CYAN       3
# define BLUE       8
#endif

struct task_t;

/* Message numbers */
enum msgnum_t{
	msg_init = 1, /* Initialize process, `arg' (see below) is ignored */
	msg_render, /* Draw content of window, `arg' is pointer to memory which we want draw to */
	msg_key /* Key is pressed, `arg' is key */
};

/* Message handler */
typedef void (*msghandler_t)(struct task_t *my_task, enum msgnum_t msgnum, int arg);

struct task_t{
	msghandler_t handler;
	char *cmdline; /* cmdline is pointer to field real_cmdline (see below), which really contents command line */
	struct task_t *next;
	int begin; /* Coordinate of left top corner of window */
	int size; /* Size of window, difference between coordinates of top left and bottom right corners */
	char data[44]; /* Application may use this field as it want */
	char real_cmdline[100];
};

/* System call numbers. See declarations in sh.c */
enum syscallnum_t{
	/* Allocates memory. Takes size of memory and returns pointer to it. The memory is not filled with zeros */
	SYS_malloc = 1,

	/* Takes file descriptor (see below) and return file size */
	SYS_file_size,

	/* Takes command line and creates process */
	SYS_create_process,

	/*
	 * Opens file. Takes file name and returns file descriptor.
	 * In fact, file descriptor is pointer to file structure in tar archive, which consists of some meta-information and file content (see TAR_DATA_OFFSET below).
	 * You can read file and write to it (of course, you will write to memory), but sh and vi don't use writing feature.
	 */
	SYS_open
};

/* If `fd' is file descriptor, `fd + TAR_DATA_OFFSET' is pointer to file content */
#define TAR_DATA_OFFSET 512

/* `dest' is pointer to memory which we want to draw to */
void draw_rectangle(char *dest, int top, int left, int height, int width, char color);
void draw_symbol(char *dest, char symbol, char color);
