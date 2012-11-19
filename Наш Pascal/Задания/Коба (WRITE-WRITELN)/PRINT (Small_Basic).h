#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

//#include "Globals.h"
#include "stdlib.h"
#include "stdio.h"


//�������� ����� ������� PRINT ����� SMALL BASIC:
///         PRINT <������ ����������>
//��� <������ ����������> ������������ ����� �������� ����������, ����������� � �������
// � ���������� �������� ��� ������ � �������.

    /* ���������� ������ ��������� PRINT */
    void print()
    {
      int answer;
      int len=0, spaces;
      char last_delim;
      do
      {
        get_token(); /* �������� ��������� ������� ������ */
        if (tok == EOL || tok == FINISHED) break;
        if (token_type == QUOTE) /* ��� ������ */
        {
            printf(token);
            len += strlen(token);
            get_token();
        }
        else  /* ��� ��������� */
        {
           putback();
           get_exp(&answer);
           get_token();
           len+= printf("%d", answer);
        }
        last_delim = *token;
        if (*token == ';')
        {       /* ���������� ����� �������� ��� �������� � ��������� ��������� */
            spaces = 8-(len % 8);
            len += spaces; /* �������� �� ���� ��������� */
            while (spaces)
            {
                printf(" ");
                spaces--;
            }
        }
        else if (*token == ','); /* ������ �� ������ */
        else if (tok != EOL && tok != FINISHED) serror(0);
     }
     while (*token == ';' || *token == ',');

     if (tok == EOL || tok == FINISHED)
     {
           if (last_delim != ';' && last_delim != ',')
                printf("\n");
           else serror(0); /* ����������� ����������� */
     }


#endif // PRINT_H_INCLUDED
