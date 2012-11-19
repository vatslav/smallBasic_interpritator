#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <string.h>

///Типы лексем (token_type)

   #define DELIMITER  1 //Разделитель: . , : ; ( ) [ ] \r \0
   #define VARIABLE   2 //Переменная
   #define CONSTANT   3 //Константа
   #define OPERATOR   4 //Оператор: := + - * / = < > <= >= <> if thrn else for to downto read write writeln begin end
   #define SYSTEM     5 //Служебное слово: program var const integer real array of true false
   #define QUOTE      6 //Кавычки:
   #define INTEGER    7 //Целое число: 15, -3
   #define REAL       8 //Вещественное число: 35.35, -13.9
   #define ERROR      9 //Ошибка

///Внутренне представление лексем (tok)
///Если tok=0, то представления нет

   ///Служебные слова

   #define PROGRAMM   10
   #define VAR        11
   #define CONST      12
   #define ARRAY      13
   #define OF         14
   #define BEGIN      15
   #define END        16
   #define TRUE       1
   #define FALSE      -1

   ///Операторы

   #define ASSIGNMENT 19 //Оператор присваивания :=
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
   #define WRITELN    30

   ///Оставшиеся разделители

   #define EOL        17 //Конец строки: \n
   #define FINISHED   18 //Конец файла (кода программы): \0

struct commands
{
    char command[20];
    char tok;
};

//Служебные слова
struct commands SYSTEM_table[] =
{
	{ "program", PROGRAMM },
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
	{ "writeln", WRITELN },
	{ "begin", BEGIN },
	{ "end", END },
	{ "", 0 }//просто признак конца массива
};

char token[80];
char token_type, tok;

char *prog; /* содержит выражение для анализа */
//jmp_buf e_buf; /* содержит среду для longjmp() */

/* Возвращает 1, если "с" пробел или табуляция */
bool iswhite(char c)
{
  if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
     return 1;
  else
     return 0;
}

/* Возвращает "истину", если "c" разделитель */
bool isdelim(char c)
{
	if(strchr(".,:;()[]+-*/<>=\r\n\t ", c) || c == 0)
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
char look_up(char *s, char *token_type)
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
        *token_type = SYSTEM; //Тип "СЛЕЖЕБНОЕ СЛОВО"
        return SYSTEM_table[i].tok;
    }

  //Поиск лексемы среди операторов
  for( i = 0; *OPERATORS_table[i].command; i++ )
    if( !strcmp( OPERATORS_table[i].command, s ) )
    {
        *token_type = OPERATOR; //Тип "ОПЕРАТОР"
        return OPERATORS_table[i].tok;
    }
  *token_type = 0;
  return 0; /* нераспознанная команда */
}


char * typeToName(int param)
{
  switch (param)
  {
    case 1:return "DELIMITER";
    case 2:return "VARIABLE";
    case 3:return "CONSTANT";
    case 4:return "OPERATOR";
    case 5:return "SYSTEM";
    case 6:return "QUOTE";
    case 7:return "INTEGER";
    case 8:return "REAL";
    case 9:return "ERROR";
    case 10:return "PROGRAMM";
    case 11:return "VAR";
    case 12:return "CONST";
    case 13:return "ARRAY";
    case 14:return "OF";
    case 15:return "BEGIN";
    case 16:return "END";
    case 17:return "EOL";
    case 18:return "FIISHED";
    case 19:return "ASSIGNMENT";
    case 100:return "TRUE";
    case -1:return "FALSE";
    case 20:return "IF";
    case 21:return "THEN";
    case 22:return "ELSE";
    case 23:return "WHILE";
    case 24:return "DO";
    case 25:return "FOR";
    case 26:return "TO";
    case 27:return "DOWNTO";
    case 28:return "READ";
    case 29:return "WRITE";
    case 30:return "WRITELN";
  }
  return "without representation";
}



#endif // GLOBALS_H_INCLUDED
