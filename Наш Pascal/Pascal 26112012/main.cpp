#include <iostream>
#include <locale.h>
#include "get_token.h"
#include "Vars&Arrays.h"
//##include "expressions.h"

using namespace std;
/**@function main �� ������ ������ ������ ��� ������� ���� ��������� �������
������������ ������ ������ �������� �� ����� � ��������� ����������� ��������� ���������
@return 0 ���� ��������� ����������� �������, ����� ��� ������.*/
int main()
{
    setlocale (LC_ALL,"RUS");
   /* prog = "Program Test\n"
    "c:=3 "
    "const a=5, b=3;\n"
    "var\n"
    "i,j:integer;\n"
    "y:array [1..a,1..b] of integer;\n"
    "Begin\n"
    "    write (a);\n"
    "    read (k);\n"
    "{ �����������     \n"
    "    �����������  }\n"
    "    if a>=b then i:=a+1 else j:=b+1\n"
    "    for i:=1 to a do i:=i*j;\n"
    "    while (i>0) i:= i - 2\n"
    "End.";
    //" (1243+236)bsg+43.36";
    printf("%s\n\nToken\t\tToken_type\tTok\n\n",prog);
    while(1)
	{
	  int result = get_token();
	  ///���k� ������ ��� ����� ���������.
		if (result==9 || result==18)
		{
		  //int a;
		  break;

		}
		printf("%s \t\t%d - %s \t\t%d - %s", token, token_type,typeToName(token_type), tok,typeToName(tok) );
		getchar();
	}*/
	AddVar ("_s",INTEGER,10,true);
	//AddVar ("sesgse3",INTEGER,5,false);


   //AddVar ("se3",REAL,112.75,false);
	//AddVar ("awf3",REAL,0.53,false);

	//PrintMap ();
    prog = "array [_s..35] of integer;";
    AddArray ("a");

    //getchar();

    return 0;
}
