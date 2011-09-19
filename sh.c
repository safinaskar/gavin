/* sh.c -- code of user-space programs `sh' and `vi */

#include "common.h"

#define LINES 25
#define COLUMNS 64

#define CONTENT_TOP (TITLE_HEIGHT + 9)
#define CONTENT_LEFT 5

int syscall(int arg, enum syscallnum_t syscallnum);

/* This functions just call syscall(...) */
void *malloc(int size);
int file_size(const char *fd);
void create_process(const char *cmdline);
char *open(const char *file_name);

/* Functions may be in any order */
void _start(struct task_t *my_task, enum msgnum_t msgnum, int arg){
	/* sh and vi use my_task->data */
	int *line = (int *)my_task->data; /* Current line in vi */
	int *column = (int *)(my_task->data + 4); /* Current cursor position in sh */
	char **content = (char **)(my_task->data + 8); /* Pointer to content of window (as string) */
	
	int shell = my_task->cmdline[0] == 's'; /* Are we shell? In fact, this is boolean variable */

	switch(msgnum){
		case msg_init:
			{
				my_task->begin = 0; /* At first window appears in top left corner */

				my_task->size = (TITLE_HEIGHT + LINES * SYMBOL_HEIGHT + 14) * SCREEN_WIDTH + COLUMNS * SYMBOL_WIDTH + 10;

				*line = 0;
				*column = 1;

				const char *fd = open(my_task->cmdline + 3); /* I opened this file for reading, so I put file descriptor to `const char *' */
				int size = file_size(fd);
				int content_size = (size > LINES + 1 ? size : LINES + 1) * COLUMNS;
				*content = (char *)malloc(content_size);

				int lin = 0;
				int col = 0;

				for(int i = 0; i != size; ++i){
					if(fd[TAR_DATA_OFFSET + i] == '\n'){
						for(; col != COLUMNS; ++col){
							(*content)[lin * COLUMNS + col] = 0;
						}
					}else{
						(*content)[lin * COLUMNS + col] = fd[TAR_DATA_OFFSET + i];
						++col;
					}
					if(col == COLUMNS){
						++lin;
						col = 0;
					}
				}

				for(int i = lin * COLUMNS + col; i != content_size; ++i){
					(*content)[i] = 0;
				}
			}
			break;
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
		case msg_key:
			if(shell){
				if(arg == '\n'){
					for(int i = 0; i < LINES * COLUMNS; ++i){
						(*content)[i] = (*content)[i + COLUMNS];
					}
					*column = 1;
					create_process(*content + (LINES - 2) * COLUMNS + 1);
				}else{
					(*content)[(LINES - 1) * COLUMNS + *column] = arg;
					++*column;
				}
			}else{
				switch(arg){
					case 0x37: /* Up */
						--*line;
						break;
					case 0x71: /* Down */
						++*line;
						break;
					case 0x38: /* PgUp */
						*line -= LINES - 1;
						break;
					case 0x77: /* PgDown */
						*line += LINES - 1;
						break;
				}
			}
			break;
	}
}

int syscall(int arg, enum syscallnum_t syscallnum){
	return (**(int(**)(int, enum syscallnum_t syscallnum))START)(arg, syscallnum);
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
