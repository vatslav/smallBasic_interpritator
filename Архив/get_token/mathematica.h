#ifndef MATHEMATICA_H_INCLUDED
#define MATHEMATICA_H_INCLUDED

//#pragma once //���� ����� ��� �����


//#include "global_defines.h"
//#include "globals.h"
//#include "get_token.h"
//#include "variables.h"
#include <math.h>


////�������������� ���������.

/*

��������� = ���� [ + ��� ][- ����]
���� = ������ [ * ������ ][ / ������ ][ div ������ ][ mod ������ ]
������ = [+|-]��������
�������� = ����������|�����|(���������)

*/


bool level2(struct variable*), level3(struct variable*), level4(struct variable*), level5(struct variable*), unary( char, struct variable* ), arith( char, struct variable*, struct variable*), primitive( struct variable * );
//leveli: ��� ������ ������� �������� ������ ���� ������ ������� �������������� ����� ���������.
//��� �������� ���������� ������� �������� ����� ��������� ������� ����� �������������� ����� ���������



//��� ������ ������� �������� ������ ���� ������ ������� ���������.
//��� �������� ���������� ������� �������� ��������� ������� ���������.
bool get_exp_abstract( struct variable*  );


bool ExecBuiltInMath( struct variable* );

/* ����� ����� � ����������. */

float get_exp_as_real( bool *ok )
{
	struct variable result;//������-�� ���� �������� ���������, �� �� ��� ��� ������� get_exp_abstract
	bool good = get_exp_abstract( &result );
	if(!good)
	{
		*ok = false;
		return 0;
	}

	*ok = true;

	if ( result.type == INTEGER )
		return (float)(*(int*)(result.var));
	else if (result.type == REAL )
		return *(float*)(result.var);
	else
	{
		serror("get_exp_as_real::Expected INTEGER or REAL in struct variable");
		*ok = false;
		return 0;
	}

}

//��� ����� ���������� ����������
int get_exp_as_integer( bool *ok )
{
	struct variable result;//������-�� ���� �������� ���������, �� �� ��� ��� ������� get_exp_abstract
	bool good = get_exp_abstract( &result );
	if(!good)
	{
		*ok = false;
		return 0;
	}

	*ok = true;

	if ( result.type == INTEGER )
		return (*(int*)(result.var));
	else if (result.type == REAL )
	{
		serror("Expected INTEGER result, got REAL");
		*ok = false;
		return 0;
	}
	else
	{
		serror("get_exp_as_integer::Expression result is neither INTEGER nor REAL");
		*ok = false;
		return 0;
	}


}


//������� �������� ��������� �� �� ����� ���������� � ���������� ����������� ������. �� �� �����.
void pass_exp_as_real( bool *ok )
{
	*ok = true;
	get_token();
	if ( strchr( ",.:;", token[0] ) ||
		token_type == OPERATOR || token_type == STRING || token_type == FINISHED ||
			token_type == ERROR || ( token_type == RESERVED && tok!=TRUE && tok != FALSE ) )
	{
		serror("Bad expression");
		*ok = false;
		return;
	}

	do
	{
		get_token();
	}
	while( !( strchr( ",.:;", token[0] ) ||
		token_type == OPERATOR || token_type == STRING || token_type == FINISHED ||
			token_type == ERROR || ( token_type == RESERVED && tok!=TRUE && tok != FALSE ) ) );
	putback();
}
void pass_exp_as_integer(bool*ok)
{
	pass_exp_as_real( ok );
}

void pass_logic_exp( bool*ok )
{
	pass_exp_as_real( ok );
}


//����������� �������� ���������. ����� �������� ��������� �� ������������ ( ������ ) ��������� variable,
// � ������� � ����� ������� ���������.
bool get_exp_abstract( struct variable* result )
{

  result -> var = 0;//���� �� ������ ��� ���������� free ��� ��� ���.
  get_token();

  if( !*token )
  {
    serror("Expected lexem");
    return false;
  }
  bool ok = level2( result );
  if(!ok) return false;
  putback(); /* ���������� ��������� ��������� ������� ������� �� ������� ����� */
  return true;
}

/* �������� ��� ��������� ���� ������ */
bool level2( struct variable* result )
{
  register char op;
  struct variable hold;hold.var = 0;//�������� ��������� ����� �����������, � �� ������ ���������.

  bool ok = level3( result );
  if(!ok) return false;

  while( ( op = *token) == '+' || op == '-' )
  {
    get_token();
    ok = level3( &hold );
	if(!ok) return false;

    //printf("result=%d, hold=%d\n", *(int*)(result->var), (hold.type) );
    //fflush(stdout);

    ok = arith( op, result, &hold );
   	if(!ok) return false;
  }

  return true;
}

/* ���������� ������������ ��� �������� ���� ��������. ��� �������������� ������� � ������� �� �������. */
bool level3( struct variable* result )
{
  register char op;
  struct variable hold;hold.var = 0;

  bool ok = level4( result );
  if( !ok ) return false;

  while( ( op = *token) == '*' || op == '/' || ( token_type == OPERATION && (op == 'd' || op == 'm')) )//d, m - div, mod
  {
    get_token();
    ok = level4( &hold );
    if ( !ok ) return false;

    ok = arith( op, result, &hold );
    if( !ok ) return false;

  }

  return true;
}



/* ������� + ��� - */
bool level4( struct variable* result )
{
	register char op;

	op = 0;
	if ( ( token_type == OPERATION ) && ( *token == '+' || *token == '-' ) )
	{
		op = *token;
		get_token();
	}
	bool ok = level5( result );
	if( !ok ) return false;

	if( op )
		ok = unary( op, result );
	if ( !ok ) return false;

	return true;
}

/* ��������� ��������� � ������� ������� ��� ��������� �����. */
bool level5( struct variable* result )
{
	bool ok;
	if( ( *token == '(' ) && ( token_type == DELIMITER ) )
	{
		get_token();
		ok = level2( result );
		if ( !ok ) return false;

		if ( *token != ')' )
		{
			serror("Expected ')'");
			return false;
		}
		get_token();
	}
	else
	{
		ok = primitive( result );
		if(!ok) return false;
	}

	return true;
}

/* ����������� �������� ���������� �� � ����� */
bool primitive( struct variable* result )
{
	//printf("primitive: token=%s; type=%d\n", token, token_type );//111111111
	switch( token_type )
	{
		case VARIABLE:
		//getNumberFromVariable �� ���������� variables �� ����� ���������� ����������
		//��������� variable, ���������� ��� � �������� ����������, ��� ������� �������������
		//��������� ���������� ������ � ���� ��� ������� ������� ( �������� �������, ������� ���� ), ���������� ��� ��������, �����
		//���������� ������ ��������� variable.
			*result = getNumberFromVariable( token );
			if ( result->type == 0 ) return false;//���� �� ������� �������� ��������.
			//����� ����� ��������, ��� ���������� � ����� ������ ����� ���������� ( ����� �������� �� get_token, �� ���
			//�������, ��� ��� VARIABLE ). ������ ������������ ����������� ��������� �������� - ������������ ����������
			//�������� ��� ������������ �������� �������� ��� ����������-�������.
			get_token();
			return true;
			break;
		case CONSTANT:
			*result = getNumberFromConstant( token );
			//��� get_token �������, ��� ��� CONSTANT, ������ ����� ��������� ����� ����. � ��� ������, ��� ������ ����� �� �����.
			//��� �� ������ � �� ���������.
			//!!!���������� ��������� �� ���� ������� �� ������� � ���, ��� �������� � ���������� variables.
			//!!!��������� var ��������� �� ������� ������, ��� ����� ����� �������� �� ���������� variables.
			//������ ���? �� ������, ��� ���� �� result.var �������� �� ������� ������, ��� �������� ������ ��������
			//���������� ( � �� ����� �������� ), ������� arith ��� �������� �� ����������. � ����� ������ �������� �����
			//������...
			get_token();
			return true;
			break;
		case INT_NUMBER:
			result -> type = INTEGER;
			int* d = (int*)malloc( sizeof( int ) );
			*d = atoi( token );
			result -> var = d;

			get_token();

			return true;
			break;
		case REAL_NUMBER:
			result -> type = REAL;
			float* dd = (float*)malloc( sizeof( float ) );
			*dd = atof( token );
			result -> var = dd;

			get_token();

			return true;
			break;
		case FUNCTION:
			{
				//printf("primitive::FUNCTION\n");
				//fflush(stdout);
				bool ok = ExecBuiltInMath( result );

				if(!ok) return false;
				else return true;
			}
			break;
		default:
			serror( "Expected identifier or variable"  ) ;
			return false;
	}
}

////// ��������� ������� /////////
//���������� �������� ��� INTEGER
bool int_arith( char o, int* r, int* h )
{


	switch( o )
	{
		case '-':
			*r = *r - *h;
			break;
		case '+':
			*r = *r + *h;
			break;
		case '*':
			*r = *r * *h;
			break;
		//case '/': // ��� �������������� � ���������� ���������.
		case 'd':

			if ( *h == 0 )
			{
				serror("Division by 0");
				return false;
			}
			*r = (*r)/(*h);
			break;
		case 'm':

			*r = (*r) % (*h);
			break;
        default:
            serror("int_arith::unreachable section is reached");
            return false;
            break;
	}
	return true;
}

//���������� �������� ��� REAL
bool float_arith( char o, float* r, float* h )
{


	switch( o )
	{
		case '-':
			*r = *r - *h;
			break;
		case '+':
			*r = *r + *h;
			break;
		case '*':
			*r = *r * *h;
			break;
		case '/':
			if ( *h == 0 )
			{
				serror("Division by 0");
				return false;
			}
			*r = (*r)/(*h);
			break;
		case 'm':
			serror("Expected integer variables for 'mod'");
			return false;
			break;
		case 'd':
      serror("Expected intger variables for 'div'");
      return false;
      break;
        default:
            serror("float_arith::reached unreachable code.");
            return false;
            break;
	}

	return true;
}

//���������� ���� INTEGER � ���� REAL



/* ���������� ����������������� ����������.
��������� ������������ � r */
bool arith( char op, struct variable* r, struct variable* h )
{


	if ( r -> type != h-> type )//���� ����� ���������� �����.
	{
		struct variable* lowest;
		if( r->type == INTEGER ) lowest = r;
		else if ( h->type == INTEGER ) lowest = h;
		else
		{
			serror("arith::Expected struct variable with type = INTEGER or REAL" );
			return false;
		}

		lowest -> type = REAL;
		int d = *(int*)(lowest->var);
		free( lowest->var );
		lowest -> var = malloc( sizeof( float ) );
		*(float*)(lowest->var) = (float)d;

	}


	if ( r-> type == INTEGER )
	{
	    if ( op == '/' )
	    //��� ������� integer-�� � ������� �� ���������� ������������ ������� �����,
	    //� ���������� ������� � ���� ������ real.
	    {
	        r->type = REAL;
	        float result = ((float)(*(int*)(r->var))) / ( * (int*)(h->var));
	        free( r-> var );
	        r->var = malloc ( sizeof( float ) );
	        *((float*)(r->var)) = result;
	        return true;
	    }
	    else return int_arith( op, (int*)(r->var), (int*)(h->var) );
	}

	else if ( r -> type == REAL )
		return float_arith( op, (float*)(r->var), (float*)(h->var) );
	else
	{
		serror("arith::Expected struct variable with type = INTEGER or REAL (2)");
		return false;
	}


}

/* ��������� ����� */
bool unary( char o, struct variable* r )
{
	if ( o == '-' )
	{
		if ( r-> type  == INTEGER )
			*(int*)(r->var) = -*(int*)(r->var);
		else if ( r->type == REAL )
			*(float*)(r->var) = -*(float*)(r->var);
		else
		{
			serror("unary: Expected struct variable with type = INTEGER or REAL");
			return false;
		}
	}
	return true;
}




// ���������� �������������� �������bool ExecExp(), ExecLn(), ExecSin(), ExecSin();

bool ExecBuiltInMath( struct variable * result )
{

	char func = tok;//���������� ���������� ������������� ���������� �������������� �������.

	get_token();
	if( *token != '(' )
	{
		serror("Expected '('");
		return false;
	}

	bool ok;
	float arg = get_exp_as_real( &ok );
	if(!ok) return false;

	get_token();
	if( *token != ')' )
	{
		serror("Expected ')'");
		return false;
	}
	get_token();



	float r;
	switch( func )
	{
		case EXP:
			r = expf( arg );//f � ����� ����� ������� ��������, ��� ��� ���������� �������� ���� float. exp(arg) ���������� �� double. ! � C ��� ���������� ������!
			break;
		case LN:
			if ( arg <= 0 )
			{
				serror("ln: arg must be >0");
				return false;
			}
			r = logf( arg );
			break;
		case SIN:
			r = sinf( arg );
			break;
		case COS:
			r = cosf( arg );
			break;
		default:
			serror("Error in ExecBuiltInMath");
			return false;

	}



	result -> type = REAL;
	if ( result->var ) free( result -> var );

	result -> var = malloc( sizeof(float) );

	*(float*)(result -> var) = r;


	return true;

}



//// �����, ���������� �� ���������� ���������� ���������.
/*
���� �� ���������� ���������� �������������� ���������. ����� �� ����� �������� �� �����������������������

 ��������� = ���� [ or ���� ] --logic_level2
 ���� = ������ [ and ������ ]
 ������ = [not] ������2 -- logic_level4
 ������2 = ��������� | ( ��������� ) | True | False
 ��������� =  ����������������������� ��������� ����������������������� -- logic_level6
 ��������� = > | < | >= | <= | = | <>
*/



bool logic_level2( bool* ok ), logic_level3( bool *ok ), logic_level4( bool *ok ), logic_level5( bool * ok ), logic_level6( bool *ok );
//logic_leveli: ��� ������ ������� �������� ������ ���� ������ ������� �������������� ����� ���������.
//��� �������� ���������� ������� �������� ����� ��������� ������� ����� �������������� ����� ���������



//��� ������ ������� �������� ������ ���� ������ ������� ���������.
//��� �������� ���������� ������� �������� ��������� ������� ���������.
bool get_logic_exp( bool* ok)
{
	bool result;

	get_token();
	if ( !*token )
	{
		serror("Expected lexem");
		*ok = false;
		return false;
	}

	 result = logic_level2( ok );
	 putback();
	 return result;
}

//��������� or
bool logic_level2( bool* ok )
{
	bool result = logic_level3( ok );
	if ( *ok == false ) // ���� ������, �� � *ok ��� ������� ������� ������, �� �� ������ ����� �������� ��������� false
		return false;

	register char op = token[0];
	register bool hold;

	while ( op == 'o' && token_type == OPERATION )
	{
		get_token();
		hold = logic_level3(ok);
		if ( !*ok ) //���� ������, ������
			return false;
		result = result || hold; //��� �� ������� �������� ���������
	}

	return result;

}

//��������� and
bool logic_level3( bool *ok )
{
	bool result = logic_level4( ok );
	if ( *ok == false ) // ���� ������, �� � *ok ��� ������� ������� ������, �� �� ������ ����� �������� ��������� false
		return false;

	register char op = token[0];
	register bool hold;

	while ( op == 'a' && token_type == OPERATION )
	{
		get_token();
		hold = logic_level4(ok);
		if ( !*ok ) //���� ������, ������
			return false;
		result = result && hold; //��� �� ������� �������� ���������
		//� �� ����� �� ���� �������, ��� result = false ��������? ���� 0 and 1 = 0.
		//���, �� �����, ��� ����� ����� �������, ��� ������� "��������" ������ ���������� ������������ �������.
	}

	return result;

}

//������ not
bool logic_level4( bool *ok )
{
	if ( token_type == OPERATION && token[0] == 'n' )
	{
		get_token();
		return !logic_level5( ok );
	}
	else
	{
		return logic_level5( ok );
	}
}

// �����������, ��� ������ ������: ���������� ��� ���������� ��������� ������������. ��� ��������� ��������� True � False
bool logic_level5( bool * ok )
{
	if ( token[0] == '(' && token_type == DELIMITER ) //���� ��������� ���������� ���������� ���������.
	{
		get_token();
		bool result = logic_level2( ok );
		if ( !*ok )
			return false;

		if ( !( token[0] == ')' && token_type == DELIMITER ) )
		{
			serror("Expected ')'");
			*ok = false;
			return false;
		}

		get_token();
		return result;
	}
	 // ���� �� ��������� ��������������� ���������� ���������.
	else if ( tok == TRUE )
	{
		*ok = true;
		get_token();
		return true;
	}
	else if (  tok == FALSE )
	{
		*ok = true;
		get_token();
		return false;
	}
	else
	{//����� ���� ��������� ���������.
		return logic_level6( ok );
	}
}

//������ ��������� ( A \ > | < | <> | = | >= | <= / B )
bool logic_level6( bool *ok )
{
	//���� ��� � ��� ���������� ������ ��������, ������ ��������� �����������.
	//??? ��� ������, ���� ������ ��������� ������ ��������� True ��� False ?
	float left, right;
	putback();//������� ����� ��� ������ ������� ��������������� �����.
	left = get_exp_as_real( ok );
	if ( !*ok )
		return false;// ��������, � ����� ���� ����� � ���,
		//��� ��� �� ������� �������� ������ ( ������� ������ ����� � *ok ),
		//������ ���� �� ������� �����-�� ��������. � ��� ���� �������� �������� �� �������.
	get_token();
	if ( ! ( token_type == OPERATION && ( strchr( "><=", token[0] ) ) ) )
	{
		serror("Expected comparing: >=, <=, >, <, <>, =.");
		*ok = false;
		return false;
	}
	char op[3] = {'\0','\0','\0'};
	strcpy( op, token );

	right = get_exp_as_real( ok );
	if ( !*ok )
		return false;

	*ok = true;

	get_token(); // ��� ����, ����� ������� ������������� ������������� � ��� �����������
	switch( op[0] )
	{
		case '=':
			return ( left == right );
			break;
		case '>':
			if ( op[1] )
				return ( left >= right );
			else
				return ( left > right );
			break;
		case '<':
			if ( op[1] == '>' )
				return ( left != right );
			else if ( op[1] == '=' )
				return ( left <= right );
			else
				return ( left < right );
			break;
		default:
			serror("Error in logic_level6::case");
			*ok = false;
			return false;
	}


}

#endif // MATHEMATICA_H_INCLUDED
