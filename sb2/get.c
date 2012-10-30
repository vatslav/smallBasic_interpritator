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
extern char *prog;  /* —одержит анализируемое выражение */
/* ѕолучить лексему */
get_token()
{
  register char *temp;
  token_type=0; tok=0;
  temp=token;

  if(*prog=='\0')  { /*  онец файла */
    *token=0;
    tok=FINISHED;
    return(token_type=DELIMITER);
  }
  while(iswhite(*prog)) ++prog;  /* пропуск пробелов */

  if(*prog=='\r') { /* crtl */
    ++prog; ++prog;
    tok= EOL; *token='\r';
    token[1]='\n';token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-*^/%=;(),><", *prog)) { /* разделитель */
    *temp=*prog;
    prog++; /* переход на слкдующую позицию */
    temp++;
    *temp=0;
    return (token_type=DELIMITER);
  }

  if(*prog=='"')  { /* строка в кавычках */
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

  if(isalpha(*prog))  { /* переменна€ или команда */
    while(!isdelim(*prog)) *temp++=*prog++;
    token_type=STRING;
  }

  *temp = '\0';
      /* ѕросматриваетс€, если строка есть команда или переменна€ */
  if(token_type==STRING) {
    tok=look_up(token); /* преобразование во внутренний
формат */
    if(!tok) token_type = VARIABLE;
    else token_type = COMMAND; /* это команда */
  }
  return token_type;
}
