#ifndef VARS&ARRAYS_H_INCLUDED
#define VARS&ARRAYS_H_INCLUDED

#include "Globals.h"
#include "get_token.h"
#include <map>
#include <string>
#include <stdlib.h>

using std::string;
using std::map;
using std::cout;
using std::cin;
using std::endl;
/**
int type - ��� ��������������
float value - �������� ��������������
bool isConst - ���������/����������
*/

//��������� ��� ���������� ���� INTEGER/REAL
struct Var
{
    int type;
    float value;
    bool isConst;
};

/**
int type - ��� �������
bool isBig - �����������/���������� ������
int from - ������ ������� �������
int to - ������� ������� �������
float *el - �������� �������
*/
//��������� ��� �������� ���� INTEGER/REAL
struct Array
{
    int type;
    bool isBig;
    int from;
    int to;
    float *el; ///��������� �� 1 ������� �������
};

//��� �������� ����������/�������� � �������� ����������
//������������� ������� (�����/����������)

map <string, Var> Vars; //����� ��� ��������������� ���� INTEGER/REAL
map <string, Array> Arrays; //����� ��� �������� ���� INTEGER/REAL

//��������� ��� ������ � ������� � �����
map <string, Var>::iterator start = Vars.begin ();
map <string, Array>::iterator start2 = Arrays.begin ();

//��������� ������� ����� ��������
Var temp;
Array temp2;

/**
������� ���������� ����������/��������� � �����

name - ��� ����������/���������
type - ��� ����������/���������
value - �������� ���������/���������
constant - �������������� (true - ���������, false - ����������)
*/
bool AddVar (char* name, int type, float value, bool constant)
{
   if (Vars.count(name) == 0) //���� � ����� ��� ���������� � ����� ������
   {
       temp.isConst = constant;
       temp.type = type;
       if (type == INTEGER)     //���� ��� ����������/��������� INTEGER
           temp.value = (int)value; //�������� � ���� INTEGER
       else if (type == REAL)   //���� ��� ����������/��������� REAL
           temp.value = value;  //��������� ��� REAL
       Vars[name] = temp;   //������ � ����� ����� ���������/���������
       return 1;
   }
   else //����� ��������, ��� � ����� ���� ����������/��������� � ����� ������
       {
            printf ("The variable/constant with this name is already exists");
           return 0;
       }


}

/**
������� ��������� ���� ����������/���������

name - ��� ����������/���������
return - ��� ����������/���������
*/
int GetTypeVar (const char* name)
{
    if (Vars.count(name) != 0) //������� ����������/��������� � �����
    {
        start->second = Vars[name]; //������ �������� �� ���� �������
        return start->second.type;  //���������� ���
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

/**
������� ��������� �������� ����������/���������

name - ��� ����������/���������
return - �������� ����������/���������
*/
float getValue (const char* name)
{
    if (Vars.count(name) != 0) //������� ����������/��������� � �����
    {
        start->second = Vars[name]; //������ �������� �� ���� �������
        return start->second.value;  //���������� ��������
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        return -1.0;
    }
}

/**
������� �������� �� ���������

name - ��� ����������/���������
return - �������������� (true - ���������, false - ����������)
*/
bool isConst (const char* name)
{
    if (Vars.count(name) != 0) //������� ����������/��������� � �����
    {
        start->second = Vars[name]; //������ �������� �� ���� �������
        return start->second.isConst; //���������� �������������� (true - ���������, false - ����������)
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

///������� ������ ���������� ���� ���� �� ����� (��� �����)
void PrintMap ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = Vars.begin (); start != Vars.end (); start++)

        if (start->second.type == INTEGER)
            printf ("%s\t\t%.0f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );
        //std::cout<<start->first.c_str()<<"\t\t"<<start->second.value<<"\t\t"<<GetTypeVar (start->first.c_str() )<<" - "<<typeToName(GetTypeVar (start->first.c_str() ))<<"\n";
        else if (start->second.type == REAL)
            printf ("%s\t\t%5.3f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );

}

int ReadFromTo ()
{
    get_token(); //��������� ������� �������
	if (token_type == INTEGER)
    {
        if (atoi(token) < 0)
        {
            printf ("������� ������� ������ ���� >= 0\n");
            return -1;
        }

        else
            return atoi(token);
        //temp2.from = atoi(token);
    }

    if (token_type == VARIABLE)
    {

        if (GetTypeVar (token) != INTEGER)
        {
            printf ("������� ������� ������ ���� ���� INTEGER\n");
            return -1;
        }


        if (isConst (token) != true)
        {
            printf ("���������� �� ����� ���� �������� �������\n");
            return -1;
        }

        if (getValue (token) < 0)
        {
            printf ("������� ������� ������ ���� >= 0\n");
            return -1;
        }
            //std::cout<<5;//getValue(token);
        std::cout<<(int) getValue(token)<<std::endl;


        return (int) getValue(token);

                    //temp2.from = atoi(token)


    }
}

void AddArray (const char* name)
{
    int from, to;

    get_token();  //��������� ��������� ����� "array"

    if ( tok != ARRAY )
	{
		printf ("Expected 'ARRAY'");
		return;
	}

	get_token(); //��������� "["

	if ( token[0] != '[' )
	{
		printf ("Expected '['");
		return;
	}

	//get_token(); //��������� ������� �������

	/*
	if (token_type == INTEGER)
        if (atoi(token) >= 1)

            std::cout<<token;
            //return atoi(token);
            //temp2.from = atoi(token);
     //   else*/
      //  {
      //      printf ("������� ������� ������ ���� >= 1\n");
      //      return;// -1;
      //  }

	from = ReadFromTo (); //���������� ������ ������� �������
	cout<<"�� ��� ��� ����!";
	//std::cout<<from;

    get_token(); //��������� ����������� ������ ������� ".."
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		printf ("Expected '..'");
		return;
	}

	get_token();
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		printf("Expected '..'");
		return;
	}

	to = ReadFromTo (); //���������� ������� ������� �������
//	std::cout<<to;

    if (to <= from)
    {
        printf("������� ������� ������� ������ ���� ������ ������");
		return;
    }
    else
    printf ("��� ��!");
    cout<<"�� ��� ��� ����!";
    return;
}

#endif // VARS&ARRAYS_H_INCLUDED
