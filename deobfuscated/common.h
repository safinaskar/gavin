/* common.h - common header for all files */

#ifndef _COMMON_H
#define _COMMON_H

/* Address at which a Linux bootloader loads an x86 kernel image. Also this is pointer to some symbol in the huge string */
#define START ((char *)0x90200)

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024

/* Height of title of window */
#define TITLE_HEIGHT 18

#define SYMBOL_HEIGHT 16
#define SYMBOL_WIDTH 8

#if (QEMU == 1)
# define BLACK      0
# define DARK_GRAY  0x38
# define GRAY       7
# define LIGHT_GRAY 63 /* Light gray is white in QEMU mode, sorry */
# define WHITE      63

# define CYAN       3
# define BLUE       8
#else
# define BLACK      0
# define DARK_GRAY  20
# define GRAY       24
# define LIGHT_GRAY 28
# define WHITE      31

# define CYAN       125
# define BLUE       176
#endif

struct task_t;

/* Message numbers */
enum msgnum_t
{
  msg_init = 1, /* Initialize process, "arg" (see below) is ignored */
  msg_render, /* Render content of window to a buffer, "arg" is the buffer */
  msg_key /* Key is pressed, "arg" is key */
};

/* Message handler */
typedef void (*msghandler_t)(struct task_t *my_task, enum msgnum_t msgnum, int arg);

struct task_t
{
  msghandler_t handler;
  const char *cmdline; /* cmdline is pointer to field real_cmdline (see below), which really contains command line */
  struct task_t *next;
  int begin; /* Coordinate of left top corner of window */
  int size; /* Size of window, difference between coordinates of top left and bottom right corners */
  char data[44]; /* Application may use this field as it want */
  char real_cmdline[100];
};

/* System call numbers. See declarations in sh.c */
enum syscallnum_t
{
  /* Allocates memory. Takes size of memory and returns pointer to it. The memory is not filled with zeros */
  SYS_malloc = 1,

  /* Takes file descriptor (see below) and returns file size */
  SYS_file_size,

  /* Takes command line and creates process */
  SYS_create_process,

  /*
   * Opens file. Takes file name and returns file descriptor.
   * In fact, file descriptor is pointer to file structure in tar archive, which consists of some meta-information and file content (see TAR_DATA_OFFSET below).
   * You can write to file (of course, you will write to memory, all changes are not saved), but sh and vi don't use this feature.
   */
  SYS_open
};

/* If "fd" is file descriptor, "fd + TAR_DATA_OFFSET" is pointer to file content */
#define TAR_DATA_OFFSET 512

/* "buffer" is pointer to memory which we want to render to */
/* render_rectangle(b, t, l, h, w, c) and render_rectangle(b + t * SCREEN_WIDTH + l, 0, 0, h, w, c) are the same */
void render_rectangle(unsigned char *buffer, int top, int left, int height, int width, unsigned char color);
void render_symbol(unsigned char *buffer, char symbol, unsigned char color);

#endif /* _COMMON_H */
