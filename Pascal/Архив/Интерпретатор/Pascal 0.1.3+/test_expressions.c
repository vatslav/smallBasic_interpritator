#include "expressions.h"
#include "variables.h"

#include <math.h>
int main()
{

	START_SUBSYSTEM_VARIABLES();


	prog = (char*)malloc( sizeof(char)*100 );


	/*
	//strcpy( prog, "(9+1)/(3-2)-(100+56.123)" );
	//strcpy( prog, " 2*exp(lN( 3 ))*2 ithen" );//Вываливается сообщение UnknownIdentifier ( но при этом ok = true ).
	//Его выдаёт подсистема variables когда get_token пытается выяснить, что за идентификатор ithen. В общем всё хорошо)
	//strcpy(prog, "ln(-2) ");
	//strcpy(prog, "ln(2) mod 4");

	struct variable result;
	bool ok = get_exp_abstract( &result );

	printf("ok = %d\n", ok );

	if ( result.type == INTEGER )
		printf( "result = %d\n",  *(int*)(result.var) );
	else if ( result.type == REAL )
		printf( "result = %f\n", *(float*)(result.var) );
	else printf("type ERROR\n");
*/


	addSimpleVariable( "abra", INTEGER );
	addSimpleVariable( "b", REAL );


	int* a = getIntegerVariable("abra");
	float* b = getRealVariable("b");

	*a = 78;
	*b = 3.14;



	strcpy( prog, " (abra >= b*abra ) or  True and 2+2.98 = 4.0 + 0.98 and_so_on" );
	//Вываливается сообщение UnknownIdentifier ( но при этом ok = true ).
	//Его выдаёт подсистема variables когда get_token пытается выяснить, что за идентификатор and_so_on.
	//Почему вываливается 2 раза? Смотрите expressions::logic_level6
	//( get_token переходит на "and_so_on" внутри get_exp_as_real и в get_token перед switch )
	// В общем всё хорошо) Так и должно работать.

	bool ok, result;
	result = get_logic_exp( &ok );
	printf("ok = %d, result = %d\n", ok, result );
	printf("prog -> %s\n", prog );



	return 0;
}
