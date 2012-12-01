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


/**@struct array
 ��������� ��� ���������� ���� INTEGER/REAL
int type - ��� ��������������
float value - �������� ��������������
bool isConst - ���������/����������
*/
struct Var
{
    int type;
    float value;
    bool isConst;
};

/**@struct array
��������� ��� �������� ���� INTEGER/REAL
int type - ��� �������
bool isBig - �����������/���������� ������
int from - ������ ������� �������
int to - ������� ������� �������
float *el - �������� �������
*/
struct Array
{
    int type;
    bool isBig;
    int from;
    int to;
    int from2;
    int to2;
    float *el; //��������� �� 1 ������� �������
    float **el2;
};

//��� �������� ����������/�������� � �������� ����������
//������������� ������� (�����/����������)
///����� ��� ��������������� ���� INTEGER/REAL
map <string, Var> Vars;
///����� ��� �������� ���� INTEGER/REAL
map <string, Array> Arrays;

///��������� ��� ������ � ������� � �����
map <string, Var>::iterator start = Vars.begin ();
map <string, Array>::iterator start2 = Arrays.begin ();

//��������� ������� ����� �������� ������ �.�. ���������� ���������
//Var temp;
//Array temp2;

/**@fuction AddVar
������� ���������� ����������/��������� � �����

name - ��� ����������/���������
type - ��� ����������/���������
value - �������� ���������/���������
constant - �������������� (true - ���������, false - ����������)
*/
bool AddVar (char* name, int type, float value, bool constant)
{
   Var temp;
   if (Vars.count(name) == 0) //���� � ����� ��� ���������� � ����� ������
   {
       temp.isConst = constant;
       temp.type = type;
       temp.value = value; //�������� � ���� INTEGER
       Vars[name] = temp;   //������ � ����� ����� ���������/���������
       return 1;
   }
   else //����� ��������, ��� � ����� ���� ����������/��������� � ����� ������
       {
           printf ("The variable/constant with this name is already exists");
           return 0;
       }
}

/**@fuction GetTypeVar
������� ��������� ���� ����������/���������

name - ��� ����������/���������
return - ��� ����������/���������
*/
int GetTypeVar (const char* name)
{
    if (Vars.count(name) != 0) //������� ����������/��������� � �����
    {
        //start->second = Vars[name]; //������ �������� �� ���� �������
        //return start->second.type;  //���������� ���
        return Vars[name].type;
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

/**@fuction getValue
������� ��������� �������� ����������/���������

name - ��� ����������/���������
return - �������� ����������/���������
*/
float getValue (const char* name)
{
    if (Vars.count(name) != 0) //������� ����������/��������� � �����
    {
        //start->second = Vars[name]; //������ �������� �� ���� �������
        //return start->second.value;  //���������� ��������
        return Vars[name].value;
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        return -1.0;
    }
}
 ///�������� �������� ����������� �������
float getValue (const char* name, int index)
{

    if (Arrays.count(name) != 0) //������� ����������/��������� � �����
    {
        //start->second = Vars[name]; //������ �������� �� ���� �������
        //return start->second.value;  //���������� ��������
        if (index > Arrays[name].to or index<Arrays[name].from)
          throw "������ �������, ������� �� ��� �������";
        return Arrays[name].el[index-Arrays[name].from];
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        return -1.0;
    }
}

 ///�������� �������� ���������� �������
float getValue (const char* name, int index, int index2)
{

    if (Arrays.count(name) != 0) //������� ����������/��������� � �����
    {
        //start->second = Vars[name]; //������ �������� �� ���� �������
        //return start->second.value;  //���������� ��������
        if (index > Arrays[name].to or index<Arrays[name].from or index2 > Arrays[name].to2 or index2<Arrays[name].from2)
          throw "������ �������, ������� �� ��� �������";
        return Arrays[name].el2[index-Arrays[name].from][index2-Arrays[name].from2];
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        throw "�������� ��� �������";
    }
}

/**@fuction isConst
������� �������� �� ���������

name - ��� ����������/���������
return - �������������� (true - ���������, false - ����������)
*/
bool isConst (const char* name)
{
    if (Vars.count(name) != 0) //������� ����������/��������� � �����
    {
        //start->second = Vars[name]; //������ �������� �� ���� �������
        //return start->second.isConst; //���������� �������������� (true - ���������, false - ����������)

        return Vars[name].isConst;
    }
    else //����� ��������� � ���, ��� �� ����� ������ � ����� ������
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

/**@fuction fuction
������� ������ ���������� ���� ���� �� ����� (��� �����)*/
void printVars ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = Vars.begin (); start != Vars.end (); start++)

        if (start->second.type == INTEGER)
            printf ("%s\t\t%.0f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );
        //std::cout<<start->first.c_str()<<"\t\t"<<start->second.value<<"\t\t"<<GetTypeVar (start->first.c_str() )<<" - "<<typeToName(GetTypeVar (start->first.c_str() ))<<"\n";
        else if (start->second.type == REAL)
            printf ("%s\t\t%5.3f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );

}

void printArrays()
{
   /*
    printf ("Name\t\tValue\t\tType\n\n");
    for (start2 = Arrays.begin (); start2 != Arrays.end (); start2++)
      if (start2->second.isBig == false)
        {
          if (start2->second.type == INTEGER)
          {*/
             //start2->first = "a";
//              cout<<start2->first<<endl;
//            for (int i=0;start2->second.to-start2->second.from; i++)
//              //printf ("%s\t\t%.0f\t\t%d - %s\n", start2->first.c_str(), getValue(start2->first.c_str()), GetTypeVar (start2->first.c_str() ), typeToName(GetTypeVar (start2->first.c_str() )) );
//                cout<<"|"<<start2->second.el[i]<<"|";
//            cout<<endl;
      /*    }
        //std::cout<<start2->first.c_str()<<"\t\t"<<start2->second.value<<"\t\t"<<GetTypeVar (start2->first.c_str() )<<" - "<<typeToName(GetTypeVar (start2->first.c_str() ))<<"\n";
        else if (start2->second.type == REAL)
            printf ("%s\t\t%5.3f\t\t%d - %s\n", start2->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );
        }*/
    for (int i=0;i<Arrays["a"].to-Arrays["a"].from;i++)
        cout<<Arrays["a"].el[i]<<"|";



}


/**@function ReadFromTo
������ ������ ������� ������� (�����)*/
int ReadFromTo ()
{
    get_token(); //��������� ������� �������
    ///���� ������ - �����
	if (token_type == INTEGER)
    {
        if (atoi(token) < 0)
        {
            printf ("������� ������� ������ ���� >= 0\n");
            return -1;
        }

        else
        {
            //cout<<atoi(token)<<endl;
            return atoi(token);
        }
        //temp2.from = atoi(token);
    }
    ///���� ������� - ����������
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
        //std::cout<<"call "<<(int) getValue(token)<<std::endl;

        return (int) getValue(token);

                    //temp2.from = atoi(token)

//
    }
}


/**@function AddArray ��������� ���������� � ����� ����������
*/
int AddArray (const char* name)
{
   int from, to, from2, to2;
   Array temp;

   temp.isBig = false;// ���������� ������, �� ���������

    get_token();  //��������� ��������� ����� "array"
    pres(int(tok));
    if ( tok != ARRAY )
	{
		printf ("Expected 'ARRAY'");
		return ERROR;
	}

	get_token(); //��������� "["
   pres(token);
	if ( token[0] != '[' )
	{
		printf ("Expected '['");
		return ERROR;
	}

	from = ReadFromTo (); //���������� ������ ������� �������
	pres(from);
	if ( from == -1 )
		return ERROR;


   get_token(); //��������� ����������� ������ ������� ".."
	pres(token);
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		printf ("Expected '..'");
		return ERROR;
	}

	get_token();
	pres(token);
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		printf("Expected '..'");
		return ERROR;
	}

	to = ReadFromTo (); //���������� ������� ������� �������
	pres(to);
	if ( to == -1 )
		return ERROR;

    if (to <= from)
    {
        printf("������� ������� ������� ������ ���� ������ ������");
        return ERROR;
    }

    get_token();
    pres(token,"token");
    //���� ������ ���������
	if( token_type == DELIMITER && token[0] == ',' )
	{

        from2 = ReadFromTo (); //���������� ������ ������� �������
        pres(from2);
        if ( from2 == -1 )
            return ERROR;

        get_token(); //��������� ����������� ������ ������� ".."
        pres(token);
        if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
        {
            printf ("Expected '..'");
            return ERROR;
        }

        get_token();
        pres(token);
        if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
        {
            printf("Expected '..'");
            return ERROR;
        }

        to2 = ReadFromTo (); //���������� ������� ������� �������
    	pres(to2);
    	//std::cout<<to2<<endl;
        if ( to2 == -1 )
            return ERROR;

        if (to2 <= from2)
        {
            printf("������� ������� ������� ������ ���� ������ ������\n");
            return ERROR;
        }
        //���� ���������� ��������� ������ :)
        temp.to2 = to2;
        temp.from2 = from2;
        temp.isBig = true;
        temp.to = to;
        temp.from = from;
        temp.el2 = new float* [to-from];
        for (int i=0; i<to-from; i++)
            {
            temp.el2[i] = new float [to2-from2];
            //���� �������, �� ��������� ���������, ����� ������
            if (testing)
                for (int j=0; j<to-from;j++)
                    temp.el[j]=j;
            else
                for (int j=0; j<to-from;j++)
                    temp.el[j]=0;

            memset(temp.el2[i], 0, to2-from2);
            }
        Arrays[name] = temp;

        get_token(); //��������� "["
        pres(token);
    }

    if ( ( (token_type==DELIMITER) && (token[0]==']') ) )
    {
        get_token();
        pres((int)tok);
		if ( (int)tok != OF )
		{
			serror("Expected 'of'\n");
			return ERROR;
		}

		get_token();
		pres((int)token_type);
		//if( token_type == ERROR ) return false;
		if ( (int)token_type == ERROR )
		{
		    serror("Expected 'INTEGER' or 'REAL'\n");
			return ERROR;
		}
		if ( (int)token_type == INTEGER )
		{
		    temp.type = INTEGER;
		}
      else
		{
		    temp.type = REAL;
		}

       //����  ���������!
       if (!temp.isBig)
       {
           temp.from = from;
           temp.to = to;
           temp.el = new float [to-from];
           //���� �������, �� ��������� ���������, ����� ������
           if (testing)
            for (int i=0;i<to-from;i++)
                for (int j=0; j<to-from;j++)
                    temp.el[j]=j;

            else
                for (int j=0; j<to-from;j++)
                    temp.el[j]=0;
           Arrays[name] = temp;

       }




    }
    else
    {
        serror("Expected ']'\n");
        return ERROR;
    }
    return 1;
}

#endif // VARS&ARRAYS_H_INCLUDED
