#ifndef GET_TOKEN_H_INCLUDED
#define GET_TOKEN_H_INCLUDED

#include "Globals.h"
#include <stdio.h>

/* ������ �������. */
char get_token()
{
  register char *temp;
  token_type = 0; tok = 0;
  temp = token;

  while(iswhite(*prog)) ++prog; /* ������� �������� */

  if (*prog == '{') /* ����������� */
  { // � ������������ ��� ����������� ������������� � ����. ��� ������ ������������
    int cnt = 1;
    while (cnt != 0)
    {
      prog++;
      if (*prog == '{')
         cnt++;
      else if (*prog == '}')
         cnt--;
      else if (*prog == '\0')
      {
          printf("Error: ����������� ������ ������ � '{'");
          break;
      }
    }
    if ( *prog ) prog++;
	while(iswhite(*prog)) ++prog; /* ������� �������� */
  }

  if(*prog == '\0') /* ����� ����� */
  {
    *token=0;
    tok = 0;
    return (token_type=FINISHED); //
  }

  if (strchr(".,:;()[]", *prog)) /* ����������� */
  {
    bool assign = false; //������� ������������
    *temp = *prog;

    if (*temp == ':')  //���� ���������� �� ":",
       assign = true;  //  �������� ��� ������������

    prog++; // ������� �� ��������� �������
    temp++;

    if (*prog == '=' and assign == true)  //���� ��������� "=" ����� ":",
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

    if(*prog == '<' or *prog == '>') //���� ���������� �� "<" ��� ">",
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

  if(isalpha(*prog) or *prog == '_') /* ��� "��������������", "���������" ��� "��������� �����"*/
  {
    //������� ���������� ���� ���������������:
    //  1) ��� ���������� � ��������� ����� ��� ������� '_'.
    //  2) ���������� ��������� ����� ���� ��������� �����, ����� � '_': _A2_B, A2.
    //����� ���������� � ����. ����� ������� ������ �� ��������� ����: IF, For � �.�.

    //C�������� ��� ���� ��������� �����, ����� ��� '_'
    int ptr=0;
    while(is_good_name (*prog))
    {
      *temp++ = *prog++;
      ptr++;
    }

    //E��� ��������� ������������ ������ ������ �����������, ������ ������
    if (!isdelim (*prog)) //
    {
        *token = 0;
        printf("ERROR! Uncorrect IDENTIFIER or OPERATOR name\n");
        return ( token_type = ERROR ); //��� "������"
    }
    if (ptr>8)
    {
        *token = 0;
        printf("ERROR! name of variable > 8\n");
        return ( token_type = ERROR ); //��� "������"
    }
    *temp = 0;

    char type; //��� �������
    tok = look_up(token, &type); // �������������� �� ���������� ������.

    if ( type != 0 ) //���� ����� ������� ����� "����������" ��� "��������� ����", ���������� ��� �������
        return ( token_type = type );
    else //����� ����� ���� � "���������������": ���������� ��� ����������
    {
      return ( token_type = VARIABLE );
        ///���� ��������
    }
  }
  ///�� ���� ���� �� �������� �� ������, ��� �������� ��� ���������� (���������
  else
    {///��� ����--
      *token = 0;
      printf("ERROR! Uncorrect IDENTIFIER or OPERATOR name\n");
      return ( token_type = ERROR ); //��� "������"
    }
}



#endif // GET_TOKEN_H_INCLUDED
