#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

//#include "Globals.h"
#include "stdlib.h"
#include "stdio.h"


//Основная форма команды PRINT языка SMALL BASIC:
///         PRINT <список аргументов>
//где <список аргументов> представляет собой перечень переменных, заключённых в кавычки
// и разделённых запятыми или точкой с запятой.

    /* Простейшая версия оператора PRINT */
    void print()
    {
      int answer;
      int len=0, spaces;
      char last_delim;
      do
      {
        get_token(); /* получить следующий элемент списка */
        if (tok == EOL || tok == FINISHED) break;
        if (token_type == QUOTE) /* это строка */
        {
            printf(token);
            len += strlen(token);
            get_token();
        }
        else  /* это выражение */
        {
           putback();
           get_exp(&answer);
           get_token();
           len+= printf("%d", answer);
        }
        last_delim = *token;
        if (*token == ';')
        {       /* Вычисление числа пробелов при переходе к следующей табуляции */
            spaces = 8-(len % 8);
            len += spaces; /* смещение на одну табуляцию */
            while (spaces)
            {
                printf(" ");
                spaces--;
            }
        }
        else if (*token == ','); /* ничего не делать */
        else if (tok != EOL && tok != FINISHED) serror(0);
     }
     while (*token == ';' || *token == ',');

     if (tok == EOL || tok == FINISHED)
     {
           if (last_delim != ';' && last_delim != ',')
                printf("\n");
           else serror(0); /* Отсутствует разделитель */
     }


#endif // PRINT_H_INCLUDED
