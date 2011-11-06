#include "user.h"

void _start(struct task_t *my_task, enum msgnum_t msgnum, int arg){
	int *prime = (int *)(my_task->data + 4); /* Prime number just printed */
	char **content = (char **)(my_task->data + 8); /* Pointer to content of window (as charaster array) */

	switch(msgnum){
		case msg_init:
			my_task->begin = 0; /* At first window appears in the top left corner */
			my_task->size = SIZE;

			*prime = 1;

			*content = (char *)malloc((LINES + 1) * COLUMNS);

			for(int i = 0; i != (LINES + 1) * COLUMNS; ++i){
				(*content)[i] = 0;
			}
			break;
		case msg_render:
			{
				char *stdout = *content;

				*stdout++ = 'P'; *stdout++ = 'r'; *stdout++ = 'e'; *stdout++ = 's'; *stdout++ = 's'; *stdout++ = ' ';
				*stdout++ = 'a'; *stdout++ = 'n'; *stdout++ = 'd'; *stdout++ = ' ';
				*stdout++ = 'h'; *stdout++ = 'o'; *stdout++ = 'l'; *stdout++ = 'd'; *stdout++ = ' ';
				*stdout++ = 'a'; *stdout++ = 'n'; *stdout++ = 'y'; *stdout++ = ' ';
				*stdout++ = 'k'; *stdout++ = 'e'; *stdout++ = 'y';

				render_content((char *)arg + my_task->begin, *content);
			}
			break;
		case msg_key:
			{
				/* Finding next prime */
				++*prime;
				for(;;){
					int i;
					for(i = 2; i != *prime; ++i){
						if(*prime % i == 0){
							break;
						}
					}
					if(*prime == i){
						break;
					}
					++*prime;
				}

				/* Printing */
				int j = 0;
				int i = *prime;
				while(i != 0){
					/* Move to right */
					for(int k = j; k != 0; k--){
						(*content)[(LINES - 1) * COLUMNS + k] = (*content)[(LINES - 1) * COLUMNS - 1 + k];
					}

					(*content)[(LINES - 1) * COLUMNS] = '0' + i % 10;
					i /= 10;
					j++;
				}

				/* Move to up */
				for(int i = 0; i < LINES * COLUMNS; ++i){
					(*content)[i] = (*content)[i + COLUMNS];
				}
			}
			break;
	}
}
