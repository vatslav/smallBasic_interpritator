#define DELIMITER   1
#define VARIABLE    2
#define NUMBER      3
#define COMMAND     4
#define STRING      5
#define QUOTE       6
#define FINISHED   10
#define EOL9
extern char token[80];
extern int tok, token_type;
extern char *prog;  /* �������� ������������� ��������� */
/* �������� ������� */
get_token()
{
  register char *temp;
  token_type=0; tok=0;
  temp=token;

  if(*prog=='\0')  { /* ����� ����� */
    *token=0;
    tok=FINISHED;
    return(token_type=DELIMITER);
  }
  while(iswhite(*prog)) ++prog;  /* ������� �������� */

  if(*prog=='\r') { /* crtl */
    ++prog; ++prog;
    tok= EOL; *token='\r';
    token[1]='\n';token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-*^/%=;(),><", *prog)) { /* ����������� */
    *temp=*prog;
    prog++; /* ������� �� ��������� ������� */
    temp++;
    *temp=0;
    return (token_type=DELIMITER);
  }

  if(*prog=='"')  { /* ������ � �������� */
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

  if(isalpha(*prog))  { /* ���������� ��� ������� */
    while(!isdelim(*prog)) *temp++=*prog++;
    token_type=STRING;
  }

  *temp = '\0';
      /* ���������������, ���� ������ ���� ������� ��� ���������� */
  if(token_type==STRING) {
    tok=look_up(token); /* �������������� �� ����������
������ */
    if(!tok) token_type = VARIABLE;
    else token_type = COMMAND; /* ��� ������� */
  }
  return token_type;
}
