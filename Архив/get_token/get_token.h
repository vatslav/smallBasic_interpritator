#ifndef GET_TOKEN_H_INCLUDED
#define GET_TOKEN_H_INCLUDED

#include <string.h>
#include <stdio.h>

///типы лексем (группы лексем)
#define DELIMITER  1   // . , : ; ( ) [ ]
#define VARIABLE   2   // Идентификаторы переменных
#define VARIABLE   3   // Идентификаторы констант
#define OPERATOR   5   // := + - * / = < > <= >= <> IF THEN ELSE WHILE DO FOR TO DOWNTO READ WRITE WRITELN BEGIN END
#define SYSTEM     6   // Служебные слова: var const integer real array of
#define QUOTE      7
#define ERROR      9   // Ошибка

#define TRUE       1
#define FALSE      -1
#define VAR        10
#define CONST      11
#define INTEGER    3
#define REAL       4
#define ARRAY      12
#define OF         13
#define SYSTEM     14

#define ASSIGNMENT 8

#define FINISHED   15
#define EOL        16
///биекция внешнего представления на внутрения
#define IF         20
#define THEN       21
#define ELSE       22

#define WHILE      23
#define DO         24

#define FOR        25
#define TO         26
#define DOWNTO     27

#define READ       28

#define WRITE      29

#define BEGIN      30
#define END        31

struct commands
{
    char command[20];
    char tok;
};

//Служебные слова
struct commands SYSTEM_table[] =
{
	{ "var", VAR },
	{ "const", CONST },
    { "integer", INTEGER },
	{ "real", REAL },
	{ "array", ARRAY },
	{ "of", OF },
	{ "true", TRUE },
	{ "false", FALSE },
	{ "", 0 }//просто признак конца массива
};

//Операторы
struct commands OPERATORS_table[] =
{
	{ "if", IF },
	{ "then", THEN },
	{ "else", ELSE },
	{ "while", WHILE },
	{ "do", DO },
	{ "for", FOR },
	{ "to", TO },
	{ "downto", DOWNTO },
	{ ":=", ASSIGNMENT },
	{ "read", READ},
	{ "write", WRITE },
	{ "begin", BEGIN },
	{ "end", END },
	{ "", 0 }//просто признак конца массива
};

char token[80];
char token_type; //Тип лексемы
char tok;  //Внутренне представление лексемы

char *prog; /* содержит выражение для анализа */
//jmp_buf e_buf; /* содержит среду для longjmp() */

/* Возвращает 1, если "с" пробел или табуляция */
bool iswhite(char c)
{
  if(c == ' ' || c == '\t')
     return 1;
  else
     return 0;
}

/* Возвращает "истину", если "c" разделитель */
bool isdelim(char c)
{
	if(strchr(".,:;()[]+-*/<>=", c) || c==9 || c=='\r' || c == 0)
		return 1;
	else
		return 0;
}

/* Возвращает "истину", если "c" является чем-то из { a..z, A..Z, 0..9, "_" } */
bool is_good_name (char c)
{
    return (isalpha(c) or isdigit(c) or c == '_' );
}

/* Поиск соответствия внутреннего формата для
   текущей лексемы в таблицах лексем.
*/
char look_up(char *s, char *type)
{
  register int i;
  char *p;

  /* преобразование к нижнему регистру */
  p = s;
  while(*p) { *p = tolower(*p); p++; }

  //Поиск лексемы среди служебных слов
  for( i = 0; *SYSTEM_table[i].command; i++ )
    if( !strcmp(SYSTEM_table[i].command, s) )
    {
        *type = SYSTEM; //Тип "СЛЕЖЕБНОЕ СЛОВО"
        return SYSTEM_table[i].tok;
    }

  //Поиск лексемы среди операторов
  for( i = 0; *OPERATORS_table[i].command; i++ )
    if( !strcmp( OPERATORS_table[i].command, s ) )
    {
        *type = OPERATOR; //Тип "ОПЕРАТОР"
        return OPERATORS_table[i].tok;
    }
  *type = 0;
  return 0; /* нераспознанная команда */
}

/* Чтение лексемы. */
char get_token()
{
  register char *temp;
  token_type = 0; tok = 0;
  temp = token;

  if(*prog == '\0') /* КОНЕЦ ФАЙЛА */
  {
    *token=0;
    tok = FINISHED;
    return (token_type=DELIMITER); //
  }

  if(*prog == '\r') /* КОНЕЦ СТРОКИ программы */
  {
    ++prog; ++prog;
    tok = EOL; *token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER); // Тип "РАЗДЕЛИТЕЛЬ"
  }

  if (*prog == '{') /* КОММЕНТАРИИ */
  {
    bool close = false; // Признак завершения комментария
    while (close != true)
    {
      prog++;
      //if (*prog == '{')
        // cnt ++;
      if (*prog == '}')
         close = true;
      else if (*prog == '\0')
         break;      // У комментариев нет внутреннего представления и типа. Они просто пропускаются
    }
    if ( *prog ) prog++;
	while(iswhite(*prog)) ++prog; /* Пропуск пробелов */
  }

  if (strchr(".,:;()[]", *prog)) /* РАЗДЕЛИТЕЛЬ */
  {
    bool assign = false; //Признак присваивания
    *temp = *prog;

    if (*temp == ':')  //Если наткнулись на ":",
       assign = true;  //  возможно это присваивание

    prog++; // Переход на следующую позицию
    temp++;

    if (*temp == '=' and assign == true)  //Если встретили "=" после ":",
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

    if(*temp == '<' or *temp == '>') //Если наткнулись на "<" или ">",
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

  if(is_good_name (*prog) == true) /* Имя "ИДЕНТИФИКАТОРА", "ОПЕРАТОРА" или "СЛУЖЕБНОЕ СЛОВО"*/
  {
    //Правила построения имен идентификаторов:
    //  1) Имя начинается с латинской буквы или символа '_'.
    //  2) Остальными символами могут быть латинские буквы, цифры и '_': _A2_B, A2.
    //Имена операторов и служ. слова состоят только из латинских букв: IF, For и т.д.

    //Cчитываем имя пока встречаем буквы, цифры или '_'
    while(isalpha(*prog) or isdigit(*prog) or *prog == '_')
       *temp++ = *prog++;

    //Eсли встретили неподходящий символ вместо разделителя, выдаем ошибку
    if (!isdelim (*prog)) //
    {
        *token = 0;
        printf("ERROR! Uncorrect IDENTIFIER or OPERATOR name\n");
        return ( token_type = ERROR ); //Тип "ОШИБКА"
    }
    *temp = 0;

    char type; //Тип лексемы
    tok = look_up(token, &type); // Преобразование во внутренний формат.

    if ( type != 0 ) //Если нашли лексему среди "ОПЕРАТОРОВ" или "СЛУЖЕБНЫХ СЛОВ", возвращаем тип лексемы
        return ( token_type = type );
    else //Иначе имеем дело с "ИДЕНТИФИКАТОРОМ": переменной или константой
    {
        //Надо обдумать
    }
  }
}


void putback()
{
	char *t;

	t = token;
	for( ; *t; t++ ) prog--;
}

#endif // GET_TOKEN_H_INCLUDED
