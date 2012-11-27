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
int/float - �������� ��������������
bool - ���������/����������*/
struct structVar
{
    //string name;
    void *value; ///��������� �� ���������� ��� ������
    bool isConst;
    int type;
};

struct array_describer
{
	char basetype;// ������� ��� ������� ( INTEGER, REAL ��� ARRAY )
	int from; // ������ ������
	int to; // ������� ������
	void* vars; // ������ �� �������� ������� ��� �� array_describer ( ���� basetype = ARRAY )
};


////��������� ��� ���������� ���� REAL
//struct RealVar
//{
//    //string name;
//    float value;
//    bool isConst;
//};

//��� �������� ����������/�������� � �������� ����������
//������������� ������� (�����/����������)

//std::map <string, IntVar> Integers; //����� ��� INTEGER
std::map <string, structVar> var;  //����� ��� REAL

std::map <string, structVar>::iterator start = var.begin ();
//std::map <string, RealVar>::iterator start2 = Reals.begin ();
/**
string - ��� ��������������
int - ������ ������ �������
int - ������ ����� �������
*/
//std::map <string, int, int> Arrays;    //����� ��� ��������

structVar temp;
//RealVar temp2;

void addVar (char* name, int type, void* value, bool constFlag) //������� ���������� �������������� � �����
{

    if (type == INTEGER) ///���� ��� ���������� INTEGER
    {
       if (var.count(name) == 0)
       {

           //temp.name = name;
           temp.isConst = constFlag;
           //temp.value = *(int*)(value);
           temp.value = value;
           temp.type = type;
           var[name] = temp;

       }
       else
           printf ("The variable with this name is already exists");
    }
//    else if (type == REAL) ///���� ��� ���������� REAL
//    {
//       if (Reals.count(name) == 0)
//       {
//           //temp2.name = name;
//           temp2.isConst = constant;
//           temp2.value = value;
//           //temp2.value = value;
//           Reals[name] = temp2;
//       }
//       else
//           printf ("The variable with this name is already exists");
//    }
}

char GetTypeVar (const char* name) //������� ��������� ���� ����������
{
    if (var.count(name) != 0)
      // if ()
       return INTEGER;
    else if (var.count(name) != 0)
       return REAL;
    else return 0;
}

void PrintMap ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = var.begin (); start != var.end (); start++)
       //printf ("%s\t\t%d\t\t%d - %s\n",start->first.c_str(), start->second.value, GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );
std::cout<<start->first.c_str()<<"\t"<<start->second.value<<"\t"<<GetTypeVar (start->first.c_str() )<<" - "<<typeToName(GetTypeVar (start->first.c_str() ));
//    for (start2 = var.begin (); start2 != var.end (); start2++)
//       printf ("%s\t\t%5.3f\t\t%d - %s\n",start2->first.c_str(), start2->second.value, GetTypeVar (start2->first.c_str() ), typeToName(GetTypeVar (start2->first.c_str() )) );
    //std::cout<<(*start).second.name<<"\t\t"<<(*start).second.value<<"\t\tINTEGER"<<"\n";
}

#endif // VARS&ARRAYS_H_INCLUDED
