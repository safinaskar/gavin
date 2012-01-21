/* mkkernel.c -- source of kernel and `mkkernel' tool */

#include "common.h"

/* This functions are in huge string. This functions do machine instructions `in' and `out' */
#define  IN(addr     ) ((*(int  (*)(int     ))(START - 8))(addr     ))
#define OUT(addr, val) ((*(void (*)(int, int))(START + 8))(addr, val))

void *memory_end; /* End of allocated memory */
struct task_t *head_task; /* Pointer to front task, head of list */

int syscall(int arg, enum syscallnum_t syscallnum);

/* draw_all draws all to real video memory */
void draw_all(int mouse);

int putchar(int c);

/* This function must be first. Main function for mkkernel and kernel */
int main(int argc, char *argv[]){
	if(argc == 0){
		/* This is real kernel */

		memory_end = (void *)0x1100800;
		head_task = 0;

		/* Setting entry point for system calls. START + 208 is end of huge string and begin of code of `main' function */
		*(int *)START = (int)START + 208 + ((int)syscall - (int)main);

		int mouse = (SCREEN_HEIGHT / 2) * SCREEN_WIDTH + SCREEN_WIDTH / 2; /* Position of center of `X' */

		/* Initialization of keyboard and mouse. I don't understand this code */

		OUT(100, 209);

		while(IN(100) & 3){
			IN(96);
		}

		OUT(96, 223);

		while(IN(100) & 3){
			IN(96);
		}

		OUT(100, 168);

		while(IN(100) & 3){
			IN(96);
		}

		OUT(96, 0);

		while(IN(100) & 3){
			IN(96);
		}

		OUT(100, 212);

		while(IN(100) & 3){
			IN(96);
		}

		OUT(96, 244);

		while((IN(100) & 3) == 0);

		IN(96);

		/* End of initialization */

		{
			/* Starting initial `sh' process. I don't know why we cannot use normal string "sh" */
			const char sh[] = {'s', 'h', 0};
			syscall((int)sh, SYS_create_process);
		}

		draw_all(mouse); /* Initial screen update */

		for(;;){
			int in_100_returned;
			while(((in_100_returned = IN(100)) & 3) == 0);
			int in_96_returned = IN(96);

			if(in_100_returned & 32){
				int moving = 0; /* `Are we moving some window?' In fact, this is boolean variable */

				if(in_96_returned & 1){
					/* Mouse is down, let's resort windows */

					struct task_t *curr = 0;
					struct task_t *prev = 0;

					for(prev = 0, curr = head_task; curr != 0; prev = curr, curr = curr->next){
						/* If mouse is inside curr */
						if(mouse >= curr->begin && mouse < curr->begin + curr->size && (mouse - curr->begin) % SCREEN_WIDTH < curr->size % SCREEN_WIDTH){
							moving = 1;
							if(prev != 0){
								prev->next = curr->next;
								curr->next = head_task;
								head_task = curr;
							}
							break;
						}
					}
				}

				while((IN(100) & 3) == 0);

				{
					int offset = (signed char)IN(96);
					mouse += offset;
					if(moving){
						head_task->begin += offset;
					}
				}

				while((IN(100) & 3) == 0);

				{
					int offset = (signed char)IN(96) * SCREEN_WIDTH;
					mouse -= offset;
					if(moving){
						head_task->begin -= offset;
					}
				}

				draw_all(mouse);
			}else if((in_96_returned & 128) == 0){
				/* Keyboard */

				/* in_96_returned & 63 is hardware key code. For example, `q' has code 16, 'w' -- 17, 'e' -- 18 */
				/* We look at keybord layout which is in the huge string */
				(*head_task->handler)(head_task, msg_key, *(START + 131 + (in_96_returned & 63)));

				draw_all(mouse);
			}
		}
	}else{
		/* This is `mkkernel' tool */

		/* The first 496 bytes of kernel may be any symbols */
		for(int i = 0; i != 496; ++i){
			putchar(0);
		}

		/*
		 * The huge string contents machine code and data.
		 * The machine code uses hardware ports, for example 10h for graphical programming.
		 * Also CPU is switched to 32-bit protected mode (lidt/lgdt/lmsw), but kernel and applications use same address space.
		 * I don't fully understand this code.
		 */
		const char huge_string[] =
			/* START - 16  */ "\000\010\020\001\000\000\000\000\131\132\122\121\354\303\125\252"
			/* START       */ "\353\056\110\144\162\123\000\040\131\132\130\120\122\121\356\303"
			/* START + 16  */ "\000\000\000\200\153\002\011\000\000\000\000\000\000\000\000\000"
			/* START + 32  */ "\000\000\000\000\000\000\014\000\000\000\000\000\377\377\377\000"
			/* START + 48  */ "\270\001\117\271\005\101\120\121\277\000\020\216\307\315\020\133"
			/* START + 64  */ "\130\100\315\020\270\060\021\267\006\315\020\056\211\056\044\000"
			/* START + 80  */ "\372\056\017\001\036\153\000\056\017\001\026\022\000\270\001\000"
			/* START + 96  */ "\017\001\360\146\352\303\002\011\000\010\000\000\000\000\000\000"
			/* START + 112 */ "\000\000\000\377\377\000\000\000\232\317\000\377\377\000\000\000"
			/* START + 128 */ "\222\317\000"
			/* START + 131 */ "  1234567890-=  qwertyuiop[]\n asdfghjkl;    zxcvbnm,.           " /* Keyboard layout */
			/* START + 195 */ "\146\270\020\000\216\330\216\320\274\360\001\011\000"
			/* START + 208 */ ;

		/* We use putchar, because huge string contents zeros */
		for(int i = 0; i != 16 + 208; ++i){
			putchar(huge_string[i]);
		}

		/* We write our code to stdout */
		for(int i = 0; i != 3888; ++i){
			putchar(*(const char *)((int)main + (int)i));
		}
	}

	return 0;
}

/* Renders window `task' and all next windows recursively */
void render_task(unsigned char *buffer, struct task_t *task){
	if(task != 0){
		/* We render front task last */
		render_task(buffer, task->next);

		render_rectangle(buffer + task->begin, 0, 0, task->size / SCREEN_WIDTH + 1, task->size % SCREEN_WIDTH,     BLACK);
		render_rectangle(buffer + task->begin, 1, 1, task->size / SCREEN_WIDTH - 1, task->size % SCREEN_WIDTH - 2, LIGHT_GRAY);
		render_rectangle(buffer + task->begin, 2, 2, task->size / SCREEN_WIDTH - 2, task->size % SCREEN_WIDTH - 3, DARK_GRAY);
		render_rectangle(buffer + task->begin, 2, 2, task->size / SCREEN_WIDTH - 3, task->size % SCREEN_WIDTH - 4, GRAY);
		render_rectangle(buffer + task->begin, 4, 4, TITLE_HEIGHT,                  task->size % SCREEN_WIDTH - 8, BLUE);

		for(int i = 0; task->cmdline[i] != 0; ++i){
			render_symbol(buffer + task->begin + 5 * SCREEN_WIDTH + 6 + i * SYMBOL_WIDTH, task->cmdline[i], WHITE);
		}

		/* Render window content */
		(*task->handler)(task, msg_render, (int)buffer);
	}
}

void draw_all(int mouse){
	unsigned char *buffer = (unsigned char *)0x1000000;

	for(int i = 0; i != SCREEN_HEIGHT * SCREEN_WIDTH; ++i){
		buffer[i] = CYAN;
	}

	render_task(buffer, head_task);
	render_symbol(buffer + mouse - (SYMBOL_HEIGHT / 2 * SCREEN_WIDTH + SYMBOL_WIDTH / 2), 'X', BLACK);

	for(int i = 0; i != SCREEN_HEIGHT * SCREEN_WIDTH; ++i){
		/* Now we write to real video memory */
		*(*(unsigned char *const *)(0x11028) + i) = buffer[i];
	}
}

int syscall(int arg, enum syscallnum_t syscallnum){
	switch(syscallnum){
		case SYS_malloc:
			{
				void *result = memory_end;
				memory_end = (void *)((char *)memory_end + arg);
				return (int)result;
			}
		case SYS_file_size:
			{
				const char *fd = (const char *)arg;
				int result = 0; /* We do `result = 0' to avoid compiler warnings */
				for(int i = 124; i != 135; ++i){
					result = (result << 3) | (fd[i] & 7);
				}
				return result;
			}
		case SYS_create_process:
			{
				const char *fd = (const char *)syscall(arg, SYS_open);

				if(*fd == 0){
					/* File not found */
					return 0;
				}

				const char *cmdline = (const char *)arg;

				struct task_t *task = (struct task_t *)syscall(sizeof(struct task_t), SYS_malloc);
				task->next = head_task;
				head_task = task;

				task->cmdline = task->real_cmdline;
				for(int j = 0; j != 100; j++){
					task->real_cmdline[j] = cmdline[j];
				}

				/* Parsing ELF */
				const char *data = fd + TAR_DATA_OFFSET;
				int a = *(const int *)(data + 32);
				while(*(const int *)(data + a + 12) != *(const int *)(data + 24)){
					a += 40;
				}
				task->handler = (msghandler_t)(data + *(const int *)(data + a + 16));
				(*task->handler)(task, msg_init, 0);
				return 0; /* Return value of create_process is ignored */
			}
		case SYS_open:
			{
				/* If file not found, we will return pointer to the end of tar and *fd will be equal to 0 */
				/* Bootloader puts pointer of initramfs to START + 24 */
				char *fd = *(char *const *)(START + 24);
				const char *file_name = (const char *)arg;
				for(;;){
					/* Cheching file name */
					for(int i = 0;; ++i){
						if(fd[i] == 0){
							return (int)fd;
						}
						if(fd[i] != file_name[i])break;
					}

					fd += (syscall((int)fd, SYS_file_size) + 1023) / 512 * 512;
				}
			}
	}

	/* Not reached. I added this `return' to avoid compiler warnings */
	return 0;
}
