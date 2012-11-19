#ifndef FOR_H_INCLUDED
#define FOR_H_INCLUDED

//#include "Globals.h"
    //  �������� ������ ��������� ����� FOR ���������:
/**
        FOR <��� ����������� ����������> = <���. ��������> TO <���. ��������>
        ..............................
        ������������������ ����������
        ..............................
        NEXT

*/

//��� ���������� ��������� ����� FOR ���� ������ ����� ��������� ���������:
/**
  struct for_stack
  {
    int var; // ������� �����
    int target; // �������� ��������
    char *loc;
  }
  fstack[FOR_NEST]; // ���� ��� ����� FOR/NEXT
  int ftos; // ������ ������ ����� FOR
*/
        /*
        �������� ������� FOR_NEST ������������ ������� ����������� �����.
        (�� ��������� ����������� 25 ������� �����������).
        ���������� ftos ������ ����� �������� ������� ������ �����.
        ��� ��������� ����� ��� ����������� ��� ������� fpush() � fpop(), ������� ��������� ����.
        */
    /* ��������� ������� � ���� */
    void fpush(i)
    struct for_stack i;
    {
      if (ftos > FOR_NEST)
        serror(10);
      fstack[ftos]=i;
      ftos++;
    }

    struct for_stack fpop()
    {
      ftos--;
      if (ftos < 0) serror(11);
      return(fstack[ftos]);
    }

    /* ���������� ���������� ��������� ����� FOR */
    void exec_for()
    {
      struct for_stack i;
      int value;
      get_token(); /* ������ ����������� ���������� */
      if (!isalpha(*token))
      {
        serror(4);
        return;
      }
      i.var=toupper(*token) - 'A'; /* �������� ������ */
      get_token(); /* ������ ������� ��������� */
      if (*token != '=') {
        serror(3);
        return;
      }
      get_exp(&value); /* �������� ��������� �������� */
      variables[i.var] = value;
      get_token();
      if (tok != TO) serror(9); /* ������ � ������ TO */
        get_exp(&i.target); /* �������� �������� �������� */
      /* ���� ���� ����������� ��������� ���, ��������� ���������� � ����*/
      if (value >= variables[i.var]) {
        i.loc = prog;
        fpush(i);
      }
      else { /* ���������� ���� ���� ������� */
      while(tok != NEXT) get_token();
      }
    }

      /* ���������� ��������� NEXT */
      void next()
      {
        struct for_stack i;
        i = fpop(); /* ������ ���������� � ����� */
        variables[i.var]++; /* ���������� ���������� ����� */
        if (variables[i.var] > i.target) return; /* ����� */
        fpush(i); /* � ��������� ������ ��������� ���������� */
        prog = i.loc; /* ���� */
      }

#endif // FOR_H_INCLUDED
