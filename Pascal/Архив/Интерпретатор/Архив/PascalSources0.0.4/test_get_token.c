#include <stdio.h>
#include "get_token.h"

int main()
{
	SimpleWork = true; // Пока другого режима работы get_token у нас нет.
	/*char* example =
	"VAR\n"
	"	i:INTEGER, r:real\n"
	"BEGIN\n"
	"i := 13;\n"
	"if i >= 4.8 then writeln( i );\n"
	"end.";
	*/
	char* example = 
	//"DELIMITER  . , ; : ( ) [ ] \n"
	//"RESERVED const var integer real array of true false\n"
	//"OPERATOR if then else while for to downto do repeat until := readln writeln read write begin end\n"
	//"OPERATION + - * / div mod < > >= <= <> = and or not \n"
	//"FUNCTION exp, ln, sin, cos\n"
	"VARIABLE a10 hello_45 _var\n"
	"CONSTANT tytytyt\n"
	"REAL_NUMBER 123.09 456.89 -78.8 \n"
	"INT_NUMBER 123 -678\n"
	"STRING 'test string' 'строчка символов !'\n"
	"ERROR 134abc 123.abc \n"
	"FINISHED";
	
	
	
	
	printf("%s\n", example );	
	prog = example;
	
	while( 1 )
	{
		get_token();
		printf("token='%s'; token_type=%d; tok=%d", token, token_type, tok );
		getchar();
	}
	
	
	return 0;
}
