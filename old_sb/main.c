#include "stdio.h"
#include "setjmp.h"
#include "math.h"
#include "ctype.h"
#include "stdlib.h"
#include "analizator.c"
#include "get.c"

#define NUM_LAB     100
#define LAB_LEN      10
#define FOR_NEST     25
#define SUB_NEST     25
#define PROG_SIZE 10000

#define DELIMITER 1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE     6

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

char *prog; /* содержит выражение для анализа */
jmp_buf e_buf; /* содержит среду для longjmp() */

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
void exec_if(), exec_for(), next(), fpush(), input();
void gosub(), greturn(), gpush(), label_init();

main(argc, argv)
int argc;
char *argv[];
{
  char in[80];
  int answer;
  char *p_buf;
  char *t;

  if(argc!=2)
  {
    printf("Используйте формат: run <filename>\n");
    exit(1);
  }

  /* Выделение памяти для программы */
  if (!(p_buf=(char *) malloc(PROG_SIZE)))
  {
    printf("Ошибка при выделении памяти ");
    exit(1);
  }

  /* Загрузка программы для выполнения */
  if(!load_program(p_buf,argv[1])) exit(1);
  if(setjmp(e_buf)) exit(1); /* инициализация буфера
 нелокальных переходов */
  prog = p_buf;
  scan_labels(); /* поиск метки в программе */
  ftos = 0; /* инициализация индеса стека FOR */
  gtos = 0; /* инициализация индеса стека GOSUB */
  do
  {
    token_type = get_token();
    /* проверка на оператор присваивания */
    if(token_type==VARIABLE)
    {
      putback(); /* возврат пер. обратно во входной поток */
      assignment(); /* должен быть оператор присваивания */
    }
    else /* это команда */
      switch(tok)
      {
      case PRINT:
        print();
        break;
      case GOTO:
        exec_if();
        break;
      case FOR:
        exec_for();
        break;
      case NEXT:
        next();
        break;
      case INPUT:
        input();
        break;
      case GOSUB:
        gosub();
        break;
      case RETURN:
        greturn();
        break;
      case END:
        exit(0);
      }
  }
  while (tok != FINISHED);
}

/* Загрузка программы. */
load_program(p, fname)
char *p;
char *fname;
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

/* Присваивание переменной значения */
assigment()
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
      print(token);
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
        print(" ");
        spaces--;
      }
    }
    else if(*token==','); /* ничего не делать */
    else if(tok!=EOL && tok!=FINISHED)  serror(0);
  }
  while (*token == ';' || *token == ',');

  if(tok==EOL || tok==FINISHED)
  {
    if(last_delim != ';' && last_delim != ',') printf("\n");
  }
  else serror(0); /* отсутствует ',' или ';'  */
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

/* Возвращает индекс на следующую свободную позицию
   массива меток. -1, если массив переполнен.
 -2, если дублирование меток. */

get_next_label(s)
char *s;
{
  register int t;

  for(t=0; t<NUM_LAB; ++t)
  {
    if(label_table[t].name[0]==0) return t;
    if(!strcmp(label_table[t].name,s)) return -2; /*дубль*/
  }

  return -1;
}

/* Поиск строки по известной метке. Значение 0 возвращается,
если метка не найдена; в противном случае возвращается
указатель на помеченную строку программы*/

char *find_label(s)

char *s;
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
нулевое значение метки символизирует пустую ячейку массива *
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
void fpush(i)
struct for_stack i;
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
void gpush(s)
char *s;
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
    return;
  }
  return(gstack[gtos--]);
}
