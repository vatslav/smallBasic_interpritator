#ifndef ASSIGNMENT_H_INCLUDED
#define ASSIGNMENT_H_INCLUDED

//#include "get_token.h"

//  � ����� BASIC �������� ������ ��������� ������������ �������� ���������:
///                <��� ����������> = <���������>
//  ������� assignment() ������������ ���� ��� ������������.
/* ��������� �������� ���������� */

    void assignment()
    {
      int var, value;
      /* �������� ��� ���������� */
      get_token();
      if(!isalpha(*token))
      {
          serror(4); /* ��� �� ���������� */
          return;
      }

      /* ����� ������� ���������� � ������� */
      var = toupper(*token)-'A';

      /* ������� ������ ���������*/
      get_token();
      if(*token!='=')
      {
        serror(3);
        return;
      }

      /* ������� ������������� ���������� �������� */
      get_exp(&value);

      /* ��������� ��������*/
      variables[var] = value;
    }


#endif // ASSIGNMENT_H_INCLUDED
