#ifndef IF_H_INCLUDED
#define IF_H_INCLUDED

//#include "Globals.h"
        //ќсновной формат оператора:
/**
        IF <выражение> <оператор> <выражение> THEN <оператор>
*/

 // ќператор, сто€щий после THEN, выполн€етс€ только в том случае, если значение сравнени€ €вл€етс€ истинным.
 // ‘ункци€ exec_if(), приведЄнна€ ниже, обеспечивает выполнение этой формы оператора IF.*/

    /* ѕростейша€ реализаци€ оператора IF  */

    void exec_if()
    {
      int x , y, cond;
      char op;
      get_exp(&x); /* получить левое выражение */
      get_token(); /* получить оператор  */
      if (!strchr("=<>", *token))
      {
        serror(0);      /* недопустимый оператор  */
        return;
      }
      op=*token;
      get_exp(&y);  /* получить правое выражение */
     /* ќпределение результата */
      cond=0;
      switch(op)
      {
        case '=':
          if(x==y) cond=1;
        break;
        case '<':
          if(x<y) cond=1;
        break;
        case '>':
          if(x>y) cond=1;
        break;
      }
      if(cond)
      {             /* если истина, то поиск нужного IF  */
        get_token();
        if(tok!=THEN)
        {
          serror(8);
          return;
        }  /* ¬ противном случае программа продолжаетс€ со
            следующей строки  */
      }
      else find_eol(); /* поиск строки продолжени€ программы */
    }



#endif // IF_H_INCLUDED
