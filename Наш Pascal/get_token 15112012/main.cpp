#include <iostream>
#include <locale.h>
#include "get_token.h"
#include "Vars&Arrays.h"

using namespace std;

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
	  ///���� ������ ��� ����� ���������.
		if (result==9 || result==18)
		{
		  //int a;
		  break;

		}
		printf("%s \t\t%d - %s \t\t%d - %s", token, token_type,typeToName(token_type), tok,typeToName(tok) );
		getchar();
	}*/
	AddVar ("_sgse3",INTEGER,25,false);
	AddVar ("sesgse3",INTEGER,5,false);
	PrintMap ();

    return 0;
}
