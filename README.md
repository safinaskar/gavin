![screenshot.png](https://github.com/safinaskar/gavin/raw/master/screenshot.png)

Introduction
------------

In 2004 Gavin Barraclough created multitasking operating system with GUI, filesystem and mouse support.
It is written in C, it has only one source file and this file has size 3.5 kilobytes (his OS has no name, so I will call it "Gavin")!
Then Gavin sent OS to IOCCC (the International Obfuscated C Code Contest) and won. Of course, code is very obfuscated, for example:

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


-----
Copyright (C) 2004, Landon Curt Noll, Simon Cooper, Peter Seebach
and Leonid A. Broukhis. All Rights Reserved. Permission for personal,
educational or non-profit use is granted provided this copyright and
notice are included in its entirety and remains unaltered.  All other
uses must receive prior permission from the contest judges.
