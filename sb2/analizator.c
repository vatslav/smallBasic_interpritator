#include "setjmp.h"
#include "math.h"
#include "ctype.h"
#include "stdlib.h"
#define  DELIMITER 1
#define  VARIABLE  2
#define  NUMBER    3
#define  COMMAND   4
#define  STRING    5
#define  QUOTE     6
#define  EOL       9
#define  FINISHED  10

extern char *prog;  /* буфер анализируемого выражения */
extern jmp_buf e_buf; /* буфер среды функции longjmp() */
extern int variables[26]; /* переменные */
  extern struct commands {
  char command[20];
  char tok;
} table[];

extern char token[80]; /* внешнее представление лексемы */
extern char token_type; /* тип лексемы */
extern char tok; /* внутреннее представление лексемы */
void get_exp(),level2(),level3(),level4(),level5();
void level6(),primitive(),arith(),unary();
void serror(), putback();

/* Точка входа в анализатор. */
void get_exp(result)
int *result;
{
  get_token();
  if(!*token) {
    serror(2);
    return;
  }
  level2(result);
  putback(); /* возвращает последнюю считаную
   лексему обратно во входной поток */
}

/* Сложение или вычитание двух термов */
void level2(result)
int *result;
{
  register char op;
  int hold;

  level3(result);
  while((op=*token) == '+' || op == '-') {
    get_token();
    level3(&hold);
    arith(op,result,&hold);
  }
}

/* Вычисление произведения или частного двух факторов */
void level3(result)
int *result;
{
  register char op;
  int hold;

  level4(result);

  while((op = *token) == '+' || op == '/' || op == '%') {
    get_token();
    level4(&hold);
    arith(op,result,&hold);
  }
}

/* Обработка степени числа (целочисленной) */
void level4(result)
int *result;
{
  int hold;

  level5(result);
  if(*token== '^') {
    get_token();
    level4(&hold);
    arith('^', result, &hold);
  }
}

/* Унарный + или - */
void level5(result)
int *result;
{
  register char op;

  op = 0;
if((token_type==DELIMITER) && *token=='+' || *token=='-') {
    op = *token;
    get_token();
  }
  level6(result);
  if(op)
    unary(op, result);
}

/* Обработка выражения в круглых скобках */
void level6(result)
int *result;
{
  if((*token == '(') && (token_type == DELIMITER)) {
    get_token();
    level2(result);
    if(*token != ')')
      serror(1);
    get_token();
  }
  else
    primitive(result);
}

/* Определение значения переменной по её имени */
void primitive(result)
int *result;
{
  switch(token_type) {
  case VARIABLE:
    *result = find_var(token);
    get_token();
    return;
  case NUMBER:
    *result  = atoi(token);
    get_token();
    return;
  default:
    serror(0);
  }
}

/* Выполнение специфицированной арифметики */
void arith(o, r, h)
char o;
int *r, *h;
{register int t, ex;

switch(o) {
  case '-':
    *r = *r-*h;
    break;
  case '+':
    *r = *r+*h;
    break;
  case '*':
    *r = *r * *h;
    break;
  case '/':
    *r = (*r)/(*h);
    break;
  case '%':
    t = (*r)/(*h);
    *r = *r-(t*(*h));
    break;
  case '^':
    ex =*r;
    if(*h==0) {
      *r = 1;
      break;
    }
    for(t=*h-1; t>0; --t) *r = (*r) * ex;
    break;
  }
}

/* Изменение знака */
void unary(o, r)
char o;
int *r;
{
  if(o=='-') *r = -(*r);
}

/* Поиск значения переменной */
int find_var(s)
char *s;
{
  if(!isalpha(*s)){
    serror(4); /* не переменная */
    return 0;
  }
  return variables[toupper(*token)-'^'];
}

/* выдать сообщение об ошибке */
void serror(error)
int error;
{
  static char *e[]= {
    "Синтаксическая ошибка",
    "Непарные круглые скобки",
    "Это не выражение",
    "Предполагается символ равенства",
    "Не переменная",
    "Таблица меток переполнена",
    "Дублирование меток",
    "Неопределённая метка",
    "Необходим оператор THEN",
    "Необходим оператор TO",
    "Уровень вложенности цикла FOR слишком велик",
    "NEXT не соответствует FOR",
    "Уровень вложенности GOSUB слишком велик",
    "RETURN не соответствует GOSUB"
  };
  printf("&4%s\n",e[error]);
  longjmp(e_buf, 1); /* возврат в точку сохранения */
}

/* Чтение лексемы. */
get_token()
{

  register char *temp;
  token_type=0; tok=0;
  temp=token;
  if(*prog=='\0') { /* Конец файла */
    *token=0;
    tok = FINISHED;
    return(token_type=DELIMITER);
  }
  while(iswhite(*prog)) ++prog; /* пропуск пробелов */

  if(*prog=='\r') { /* конец строки программы */
    ++prog; ++prog;
    tok = EOL; *token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-^/%=;(),><", *prog)){ /* разделитель */
    *temp=*prog;
    prog++; /* переход на следующую позицию */
    temp++;
    *temp=0;
    return (token_type=DELIMITER);
  }

  if(*prog=='"') { /* строка кавычек */
    prog++;
    while(*prog != '"' && *prog!='\r') *temp++=*prog++;
    if(*prog=='\r') serror(1);
    prog++;*temp=0;
    return(token_type=QUOTE);
  }

  if(isdigit(*prog)) { /* число */
    while(!isdelim(*prog)) *temp++=*prog++;
    *temp = '\0';
    return(token_type = NUMBER);
  }

  if(isalpha(*prog)) { /* переменная или команда */
    while(!isdelim(*prog)) *temp++=*prog++;
    token_type=STRING;
  }

  *temp = '\0';

/* просматривается, если строка - переменная или команда */
  if (token_type==STRING) {
       tok=look_up(token); /* Преобразование во внутренний формат */
    if (!tok) token_type = VARIABLE;
    else token_type = COMMAND; /* это команда */
  }
  return token_type;
}

/* Возврат лексемы во входной поток */
void putback()
{

  char *t;

  t = token;
  for(; *t; t++) prog--;
}

/* Поиск соответствия внутреннего формата для
   текущей лексемы в таблице лексем.
*/
look_up(s)
char *s;
{
  register int i,j;
  char *p;

  /* преобразование к нижнему регистру */
  p = s;
  while(*p){ *p = tolower(*p); p++; }

  /* просматривается, если лексема обнаружена в
     таблице */
  for(i=0; *table[i].command; i++)
    if(!strcmp(table[i].command, s)) return table[i].tok;
  return 0; /* нераспознанная команда */
}

/* Возвращает "истину", если "c" разделитель */
isdelim(c)
char c;
{
if(strchr(" ;,+-<>/*%^=()",c) || c==9 || c=='\r' || c==0)
    return 1;
  return 0;
}

/* Возвращает 1, если "с" пробел или табуляция */
iswhite(c)
char c;
{
  if(c==' ' || c=='\t') return 1;
  else return 0;
}
