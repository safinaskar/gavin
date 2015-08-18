#include "user.h"

void _start(struct task_t *my_task, enum msgnum_t msgnum, int arg)
{
  int *column = (int *)(my_task->data + 4); /* Current cursor position */
  char **content = (char **)(my_task->data + 8); /* Pointer to content of window (as character array) */

  switch (msgnum){
    case msg_init:
      my_task->begin = 0; /* At first window appears in the top left corner */
      my_task->size = SIZE;

      *column = 1;

      *content = (char *)malloc((LINES + 1) * COLUMNS);

      for (int i = 0; i != (LINES + 1) * COLUMNS; ++i){
        (*content)[i] = 0;
      }
      break;
    case msg_render:
      (*content)[(LINES - 1) * COLUMNS] = '>';
      render_content((unsigned char *)arg + my_task->begin, *content);
      break;
    case msg_key:
      if (arg == '\n'){
        for (int i = 0; i < LINES * COLUMNS; ++i){
          (*content)[i] = (*content)[i + COLUMNS];
        }
        *column = 1;
        create_process(*content + (LINES - 2) * COLUMNS + 1);
      }else{
        (*content)[(LINES - 1) * COLUMNS + *column] = arg;
        ++*column;
      }
      break;
  }
}
