/* Минимальный BASIC-интепретатор */
#include "stdio.h"
#include "setjmp.h"
#include "math.h"
#include "ctype.h"
#include "stdlib.h"
#include <locale.h>
#include "Functions.h"
#include <conio.h>

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "Russian");
  char in[80];
  int answer;
  char *p_buf = "Test.txseg";
  char *t;

  if(argc!=2)
  {
    printf("Используйте формат: run <filename>\n");
    exit(1);
  }

  /* Выделение памяти для программы */
  if (!(p_buf=(char *) malloc(PROG_SIZE)))
  {
    printf("Ошибка при выделении памяти ");
    exit(1);
  }

  /* Загрузка программы для выполнения */
  if(!load_program(p_buf,argv[1])) exit(1);
  if(setjmp(e_buf)) exit(1); /* инициализация буфера
 нелокальных переходов */
  prog = p_buf;
  scan_labels(); /* поиск метки в программе */
  ftos = 0; /* инициализация индеса стека FOR */
  gtos = 0; /* инициализация индеса стека GOSUB */
  do
  {
    token_type = get_token();
    /* проверка на оператор присваивания */
    if(token_type==VARIABLE)
    {
      putback(); /* возврат пер. обратно во входной поток */
      assigment(); /* должен быть оператор присваивания */
    }
    else /* это команда */
      switch(tok)
      {
      case PRINT:
        print();
        break;
      case GOTO:
        exec_if();
        break;
      case FOR:
        exec_for();
        break;
      case NEXT:
        next();
        break;
      case INPUT:
        input();
        break;
      case GOSUB:
        gosub();
        break;
      case RETURN:
        greturn();
        break;
      case END:
        exit(0);
      }
  }
  while (tok != FINISHED);
  getch ();
}
