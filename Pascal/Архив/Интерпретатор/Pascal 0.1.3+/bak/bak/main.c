//#include "preambule.h"
#include "get_token.h"

extern char* prog; /*буфер анализируемого выражения*/

int main()
{
	prog =  (char*)calloc( sizeof( char ) , 100 );
	strcpy( prog, 
	
	"program FirstProgram;\r\n"
	"var a,i:integer;\r\n"
	"begin\r\n"
	"writeln( 'Таня не спи', 111.78 );\r\n"
	"for i:=1 to 10 do writeln(i);\r\n"
	"write( 'Пока' );\r\n"
	"readln( a );\r\n"
	"a:= a + 1;\r\n"
	"end."
	);
	printf("%s\n", prog );
	//scanf("%s", prog );
	while( 1 )
	{
		get_token();
		printf("token_type = '%d', token = '%s', tok = '%d'\n", token_type, token, tok );

		getchar();
	}

	return 0;
}
