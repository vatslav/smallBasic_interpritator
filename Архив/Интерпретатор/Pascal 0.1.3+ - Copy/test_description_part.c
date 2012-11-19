#include "variables.h"
#include "description_part.h"


int main()
{
	START_SUBSYSTEM_VARIABLES();

	char * example =
	"VAR\n"
	"a,b,c:integer;\n"
	"d:real;\n"
	"ar1:array[1..10] of real;\n"
	"ar2,ar3:array[1..56,5..67] of real;\n"
	"begin";
	prog = example;
	get_token();

	ReadVAR();
	debug();
	printf("prog = %s\n", prog );//Верно. Текущей лексемой является последняя лексема секции VAR, то есть prog
	//указывает на "begin"

	return 0;

}
