#include "variables.h"



extern struct context GLOBALS;//Из подсистемы variables. Глобальный контекст.
/* Инициализация интерпретатора */
void InitInterpreter()
{
	START_SUBSYSTEM_VARIABLES();//запуск подсистемы хранения переменных
}


/*

Напишу замечание относительно работы с подсистемой variables вкупе с get_token

get_token приводит все считанные лексемы к нижнему регистру. А подсистема variables
при хранении переменных учитывает регистр.

Это стоит учитывать. В дальнейшем в подсистему variables будут передаваться строки,
полученные при помощи get_token(). А эти строки автоматически в нижнем регистре.


*/

int main( int argc, char* argv[] )
{
	InitInterpreter();


	/*addSimpleVariable( "hello", INTEGER );
	addSimpleVariable( "a", REAL );
//	debug();

	char type = getVariableType( "hello");

	int* d = getIntegerVariable("hello");
	float* f = getRealVariable("a");


	//printf("%p %p\n", d, f );

	*f = 3.14;
	*d = 35;


	//getVariableType("hjl");
	//printf("%d %f\n", *getIntegerVariable("hello"), *getRealVariable("a") );

	debug();



	addSimpleVariable( "z", REAL );
	addSimpleVariable("m809", ARRAY );

	f = getRealVariable("z");
	*f = 2.72;
	f = getRealVariable("z");
	//printf("%p %f\n", f, *f );

	debug();


	printf("%d %d %d\n",
		getTokenTypeForIdentifier("z"),
		getTokenTypeForIdentifier("m809"),
		getTokenTypeForIdentifier("4tr45") );
		///getVariableType( "z") ==
    /// == getTokenTypeForIdentifier("z") ???;
*/

    char* example = "b:array[1..10, -100..-45] of real";
    prog = example+2;
    addArrayVariable("b");
    debug();

    char type = getArrayBaseType( "b" );
    printf("array basetype = %d\n", type);


	return 0;
}
