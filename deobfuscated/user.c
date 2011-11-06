/* user.c -- coomon code for all user-space applications */

#include "user.h"

int syscall(int arg, enum syscallnum_t syscallnum){
	return (**(int(*const *)(int, enum syscallnum_t syscallnum))START)(arg, syscallnum);
}

void *malloc(int size){
	return (void *)syscall(size, SYS_malloc);
}

int file_size(const char *fd){
	return syscall((int)fd, SYS_file_size);
}

void create_process(const char *cmdline){
	syscall((int)cmdline, SYS_create_process);
}

char *open(const char *file_name){
	return (char *)syscall((int)file_name, SYS_open);
}

void render_content(char *buffer, const char *content){
	/* Frame */
	render_rectangle(buffer, TITLE_HEIGHT + 8, 4,            LINES * SYMBOL_HEIGHT + 3, COLUMNS * SYMBOL_WIDTH + 2, BLACK);

	/* Area */
	render_rectangle(buffer, CONTENT_TOP,      CONTENT_LEFT, LINES * SYMBOL_HEIGHT + 1, COLUMNS * SYMBOL_WIDTH,     WHITE);

	/* Text */
	for(int i = 0; i != LINES; ++i){
		for(int j = 0; j != COLUMNS; ++j){
			render_symbol(buffer + (CONTENT_TOP + i * SYMBOL_HEIGHT) * SCREEN_WIDTH + CONTENT_LEFT + j * SYMBOL_WIDTH, content[i * COLUMNS + j], BLACK);
		}
	}
}
