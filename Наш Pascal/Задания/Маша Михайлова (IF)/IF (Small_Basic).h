#ifndef IF_H_INCLUDED
#define IF_H_INCLUDED

//#include "Globals.h"
        //�������� ������ ���������:
/**
        IF <���������> <��������> <���������> THEN <��������>
*/

 // ��������, ������� ����� THEN, ����������� ������ � ��� ������, ���� �������� ��������� �������� ��������.
 // ������� exec_if(), ���������� ����, ������������ ���������� ���� ����� ��������� IF.*/

    /* ���������� ���������� ��������� IF  */

    void exec_if()
    {
      int x , y, cond;
      char op;
      get_exp(&x); /* �������� ����� ��������� */
      get_token(); /* �������� ��������  */
      if (!strchr("=<>", *token))
      {
        serror(0);      /* ������������ ��������  */
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
      if(cond)
      {             /* ���� ������, �� ����� ������� IF  */
        get_token();
        if(tok!=THEN)
        {
          serror(8);
          return;
        }  /* � ��������� ������ ��������� ������������ ��
            ��������� ������  */
      }
      else find_eol(); /* ����� ������ ����������� ��������� */
    }



#endif // IF_H_INCLUDED
