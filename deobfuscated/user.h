/* user.h - coomon code for all user-space applications */

#include "common.h"

#define LINES 25
#define COLUMNS 64

#define CONTENT_TOP (TITLE_HEIGHT + 9)
#define CONTENT_LEFT 5

#define SIZE ((TITLE_HEIGHT + LINES * SYMBOL_HEIGHT + 14) * SCREEN_WIDTH + COLUMNS * SYMBOL_WIDTH + 10)

/* This functions just call syscall(...) */
void *malloc(int size);
int file_size(const char *fd);
void create_process(const char *cmdline);
char *open(const char *file_name);

/* This code renders content of window (black frame, white area and text) */
void render_content(unsigned char *buffer, const char *content);
