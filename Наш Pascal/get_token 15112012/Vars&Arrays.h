#ifndef VARS&ARRAYS_H_INCLUDED
#define VARS&ARRAYS_H_INCLUDED

#include "Globals.h"
#include "get_token.h"
#include <map>
#include <string>

using std::string;
using std::make_pair;

//��������� ��� ���������� ���� INTEGER
/**
string - ��� ��������������
int/float - �������� ��������������
bool - ���������/����������*/
struct IntVar
{
    string name;
    int value;
    bool isConst;
};

//��������� ��� ���������� ���� REAL
struct RealVar
{
    string name;
    float value;
    bool isConst;
};

//��� �������� ����������/�������� � �������� ����������
//������������� ������� (�����/����������)

std::map <string, IntVar> Integers; //����� ��� INTEGER
std::map <string, RealVar> Reals;  //����� ��� REAL

std::map <string, IntVar>::iterator start = Integers.begin ();
std::map <string, RealVar>::iterator start2 = Reals.begin ();
/**
string - ��� ��������������
int - ������ ������ �������
int - ������ ����� �������
*/
std::map <string, int, int> Arrays;    //����� ��� ��������

IntVar temp;
RealVar temp2;

void AddVar (char* name, char type, int value,/*void* value,*/ bool constant) //������� ���������� �������������� � �����
{
    if (type == INTEGER) ///���� ��� ���������� INTEGER
    {
       if (Integers.count(name) == 0)
       {
           temp.name = name;
           temp.isConst = constant;
           //temp.value = *(int*)(value);
           temp.value = value;
           Integers[name] = temp;
       }
       else
           printf ("The variable with this name is already exists");
    }
    else if (type == REAL) ///���� ��� ���������� REAL
    {
       if (Reals.count(name) == 0)
       {
           temp2.name = name;
           temp2.isConst = constant;
           temp2.value = *(float*)(value);
           Reals[name] = temp2;
       }
       else
           printf ("The variable with this name is already exists");
    }
}

char GetTypeVar (char* name) //������� ��������� ���� ����������
{
    if (Integers.count(name) != 0)
      // if ()
       return INTEGER;
    else if (Reals.count(name) != 0)
       return REAL;
    else return 0;
}

void PrintMap ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = Integers.begin (); start != Integers.end (); start++)
    //   printf ("%s\t%d\tINTEGER",/*(*start).first*/(*start).second.name/*start->second.value*/);
    std::cout<<(*start).second.name<<"\t\t"<<(*start).second.value<<"\t\tINTEGER"<<"\n";
}



#endif // VARS&ARRAYS_H_INCLUDED
