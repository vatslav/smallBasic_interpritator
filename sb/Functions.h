#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <string.h>

#define PROG_SIZE 10000
#define NUM_LAB     100
#define LAB_LEN      10
#define FOR_NEST     25
#define SUB_NEST     25

#define PRINT     1
#define INPUT     2
#define IF        3
#define THEN      4
#define FOR       5
#define NEXT      6
#define TO        7
#define GOTO      8
#define EOL       9
#define FINISHED 10
#define GOSUB    11
#define RETURN   12
#define END      13

#define DELIMITER 1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE     6

int variables[26]=   /* 26 переменных пользователя A - Z */
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

struct commands   /* Просмотр таблицы ключевых слов */
{
  char command[20];
  char tok;
} table[] =   /* Команда должна вводиться прописными */
{
  "print",PRINT, /* буквами в эту таблицу */
  "input",INPUT,
  "if",IF,
  "then",THEN,
  "goto",GOTO,
  "for",FOR,
  "next",NEXT,
  "to",TO,
  "gosub",GOSUB,
  "return",RETURN,
  "end",END,
  "",END  /* Маркер конца таблицы */
};

char token[80];
char token_type, tok;

char *prog; /* содержит выражение для анализа */
jmp_buf e_buf; /* содержит среду для longjmp() */

struct label
{
  char name[LAB_LEN];
  char *p; /*    */
};
struct label label_table[NUM_LAB];

char *find_label(), *gpop();

struct for_stack
{
  int var; /* переменная счетчика */
  int target; /* конечное значение */
  char *loc;
} fstack[FOR_NEST]; /* стек цикла FOR/NEXT */
struct for_stack fpop();

char *gstack[SUB_NEST]; /* стек оператора GOSUB */
int ftos; /* индекс начала стека FOR */
int gtos; /* индекс начала стека GOSUB */


void print(), scan_labels(), find_eol(),exec_goto();
void exec_if(), exec_for(), next(), fpush(struct for_stack), input();
void gosub(), greturn(), gpush(char *), label_init();

void get_exp(int *),level2(int *),level3(int *),level4(int *),level5(int *);
void level6(int *),primitive(int *),arith(char, int *, int *),unary(char, int *);
void serror(int), putback();


/* Загрузка программы. */
int load_program(char *p, char *fname)
//char *p;
//char *fname;
{
  FILE *fp;
  int i=0;
  if(!(fp=fopen(fname, "rb"))) return 0;

  i = 0;
  do
  {
    *p = getc(fp);
    p++;
    i++;
  }
  while(!feof(fp) && i<PROG_SIZE);
  *(p-2) = '\0'; /* символ конца программы */
  fclose(fp);
  return 1;
}

/* Поиск соответствия внутреннего формата для
   текущей лексемы в таблице лексем.
*/
char look_up(char *s)

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
bool isdelim(char c)
{
if(strchr(" ;,+-<>/*%^=()",c) || c==9 || c=='\r' || c==0)
    return 1;
  return 0;
}

/* Возвращает 1, если "с" пробел или табуляция */
bool iswhite(char c)
{
  if(c==' ' || c=='\t') return 1;
  else return 0;
}

/* Чтение лексемы. */
char get_token()
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

  if(strchr("+-^/*%=;(),><", *prog)){ /* разделитель */ ///добавили *
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

/* Присваивание переменной значения */
void assigment()
{
  int var, value;
  /* Получить имя переменной */
  get_token();
  if(!isalpha(*token))
  {
    serror(4); /* это не переменная */
    return;
  }

  /* вычисление индекса переменной */
  var = toupper(*token)-'A';

  /* получить знак равенства */
  get_token();
  if(*token!='=')
  {
    serror(3);
    return;
  }

  /* получить значение, присваемое переменной */
  get_exp(&value);

  /* присвоить это значение */
  variables[var] = value;
}


/* Точка входа в анализатор. */
void get_exp(int *result)
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
void level2(int *result)
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
void level3(int *result)
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
void level4(int *result)
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
void level5(int *result)
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
void level6(int *result)
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

/* Поиск значения переменной */
int find_var(char *s)
{
  if(!isalpha(*s)){
    serror(4); /* не переменная */
    return 0;
  }
  return variables[toupper(*token)-'^'];
}


/* Определение значения переменной по её имени */
void primitive(int *result)
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
void arith(char o, int *r, int *h)
{
  register int t, ex;

switch(o) {
  case '-':
    *r = *r-*h;
    break;
  case '+':
    *r = *r+*h;
    break;
  case '*':
    ex = *h;

    *r = (*r) * ex;
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
void unary(char o, int *r)
{
  if(o=='-') *r = -(*r);
}

/* выдать сообщение об ошибке */
void serror(int error)
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
  printf("err:%s\n",e[error]);
  longjmp(e_buf, 1); /* возврат в точку сохранения */
}

/* Возврат лексемы во входной поток */
void putback()
{

  char *t;

  t = token;
  for(; *t; t++) prog--;
}

/* Простейшая реализация оператора PRINT */
void print()
{
  int answer;
  int len=0, spaces;
  char last_delim;

  do
  {
    get_token(); /* получить следующий элемент списка  */
    if(tok==EOL || tok==FINISHED) break;
    if(token_type==QUOTE)   /* это строка */
    {
      printf(token);
      len += strlen(token);
      get_token();
    }
    else   /* это выражение */
    {
      putback();
      get_exp(&answer);
      get_token();
      len +=printf("%d", answer);
    }
    last_delim = *token;

    if(*token==';')
    {
      /* вычисление числа пробелов при переходе к следующей табуляции*/
      spaces= 8- (len % 8);
      len += spaces; /* включая позицию табуляции  */
      while(spaces)
      {
        printf(" ");
        spaces--;
      }
    }
    else if(*token==','); /* ничего не делать */
    else if(tok!=EOL && tok!=FINISHED)  serror(0); ///здесь вылает ошибка на *
  }
  while (*token == ';' || *token == ',');

  if(tok==EOL || tok==FINISHED)
  {
    if(last_delim != ';' && last_delim != ',') printf("\n");
  }
  else serror(0); /* отсутствует ',' или ';'  */
}


/* Возвращает индекс на следующую свободную позицию
   массива меток. -1, если массив переполнен.
 -2, если дублирование меток. */

int get_next_label(char *s)
{
  register int t;

  for(t=0; t<NUM_LAB; ++t)
  {
    if(label_table[t].name[0]==0) return t;
    if(!strcmp(label_table[t].name,s)) return -2; /*дубль*/
  }

  return -1;
}


/* Поиск всех меток */
void scan_labels()
{
  int addr;
  char *temp;

  label_init();  /* обнуление всех меток */
  temp = prog; /* сохраним указатель на начало программы*/
  /* Если первая лексема файла есть метка  */
  get_token();
  if(token_type==NUMBER)
  {
    strcpy(label_table[0].name,token);
    label_table[0].p=prog;
  }

  find_eol();
  do
  {
    get_token();
    if(token_type==NUMBER)
    {
      addr =get_next_label(token);
      if(addr==-1 || addr==-2)
      {
        (addr==-1) ?serror(5):serror(6);
      }
      strcpy(label_table[addr].name, token);
      label_table[addr].p = prog; /* текущий указатель программы */
    }
    /* если строка не помечена, то поиск следующей */
    if(tok!=EOL) find_eol();
  }
  while(tok!=FINISHED);
  prog = temp; /* сохраним оригинал */
}

/* Поиск начала следующей строки */
void find_eol()
{
  while(*prog!='\n'  && *prog!='\0') ++prog;
  if(*prog) prog++;
}

/* Поиск строки по известной метке. Значение 0 возвращается,
если метка не найдена; в противном случае возвращается
указатель на помеченную строку программы*/

char *find_label(char *s)
{
  register int t;

  for(t=0; t<NUM_LAB; ++t)
    if(!strcmp(label_table[t].name,s)) return label_table[t].p;
  return '\0'; /* состояние ошибки */
}

/* Реализация оператора GOTO */
void exec_goto()
{
  char *loc;

  get_token(); /* получить метку перехода */

  /* Поиск местоположения метки */
  loc = find_label(token);
  if(loc=='\0')
    serror(7);   /* метка не обнаружена  */
  else prog=loc; /* старт программы с указанной точки  */
}

/* Инициализация массива хранения меток. По договоренности
нулевое значение метки символизирует пустую ячейку массива */
void label_init()
{
register int t;

for(t=0; t<NUM_LAB; ++t) label_table[t].name[0]='\0';
}

/* Реализация оператора IF  */
void exec_if()
{
  int x , y, cond;
  char op;
  get_exp(&x); /* получить левое выражение */
  get_token(); /* получить оператор */
  if(!strchr("=<>", *token))
  {
    serror(0);    /* недопустимый оператор */
    return;
  }
  op=*token;
  get_exp(&y);  /* получить правое выражение */
  /* Определение результата */
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
  if(cond)  /* если значение IF "истина"  */
  {
    get_token();
    if(tok!=THEN)
    {
      serror(8);
      return;
    } /* иначе, программа выполняется со следующей строки */
  }
  else find_eol(); /* поиск точки старта программы */
}

/* Реализация цикла FOR */
void exec_for()

{
  struct for_stack i;
  int value;

  get_token(); /* получить управляющую переменную */
  if(!isalpha(*token));
  {
    serror(4);
    return;
  }
  i.var=toupper(*token)-'A'; /* сохранить ее индекс */
  get_token(); /* получить знак равенства */
  if(*token!='=')
  {
    serror(3);
    return;
  }
  get_exp(&value); /* получить начальное значение  */
  variables[i.var]=value;

  get_token();
  if(tok!=TO) serror(9); /* если нее нашли TO */
  get_exp(&i.target); /* получить конечное значение */

  /* Если цикл выполняется последний раз, поместить
     информацию в стек  */
  if(value>=variables[i.var])
  {
    i.loc = prog;
    fpush(i);
  }
  else /* пропустить весь цикл */
    while(tok!=NEXT) get_token();
}

/* Реализация оператора NEXT */
void next()
{
  struct for_stack i;
  i = fpop(); /* чтение информации о цикле */

  variables[i.var]++; /* увеличение управляющей переменной*/
  if(variables[i.var]>i.target) return; /* конец цикла */
  fpush(i); /* иначе, сохранить информацию в стеке */
  prog = i.loc; /* цикл */
}

/* Поместить информацию в стек FOR */
void fpush(struct for_stack i)

{
  if(ftos>FOR_NEST)
    serror(10);
  fstack[ftos]=i;
  ftos++;
}
struct for_stack fpop()
{
  ftos--;
  if(ftos<0) serror(11);
  return(fstack[ftos]);
}

/* Реализация оператора INPUT */
void input()
{
  char str[80], var;
  int i;

  get_token(); /*просматривается если существует строка символов*/
  if(token_type==QUOTE)
  {
    printf(token); /* если да, то ее печать и контроль ',' */
    get_token();
    if(*token!=',') serror(1);
    get_token();
  }
  else printf("? "); /* выдача строки по умолчанию */
  var = toupper(*token)-'A'; /* получить индекс имени переменной*
scanf("%d",&i);   /* чтение ввода данных */
  variables[var] = i;  /* сохранение данных */
}

/* Реализация оператора GOSUB */
void gosub()
{
  char *loc;

  get_token();
  /* поиск метки вызова */
  loc = find_label(token);
  if(loc=='\0')
    serror(7); /* метка не определена */
  else
  {
    gpush(prog); /* запомним место, куда вернёмся */
    prog = loc; /* старт программы с указанной точки */
  }
}

/* Возврат из подпрограммы, вызванной по GOSUB */
void greturn()
{
  prog = gpop();
}

/* Помещает данные в стек GOSUB */
void gpush(char *s)
{
  gtos++;
  if(gtos==SUB_NEST)
  {
    serror(12);
    return;
  }
  gstack[gtos]=s;
}

/*   */
char *gpop()
{
  if(gtos==0)
  {
    serror(13);
    return 0;
  }
  return(gstack[gtos--]);
}


#endif // FUNCTIONS_H_INCLUDED
