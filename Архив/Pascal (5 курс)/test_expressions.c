#include "expressions.h"
#include "variables.h"

#include <math.h>
int main()
{

	START_SUBSYSTEM_VARIABLES();


	prog = (char*)malloc( sizeof(char)*100 );


	/*
	//strcpy( prog, "(9+1)/(3-2)-(100+56.123)" );
	//strcpy( prog, " 2*exp(lN( 3 ))*2 ithen" );//������������ ��������� UnknownIdentifier ( �� ��� ���� ok = true ).
	//��� ����� ���������� variables ����� get_token �������� ��������, ��� �� ������������� ithen. � ����� �� ������)
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
	//������������ ��������� UnknownIdentifier ( �� ��� ���� ok = true ).
	//��� ����� ���������� variables ����� get_token �������� ��������, ��� �� ������������� and_so_on.
	//������ ������������ 2 ����? �������� expressions::logic_level6
	//( get_token ��������� �� "and_so_on" ������ get_exp_as_real � � get_token ����� switch )
	// � ����� �� ������) ��� � ������ ��������.

	bool ok, result;
	result = get_logic_exp( &ok );
	printf("ok = %d, result = %d\n", ok, result );
	printf("prog -> %s\n", prog );



	return 0;
}
