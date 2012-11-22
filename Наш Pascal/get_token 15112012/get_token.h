#ifndef GET_TOKEN_H_INCLUDED
#define GET_TOKEN_H_INCLUDED

#include "Globals.h"
#include <stdio.h>

/* Чтение лексемы. */
char get_token()
{
  register char *temp;
  token_type = 0; tok = 0;
  temp = token;

  while(iswhite(*prog)) ++prog; /* Пропуск пробелов */

  if (*prog == '{') /* КОММЕНТАРИИ */
  { // У комментариев нет внутреннего представления и типа. Они просто пропускаются
    int cnt = 1;
    while (cnt != 0)
    {
      prog++;
      if (*prog == '{')
         cnt++;
      else if (*prog == '}')
         cnt--;
      else if (*prog == '\0')
      {
          printf("Error: Отсутствует парный символ к '{'");
          break;
      }
    }
    if ( *prog ) prog++;
	while(iswhite(*prog)) ++prog; /* Пропуск пробелов */
  }

  if(*prog == '\0') /* КОНЕЦ ФАЙЛА */
  {
    *token=0;
    tok = 0;
    return (token_type=FINISHED); //
  }

  if (strchr(".,:;()[]", *prog)) /* РАЗДЕЛИТЕЛЬ */
  {
    bool assign = false; //Признак присваивания
    *temp = *prog;

    if (*temp == ':')  //Если наткнулись на ":",
       assign = true;  //  возможно это присваивание

    prog++; // Переход на следующую позицию
    temp++;

    if (*prog == '=' and assign == true)  //Если встретили "=" после ":",
    {                                    //  значит это точно присваивание
        *temp = *prog;
        prog++;
        temp++;
        *temp = 0;
        tok = ASSIGNMENT;  //Внутреннее представление присваивания.
        return ( token_type = OPERATOR ); // Тип "ОПЕРАТОР"
    }

    *temp = 0;
    return (token_type = DELIMITER);  // Тип "РАЗДЕЛИТЕЛЬ"
  }

  if (strchr("+-*/=<>", *prog)) /* ОПЕРАТОР */
  {
    bool two_symb = false; //Признак двухсимвольного оператора "<=", ">=" или "<>"
    *temp = *prog;

    if(*prog == '<' or *prog == '>') //Если наткнулись на "<" или ">",
       two_symb = true;              //  возможно это двухсимвольный оператор "<=", ">=" или "<>"

    prog++; // Переход на следующую позицию
	temp++;

    if (two_symb == true)
        if( (*temp == '=') or (*(temp-1) == '<' and *temp == '>' ) )
        {         //Если встретили "=" после "<" или ">" ("<=",">=") или
           *temp = *prog;  // ">" после "<" ("<>"), значит оператор двухсимвольный
           prog++;
           temp++;
        }

    *temp = 0;
    return (token_type = OPERATOR);  // Тип "ОПЕРАТОР"
  }

  if(isdigit(*prog) == true) /* ЧИСЛО */
  {
    while (isdigit(*prog) == true) //Пока встречаем числа, считываем
       *temp++=*prog++;

    if(*prog == '.') //Если наткнулись на точку, то это либо вещественное число "REAL",
    {                //либо диапазон индексов внутри массива
      if (*(prog+1) == '.')//Если 2 точки подряд, значит
      {                    //это диапазон индексов внутри массива "A:array [3..12]"
        *temp = 0;
        return (token_type = INTEGER); //Тип "ЦЕЛОЕ ЧИСЛО"
      }

      *temp++ = '.';
      prog++;
      while(isdigit(*prog) == true) //Пока встречаем числа, считываем
         *temp++ = *prog++;

      if(*(prog-1) == '.')   // Если цикл ни разу не выполнился,
      {                      // значит некорректное выражение типа "17.a"
        *token = '\0';
        printf("ERROR! Expected REAL number\n");
        return (token_type = ERROR); //Тип "ОШИБКА"
      }

      if (isdelim(*prog) == false)  //Если не встретили разделителя или оператора,
      {                              // значит некорректное выражение типа "17.23a"
        *token = 0;
        printf("ERROR! Expected REAL number\n");
        return (token_type = ERROR);  //Тип "ОШИБКА"
      }

      *temp = '\0';
      return(token_type = REAL); //Тип "ВЕЩЕСТВЕННОЕ ЧИСЛО"
    }
    else if (isdelim(*prog) == false) //Если не встретили разделителя или оператора,
    {                                  // значит некорректное выражение типа "17a"
      *token = 0;
      printf("ERROR! Expected INTEGER number\n");
      return (token_type = ERROR);  //Тип "ОШИБКА"
    }

	*temp = '\0';
	return(token_type = INTEGER); //Тип "ЦЕЛОЕ ЧИСЛО"
  }

  if(isalpha(*prog) or *prog == '_') /* Имя "ИДЕНТИФИКАТОРА", "ОПЕРАТОРА" или "СЛУЖЕБНОЕ СЛОВО"*/
  {
    //Правила построения имен идентификаторов:
    //  1) Имя начинается с латинской буквы или символа '_'.
    //  2) Остальными символами могут быть латинские буквы, цифры и '_': _A2_B, A2.
    //Имена операторов и служ. слова состоят только из латинских букв: IF, For и т.д.

    //Cчитываем имя пока встречаем буквы, цифры или '_'
    int ptr=0;
    while(is_good_name (*prog))
    {
      *temp++ = *prog++;
      ptr++;
    }

    //Eсли встретили неподходящий символ вместо разделителя, выдаем ошибку
    if (!isdelim (*prog)) //
    {
        *token = 0;
        printf("ERROR! Uncorrect IDENTIFIER or OPERATOR name\n");
        return ( token_type = ERROR ); //Тип "ОШИБКА"
    }
    if (ptr>8)
    {
        *token = 0;
        printf("ERROR! name of variable > 8\n");
        return ( token_type = ERROR ); //Тип "ОШИБКА"
    }
    *temp = 0;

    char type; //Тип лексемы
    tok = look_up(token, &type); // Преобразование во внутренний формат.

    if ( type != 0 ) //Если нашли лексему среди "ОПЕРАТОРОВ" или "СЛУЖЕБНЫХ СЛОВ", возвращаем тип лексемы
        return ( token_type = type );
    else //Иначе имеем дело с "ИДЕНТИФИКАТОРОМ": переменной или константой
    {
      return ( token_type = VARIABLE );
        ///Надо обдумать
    }
  }
  ///по идее надо бы сообщить об ошибки, что неверное имя переменной (оператора
  else
    {///это наше--
      *token = 0;
      printf("ERROR! Uncorrect IDENTIFIER or OPERATOR name\n");
      return ( token_type = ERROR ); //Тип "ОШИБКА"
    }
}



#endif // GET_TOKEN_H_INCLUDED
