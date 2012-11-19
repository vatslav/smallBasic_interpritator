#ifndef ASSIGNMENT_H_INCLUDED
#define ASSIGNMENT_H_INCLUDED

//#include "get_token.h"

//  В языке BASIC основной формой оператора присваивания является следующая:
///                <имя переменной> = <выражение>
//  Функция assignment() поддерживает этот тип присваивания.
/* Присвоить значение переменной */

    void assignment()
    {
      int var, value;
      /* получить имя переменной */
      get_token();
      if(!isalpha(*token))
      {
          serror(4); /* это не переменная */
          return;
      }

      /* поиск индекса переменной в массиве */
      var = toupper(*token)-'A';

      /* считать символ равенства*/
      get_token();
      if(*token!='=')
      {
        serror(3);
        return;
      }

      /* считать присваиваемое переменной значение */
      get_exp(&value);

      /* присвоить значение*/
      variables[var] = value;
    }


#endif // ASSIGNMENT_H_INCLUDED
