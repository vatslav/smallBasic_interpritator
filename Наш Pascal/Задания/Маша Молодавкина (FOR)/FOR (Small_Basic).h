#ifndef FOR_H_INCLUDED
#define FOR_H_INCLUDED

//#include "Globals.h"
    //  ќсновной формат оператора цикла FOR следующий:
/**
        FOR <им€ управл€ющей переменной> = <нач. значение> TO <кон. значение>
        ..............................
        последовательность операторов
        ..............................
        NEXT

*/

//ƒл€ реализации оператора цикла FOR стек должен иметь следующую структуру:
/**
  struct for_stack
  {
    int var; // счетчик цикла
    int target; // конечное значение
    char *loc;
  }
  fstack[FOR_NEST]; // стек дл€ цикла FOR/NEXT
  int ftos; // индекс начала стека FOR
*/
        /*
        «начение макроса FOR_NEST ограничивает уровень вложенности цикла.
        (ѕо умолчанию допускаетс€ 25 уровней вложенности).
        ѕеременна€ ftos всегда имеет значение индекса начала стека.
        ƒл€ обработки стека вам понадоб€тс€ две функции fpush() и fpop(), которые приведены ниже.
        */
    /* ѕоместить элемент в стек */
    void fpush(i)
    struct for_stack i;
    {
      if (ftos > FOR_NEST)
        serror(10);
      fstack[ftos]=i;
      ftos++;
    }

    struct for_stack fpop()
    {
      ftos--;
      if (ftos < 0) serror(11);
      return(fstack[ftos]);
    }

    /* ѕростейша€ реализаци€ оператора цикла FOR */
    void exec_for()
    {
      struct for_stack i;
      int value;
      get_token(); /* чтение управл€ющей переменной */
      if (!isalpha(*token))
      {
        serror(4);
        return;
      }
      i.var=toupper(*token) - 'A'; /* сохраним индекс */
      get_token(); /* чтение символа равенства */
      if (*token != '=') {
        serror(3);
        return;
      }
      get_exp(&value); /* получить начальное значение */
      variables[i.var] = value;
      get_token();
      if (tok != TO) serror(9); /* чтение и анализ TO */
        get_exp(&i.target); /* получить конечное значение */
      /* ≈сли цикл выполн€етс€ последний раз, поместить информацию в стек*/
      if (value >= variables[i.var]) {
        i.loc = prog;
        fpush(i);
      }
      else { /* пропустить весь цикл целиком */
      while(tok != NEXT) get_token();
      }
    }

      /* ¬ыполнение оператора NEXT */
      void next()
      {
        struct for_stack i;
        i = fpop(); /* чтение информации о цикле */
        variables[i.var]++; /* увеличение переменной цикла */
        if (variables[i.var] > i.target) return; /* конец */
        fpush(i); /* в противном случае запомнить информацию */
        prog = i.loc; /* цикл */
      }

#endif // FOR_H_INCLUDED
