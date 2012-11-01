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

char *prog; /* �������� ��������� ��� ������� */
jmp_buf e_buf; /* �������� ����� ��� longjmp() */

int variables[26]=   /* 26 ���������� ������������ A - Z */
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

struct commands   /* �������� ������� �������� ���� */
{
  char command[20];
  char tok;
} table[] =   /* ������� ������ ��������� ���������� */
{
  "print",PRINT, /* ������� � ��� ������� */
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
  "",END  /* ������ ����� ������� */
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
  int var; /* ���������� �������� */
  int target; /* �������� �������� */
  char *loc;
} fstack[FOR_NEST]; /* ���� ����� FOR/NEXT */
struct for_stack fpop();

char *gstack[SUB_NEST]; /* ���� ��������� GOSUB */
int ftos; /* ������ ������ ����� FOR */
int gtos; /* ������ ������ ����� GOSUB */

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
    printf("����������� ������: run <filename>\n");
    exit(1);
  }

  /* ��������� ������ ��� ��������� */
  if (!(p_buf=(char *) malloc(PROG_SIZE)))
  {
    printf("������ ��� ��������� ������ ");
    exit(1);
  }

  /* �������� ��������� ��� ���������� */
  if(!load_program(p_buf,argv[1])) exit(1);
  if(setjmp(e_buf)) exit(1); /* ������������� ������
 ����������� ��������� */
  prog = p_buf;
  scan_labels(); /* ����� ����� � ��������� */
  ftos = 0; /* ������������� ������ ����� FOR */
  gtos = 0; /* ������������� ������ ����� GOSUB */
  do
  {
    token_type = get_token();
    /* �������� �� �������� ������������ */
    if(token_type==VARIABLE)
    {
      putback(); /* ������� ���. ������� �� ������� ����� */
      assignment(); /* ������ ���� �������� ������������ */
    }
    else /* ��� ������� */
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

/* �������� ���������. */
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
  *(p-2) = '\0'; /* ������ ����� ��������� */
  fclose(fp);
  return 1;
}

/* ������������ ���������� �������� */
assigment()
{
  int var, value;
  /* �������� ��� ���������� */
  get_token();
  if(!isalpha(*token))
  {
    serror(4); /* ��� �� ���������� */
    return;
  }

  /* ���������� ������� ���������� */
  var = toupper(*token)-'A';

  /* �������� ���� ��������� */
  get_token();
  if(*token!='=')
  {
    serror(3);
    return;
  }

  /* �������� ��������, ���������� ���������� */
  get_exp(&value);

  /* ��������� ��� �������� */
  variables[var] = value;
}

/* ���������� ���������� ��������� PRINT */
void print()
{
  int answer;
  int len=0, spaces;
  char last_delim;

  do
  {
    get_token(); /* �������� ��������� ������� ������  */
    if(tok==EOL || tok==FINISHED) break;
    if(token_type==QUOTE)   /* ��� ������ */
    {
      print(token);
      len += strlen(token);
      get_token();
    }
    else   /* ��� ��������� */
    {
      putback();
      get_exp(&answer);
      get_token();
      len +=printf("%d", answer);
    }
    last_delim = *token;

    if(*token==';')
    {
      /* ���������� ����� �������� ��� �������� � ��������� ���������*/
      spaces= 8- (len % 8);
      len += spaces; /* ������� ������� ���������  */
      while(spaces)
      {
        print(" ");
        spaces--;
      }
    }
    else if(*token==','); /* ������ �� ������ */
    else if(tok!=EOL && tok!=FINISHED)  serror(0);
  }
  while (*token == ';' || *token == ',');

  if(tok==EOL || tok==FINISHED)
  {
    if(last_delim != ';' && last_delim != ',') printf("\n");
  }
  else serror(0); /* ����������� ',' ��� ';'  */
}

/* ����� ���� ����� */
void scan_labels()
{
  int addr;
  char *temp;

  label_init();  /* ��������� ���� ����� */
  temp = prog; /* �������� ��������� �� ������ ���������*/
  /* ���� ������ ������� ����� ���� �����  */
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
      label_table[addr].p = prog; /* ������� ��������� ��������� */
    }
    /* ���� ������ �� ��������, �� ����� ��������� */
    if(tok!=EOL) find_eol();
  }
  while(tok!=FINISHED);
  prog = temp; /* �������� �������� */
}

/* ����� ������ ��������� ������ */
void find_eol()
{
  while(*prog!='\n'  && *prog!='\0') ++prog;
  if(*prog) prog++;
}

/* ���������� ������ �� ��������� ��������� �������
   ������� �����. -1, ���� ������ ����������.
 -2, ���� ������������ �����. */

get_next_label(s)
char *s;
{
  register int t;

  for(t=0; t<NUM_LAB; ++t)
  {
    if(label_table[t].name[0]==0) return t;
    if(!strcmp(label_table[t].name,s)) return -2; /*�����*/
  }

  return -1;
}

/* ����� ������ �� ��������� �����. �������� 0 ������������,
���� ����� �� �������; � ��������� ������ ������������
��������� �� ���������� ������ ���������*/

char *find_label(s)

char *s;
{
  register int t;

  for(t=0; t<NUM_LAB; ++t)
    if(!strcmp(label_table[t].name,s)) return label_table[t].p;
  return '\0'; /* ��������� ������ */
}

/* ���������� ��������� GOTO */
void exec_goto()
{
  char *loc;

  get_token(); /* �������� ����� �������� */

  /* ����� �������������� ����� */
  loc = find_label(token);
  if(loc=='\0')
    serror(7);   /* ����� �� ����������  */
  else prog=loc; /* ����� ��������� � ��������� �����  */
}

/* ������������� ������� �������� �����. �� ��������������
������� �������� ����� ������������� ������ ������ ������� *
void label_init()
{
register int t;

for(t=0; t<NUM_LAB; ++t) label_table[t].name[0]='\0';
}

/* ���������� ��������� IF  */
void exec_if()
{
  int x , y, cond;
  char op;
  get_exp(&x); /* �������� ����� ��������� */
  get_token(); /* �������� �������� */
  if(!strchr("=<>", *token))
  {
    serror(0);    /* ������������ �������� */
    return;
  }
  op=*token;
  get_exp(&y);  /* �������� ������ ��������� */
  /* ����������� ���������� */
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
  if(cond)  /* ���� �������� IF "������"  */
  {
    get_token();
    if(tok!=THEN)
    {
      serror(8);
      return;
    } /* �����, ��������� ����������� �� ��������� ������ */
  }
  else find_eol(); /* ����� ����� ������ ��������� */
}

/* ���������� ����� FOR */
void exec_for()

{
  struct for_stack i;
  int value;

  get_token(); /* �������� ����������� ���������� */
  if(!isalpha(*token));
  {
    serror(4);
    return;
  }
  i.var=toupper(*token)-'A'; /* ��������� �� ������ */
  get_token(); /* �������� ���� ��������� */
  if(*token!='=')
  {
    serror(3);
    return;
  }
  get_exp(&value); /* �������� ��������� ��������  */
  variables[i.var]=value;

  get_token();
  if(tok!=TO) serror(9); /* ���� ��� ����� TO */
  get_exp(&i.target); /* �������� �������� �������� */

  /* ���� ���� ����������� ��������� ���, ���������
     ���������� � ����  */
  if(value>=variables[i.var])
  {
    i.loc = prog;
    fpush(i);
  }
  else /* ���������� ���� ���� */
    while(tok!=NEXT) get_token();
}

/* ���������� ��������� NEXT */
void next()
{
  struct for_stack i;
  i = fpop(); /* ������ ���������� � ����� */

  variables[i.var]++; /* ���������� ����������� ����������*/
  if(variables[i.var]>i.target) return; /* ����� ����� */
  fpush(i); /* �����, ��������� ���������� � ����� */
  prog = i.loc; /* ���� */
}

/* ��������� ���������� � ���� FOR */
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

/* ���������� ��������� INPUT */
void input()
{
  char str[80], var;
  int i;

  get_token(); /*��������������� ���� ���������� ������ ��������*/
  if(token_type==QUOTE)
  {
    printf(token); /* ���� ��, �� �� ������ � �������� ',' */
    get_token();
    if(*token!=',') serror(1);
    get_token();
  }
  else printf("? "); /* ������ ������ �� ��������� */
  var = toupper(*token)-'A'; /* �������� ������ ����� ����������*
scanf("%d",&i);   /* ������ ����� ������ */
  variables[var] = i;  /* ���������� ������ */
}

/* ���������� ��������� GOSUB */
void gosub()
{
  char *loc;

  get_token();
  /* ����� ����� ������ */
  loc = find_label(token);
  if(loc=='\0')
    serror(7); /* ����� �� ���������� */
  else
  {
    gpush(prog); /* �������� �����, ���� ������� */
    prog = loc; /* ����� ��������� � ��������� ����� */
  }
}

/* ������� �� ������������, ��������� �� GOSUB */
void greturn()
{
  prog = gpop();
}

/* �������� ������ � ���� GOSUB */
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
