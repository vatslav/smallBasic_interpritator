/* ����������� BASIC-������������ */
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
    printf("����������� ������: run <filename>\n");
    exit(1);
  }

  /* ��������� ������ ��� ��������� */
  if (!(p_buf=(char *) malloc(PROG_SIZE)))
  {
    printf("������ ��� ��������� ������ ");
    exit(1);
  }

  /* �������� ��������� ��� ���������� */
  if(!load_program(p_buf,argv[1])) exit(1);
  if(setjmp(e_buf)) exit(1); /* ������������� ������
 ����������� ��������� */
  prog = p_buf;
  scan_labels(); /* ����� ����� � ��������� */
  ftos = 0; /* ������������� ������ ����� FOR */
  gtos = 0; /* ������������� ������ ����� GOSUB */
  do
  {
    token_type = get_token();
    /* �������� �� �������� ������������ */
    if(token_type==VARIABLE)
    {
      putback(); /* ������� ���. ������� �� ������� ����� */
      assigment(); /* ������ ���� �������� ������������ */
    }
    else /* ��� ������� */
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
