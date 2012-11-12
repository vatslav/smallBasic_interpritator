#ifndef GET_TOKEN_H_INCLUDED
#define GET_TOKEN_H_INCLUDED

#include <string.h>
#include <stdio.h>
///���� ������ (������ ������)
#define DELIMITER  1
#define VARIABLE   2
#define INTEGER    3
#define REAL       4
#define OPERATOR   5
#define SYSTEM     6
#define QUOTE      7
#define ASSIGNMENT 8
#define ERROR      9

#define TRUE       1
#define FALSE      -1
#define VAR        10
#define CONST      11
#define ARRAY      12
#define OF         13
#define SYSTEM     14

#define FINISHED   15
#define EOL        16
///������� �������� ������������� �� ���������
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

//��������� �����
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
	{ "", 0 }//������ ������� ����� �������
};

//���������
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
	{ "", 0 }//������ ������� ����� �������
};

char token[80];
char token_type; //��� �������
char tok;  //��������� ������������� �������

char *prog; /* �������� ��������� ��� ������� */
//jmp_buf e_buf; /* �������� ����� ��� longjmp() */

/* ���������� 1, ���� "�" ������ ��� ��������� */
bool iswhite(char c)
{
  if(c == ' ' || c == '\t')
     return 1;
  else
     return 0;
}

/* ���������� "������", ���� "c" ����������� */
bool isdelim(char c)
{
	if(strchr(".,:;()[]+-*/<>=", c) || c==9 || c=='\r' || c == 0)
		return 1;
	else
		return 0;
}

/* ���������� "������", ���� "c" �������� ���-�� �� { a..z, A..Z, 0..9, "_" } */
bool is_good_name (char c)
{
    return (isalpha(c) or isdigit(c) or c == '_' );
}

/* ����� ������������ ����������� ������� ���
   ������� ������� � �������� ������.
*/
char look_up(char *s, char *type)
{
  register int i;
  char *p;

  /* �������������� � ������� �������� */
  p = s;
  while(*p) { *p = tolower(*p); p++; }

  //����� ������� ����� ��������� ����
  for( i = 0; *SYSTEM_table[i].command; i++ )
    if( !strcmp(SYSTEM_table[i].command, s) )
    {
        *type = SYSTEM; //��� "��������� �����"
        return SYSTEM_table[i].tok;
    }

  //����� ������� ����� ����������
  for( i = 0; *OPERATORS_table[i].command; i++ )
    if( !strcmp( OPERATORS_table[i].command, s ) )
    {
        *type = OPERATOR; //��� "��������"
        return OPERATORS_table[i].tok;
    }
  *type = 0;
  return 0; /* �������������� ������� */
}

/* ������ �������. */
char get_token()
{
  register char *temp;
  token_type = 0; tok = 0;
  temp = token;

  if(*prog == '\0') /* ����� ����� */
  {
    *token=0;
    tok = FINISHED;
    return (token_type=DELIMITER); //
  }

  if(*prog == '\r') /* ����� ������ ��������� */
  {
    ++prog; ++prog;
    tok = EOL; *token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER); // ��� "�����������"
  }

  if (*prog == '{') /* ����������� */
  {
    bool close = false; // ������� ���������� �����������
    while (close != true)
    {
      prog++;
      //if (*prog == '{')
        // cnt ++;
      if (*prog == '}')
         close = true;
      else if (*prog == '\0')
         break;      // � ������������ ��� ����������� ������������� � ����. ��� ������ ������������
    }
    if ( *prog ) prog++;
	while(iswhite(*prog)) ++prog; /* ������� �������� */
  }

  if (strchr(".,:;()[]", *prog)) /* ����������� */
  {
    bool assign = false; //������� ������������
    *temp = *prog;

    if (*temp == ':')  //���� ���������� �� ":",
       assign = true;  //  �������� ��� ������������

    prog++; // ������� �� ��������� �������
    temp++;

    if (*temp == '=' and assign == true)  //���� ��������� "=" ����� ":",
    {                                    //  ������ ��� ����� ������������
        *temp = *prog;
        prog++;
        temp++;
        *temp = 0;
        tok = ASSIGNMENT;  //���������� ������������� ������������.
        return ( token_type = OPERATOR ); // ��� "��������"
    }

    *temp = 0;
    return (token_type = DELIMITER);  // ��� "�����������"
  }

  if (strchr("+-*/=<>", *prog)) /* �������� */
  {
    bool two_symb = false; //������� ��������������� ��������� "<=", ">=" ��� "<>"
    *temp = *prog;

    if(*temp == '<' or *temp == '>') //���� ���������� �� "<" ��� ">",
       two_symb = true;              //  �������� ��� �������������� �������� "<=", ">=" ��� "<>"

    prog++; // ������� �� ��������� �������
	temp++;

    if (two_symb == true)
        if( (*temp == '=') or (*(temp-1) == '<' and *temp == '>' ) )
        {         //���� ��������� "=" ����� "<" ��� ">" ("<=",">=") ���
           *temp = *prog;  // ">" ����� "<" ("<>"), ������ �������� ��������������
           prog++;
           temp++;
        }

    *temp = 0;
    return (token_type = OPERATOR);  // ��� "��������"
  }

  if(isdigit(*prog) == true) /* ����� */
  {
    while (isdigit(*prog) == true) //���� ��������� �����, ���������
       *temp++=*prog++;

    if(*prog == '.') //���� ���������� �� �����, �� ��� ���� ������������ ����� "REAL",
    {                //���� �������� �������� ������ �������
      if (*(prog+1) == '.')//���� 2 ����� ������, ������
      {                    //��� �������� �������� ������ ������� "A:array [3..12]"
        *temp = 0;
        return (token_type = INTEGER); //��� "����� �����"
      }

      *temp++ = '.';
      prog++;
      while(isdigit(*prog) == true) //���� ��������� �����, ���������
         *temp++ = *prog++;

      if(*(prog-1) == '.')   // ���� ���� �� ���� �� ����������,
      {                      // ������ ������������ ��������� ���� "17.a"
        *token = '\0';
        printf("ERROR! Expected REAL number\n");
        return (token_type = ERROR); //��� "������"
      }

      if (isdelim(*prog) == false)  //���� �� ��������� ����������� ��� ���������,
      {                              // ������ ������������ ��������� ���� "17.23a"
        *token = 0;
        printf("ERROR! Expected REAL number\n");
        return (token_type = ERROR);  //��� "������"
      }

      *temp = '\0';
      return(token_type = REAL); //��� "������������ �����"
    }
    else if (isdelim(*prog) == false) //���� �� ��������� ����������� ��� ���������,
    {                                  // ������ ������������ ��������� ���� "17a"
      *token = 0;
      printf("ERROR! Expected INTEGER number\n");
      return (token_type = ERROR);  //��� "������"
    }

	*temp = '\0';
	return(token_type = INTEGER); //��� "����� �����"
  }

  if(is_good_name (*prog) == true) /* ��� "��������������", "���������" ��� "��������� �����"*/
  {
    //������� ���������� ���� ���������������:
    //  1) ��� ���������� � ��������� ����� ��� ������� '_'.
    //  2) ���������� ��������� ����� ���� ��������� �����, ����� � '_': _A2_B, A2.
    //����� ���������� � ����. ����� ������� ������ �� ��������� ����: IF, For � �.�.

    //C�������� ��� ���� ��������� �����, ����� ��� '_'
    while(isalpha(*prog) or isdigit(*prog) or *prog == '_')
       *temp++ = *prog++;

    //E��� ��������� ������������ ������ ������ �����������, ������ ������
    if (!isdelim (*prog)) //
    {
        *token = 0;
        printf("ERROR! Uncorrect IDENTIFIER or OPERATOR name\n");
        return ( token_type = ERROR ); //��� "������"
    }
    *temp = 0;

    char type; //��� �������
    tok = look_up(token, &type); // �������������� �� ���������� ������.

    if ( type != 0 ) //���� ����� ������� ����� "����������" ��� "��������� ����", ���������� ��� �������
        return ( token_type = type );
    else //����� ����� ���� � "���������������": ���������� ��� ����������
    {
        //���� ��������
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
