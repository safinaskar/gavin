#include "common.h"

int
_start (struct task_t *my_task, int q, int d)
{
	int *line = (int *)my_task->data; /* Current line in vi TODO 20 */
	int *column = (int *)(my_task->data + 4); /* Current cursor position in sh TODO 24 */
	char **content = (char **)(my_task->data + 8); /* Pointer to content of window (as string) TODO 28 */
  int i = 0, j = 0, h, n, p = 393728, s = 26739, C, D;
  if (!--q)
    {
      D = ((int (*)(int my_task, int q, int d)) (*((int *) (0x90200)))) (C =
								   ((int (*)
								     (int my_task,
								      int q,
								      int
								      d)) (*((int *) (0x90200)))) (my_task->cmdline + 3, 4, 0), 2, 0);
      my_task->begin = *line = i;
      *column = 1;
      *content = my_task;
      my_task->size = 442890;
      *content = d =
	((int (*)(int my_task, int q, int d)) (*((int *) (0x90200)))) (s =
								 D * 8 + 1664,
								 1, 0);
      for (p = 0; j < s; j++, p++)
	*((signed char *) (d + j)) = i == D | j < p ? p--, 0 : (n =
								*((signed char
								   *) (C +
								       512 +
								       i++)))
	  < ' ' ? p |= n * 56 - 497, 0 : n;
    }

  if (!--q)
    {
  n = *content;
  	*(char*)n++ = 'P';
  	*(char*)n++ = 'r';
  	*(char*)n++ = 'e';
  	*(char*)n++ = 's';
  	*(char*)n++ = 's';
  	*(char*)n++ = ' ';
  	*(char*)n++ = 'a';
  	*(char*)n++ = 'n';
  	*(char*)n++ = 'y';
  	*(char*)n++ = ' ';
  	*(char*)n++ = 'k';
  	*(char*)n++ = 'e';
  	*(char*)n++ = 'y';
      for (D = 0; D < 786432; D++)
	*((signed char *) (d + D)) =
	  (D >= my_task->begin + 26628
	   && D < (my_task->begin + 26628 + 412162)
	   && (D - (my_task->begin + 26628) & 1023) <
	   (412162 & 1023)) ? (D >= my_task->begin + 27653
			       && D < (my_task->begin + 27653 + 410112)
			       && (D - (my_task->begin + 27653) & 1023) <
			       (410112 & 1023)) ? 31 : 0 : *((signed char
							      *) (d + D));

      for (; j < 12800; j += 8)
	for (h = 0; h >> 14 == 0; h += 129)
	  *((int *) (16 **
		     ((signed char *) (*content + j / 8 +
				       64 * *line)) + h / 1024 +
		     *((int *) (0x90200 +
				36)))) & 128 >> (h & 7) ? *((signed char *) (d
									     +
									     27653
									     +
									     my_task->begin + ' ' * (j & ~511) + j % 512 + (h & 15367))) = 0 : 0;

    }

  if (!--q)
    {
	int i, j, k;
	  D = *content;
	for (; ++*column; ) {
		j = 2;
		i = *column;
		for (; j < i; j++) if (i%j==0) break;
		if (i==j)break;
	}
	j = 0;
	do { for (k = j; k; k--) *(char*)(D + 1536 + k) = *(char*)(D + 1535 + k);
	*(char*)(D + 1536) = i % 10 + '0';
	i /= 10;
	j++;
	} while (i);
	for (i = D; i < D + 1600; i++) *((signed char *) (i)) = *((signed char *) (i + 64));
    }
}

