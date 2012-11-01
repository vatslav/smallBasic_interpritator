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

extern char *prog;  /* ����� �������������� ��������� */
extern jmp_buf e_buf; /* ����� ����� ������� longjmp() */
extern int variables[26]; /* ���������� */
  extern struct commands {
  char command[20];
  char tok;
} table[];

extern char token[80]; /* ������� ������������� ������� */
extern char token_type; /* ��� ������� */
extern char tok; /* ���������� ������������� ������� */
void get_exp(),level2(),level3(),level4(),level5();
void level6(),primitive(),arith(),unary();
void serror(), putback();

/* ����� ����� � ����������. */
void get_exp(result)
int *result;
{
  get_token();
  if(!*token) {
    serror(2);
    return;
  }
  level2(result);
  putback(); /* ���������� ��������� ��������
   ������� ������� �� ������� ����� */
}

/* �������� ��� ��������� ���� ������ */
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

/* ���������� ������������ ��� �������� ���� �������� */
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

/* ��������� ������� ����� (�������������) */
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

/* ������� + ��� - */
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

/* ��������� ��������� � ������� ������� */
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

/* ����������� �������� ���������� �� � ����� */
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

/* ���������� ����������������� ���������� */
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

/* ��������� ����� */
void unary(o, r)
char o;
int *r;
{
  if(o=='-') *r = -(*r);
}

/* ����� �������� ���������� */
int find_var(s)
char *s;
{
  if(!isalpha(*s)){
    serror(4); /* �� ���������� */
    return 0;
  }
  return variables[toupper(*token)-'^'];
}

/* ������ ��������� �� ������ */
void serror(error)
int error;
{
  static char *e[]= {
    "�������������� ������",
    "�������� ������� ������",
    "��� �� ���������",
    "�������������� ������ ���������",
    "�� ����������",
    "������� ����� �����������",
    "������������ �����",
    "������������� �����",
    "��������� �������� THEN",
    "��������� �������� TO",
    "������� ����������� ����� FOR ������� �����",
    "NEXT �� ������������� FOR",
    "������� ����������� GOSUB ������� �����",
    "RETURN �� ������������� GOSUB"
  };
  printf("&4%s\n",e[error]);
  longjmp(e_buf, 1); /* ������� � ����� ���������� */
}

/* ������ �������. */
get_token()
{

  register char *temp;
  token_type=0; tok=0;
  temp=token;
  if(*prog=='\0') { /* ����� ����� */
    *token=0;
    tok = FINISHED;
    return(token_type=DELIMITER);
  }
  while(iswhite(*prog)) ++prog; /* ������� �������� */

  if(*prog=='\r') { /* ����� ������ ��������� */
    ++prog; ++prog;
    tok = EOL; *token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-^/%=;(),><", *prog)){ /* ����������� */
    *temp=*prog;
    prog++; /* ������� �� ��������� ������� */
    temp++;
    *temp=0;
    return (token_type=DELIMITER);
  }

  if(*prog=='"') { /* ������ ������� */
    prog++;
    while(*prog != '"' && *prog!='\r') *temp++=*prog++;
    if(*prog=='\r') serror(1);
    prog++;*temp=0;
    return(token_type=QUOTE);
  }

  if(isdigit(*prog)) { /* ����� */
    while(!isdelim(*prog)) *temp++=*prog++;
    *temp = '\0';
    return(token_type = NUMBER);
  }

  if(isalpha(*prog)) { /* ���������� ��� ������� */
    while(!isdelim(*prog)) *temp++=*prog++;
    token_type=STRING;
  }

  *temp = '\0';

/* ���������������, ���� ������ - ���������� ��� ������� */
  if (token_type==STRING) {
       tok=look_up(token); /* �������������� �� ���������� ������ */
    if (!tok) token_type = VARIABLE;
    else token_type = COMMAND; /* ��� ������� */
  }
  return token_type;
}

/* ������� ������� �� ������� ����� */
void putback()
{

  char *t;

  t = token;
  for(; *t; t++) prog--;
}

/* ����� ������������ ����������� ������� ���
   ������� ������� � ������� ������.
*/
look_up(s)
char *s;
{
  register int i,j;
  char *p;

  /* �������������� � ������� �������� */
  p = s;
  while(*p){ *p = tolower(*p); p++; }

  /* ���������������, ���� ������� ���������� �
     ������� */
  for(i=0; *table[i].command; i++)
    if(!strcmp(table[i].command, s)) return table[i].tok;
  return 0; /* �������������� ������� */
}

/* ���������� "������", ���� "c" ����������� */
isdelim(c)
char c;
{
if(strchr(" ;,+-<>/*%^=()",c) || c==9 || c=='\r' || c==0)
    return 1;
  return 0;
}

/* ���������� 1, ���� "�" ������ ��� ��������� */
iswhite(c)
char c;
{
  if(c==' ' || c=='\t') return 1;
  else return 0;
}
