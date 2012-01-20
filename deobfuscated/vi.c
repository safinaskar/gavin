#include "user.h"

void _start(struct task_t *my_task, enum msgnum_t msgnum, int arg){
	int *line = (int *)my_task->data; /* Current line */
	char **content = (char **)(my_task->data + 8); /* Pointer to content of window (as charaster array) */
	
	switch(msgnum){
		case msg_init:
			{
				my_task->begin = 0; /* At first window appears in the top left corner */
				my_task->size = SIZE;

				*line = 0;

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
			render_content((unsigned char *)arg + my_task->begin, *content + *line * COLUMNS);
			break;
		case msg_key:
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
			break;
	}
}
