/// Тремя слешами закомментированы строки кода, которые сейчас только мешаются. В тексте же интерпретатора эти строки должны присутствовать.
/**/ /// Также некоторые элементы кода закомментированы.


#include "preambule.h"

extern char* prog; /*буфер анализируемого выражения*/
///extern jmp_buf e_buf; /* буфер среды функции longjmp() */
extern int variables[26]; /* переменные */
//extern struct commands{
    //char command[20];
    //char tok;
//};
extern commands table[];


extern char token[80]; /* внешнее представление лексемы*/
extern char token_type; /* тип лексемы*/
extern char tok; /* внутреннее представление лексемы */

void get_exp( int* ), level2( int* ), level3( int* ), level4( int* ), level5( int* );
void level6( int* ), primitive( int* ), arith( char, int*, int* ), unary(char, int*);

/////ИЗ preambule.h/////////////
int find_var( char* );
void serror( int ), putback();
char get_token();
///////////////////////////////

/* Точка входа в анализатор. */
void get_exp( int* result )
{
  get_token();

  if( !*token )
  {
    serror(2);
    return;
  }
  level2( result );
  putback(); /* Возвращает последнюю считанную лексему обратно во входной поток */
}

/* Сложение или вычитание двух термов */
void level2( int* result )
{
  register char op;
  int hold;

  level3(result);
 
  while( ( op = *token) == '+' || op == '-' )
  {
    get_token();
    level3( &hold );
    arith( op, result, &hold );
  }
}

/* Вычисление произведения или частного двух факторов */
void level3( int* result )
{
  register char op;
  int hold;

  level4( result );
   while( ( op = *token) == '*' || op == '/' || op == '%' )
  {
    get_token();
    level4( &hold );
    arith( op, result, &hold );
  }
}

/* Обработка степени числа ( целочисленной ) */
void level4( int* result )
{
	int hold;
	level5( result );
	if( *token == '^' )
	{
		get_token();
		level4( &hold );
		arith( '^', result, &hold );
	}
}

/* Унарный + или - */
void level5( int* result )
{
	register char op;
	
	op = 0;
	if ( ( token_type == DELIMITER ) && *token == '+' || *token == '-' )
	{
		op = *token;
		get_token();
	}
	level6( result );
	
	if( op )
		unary( op, result );
}

/* Обработку выражения в круглых скобках */
void level6( int* result )
{
	if( ( *token == '(' ) && ( token_type == DELIMITER ) )
	{
		get_token();
		level2( result );
		if ( *token != ')' )
			serror(1);
		get_token();
	}
	else
		primitive( result );
}

/* Определение значения переменной по её имени */
void primitive( int* result )
{
	//printf("primitive: token=%s; type=%d\n", token, token_type );//111111111
	switch( token_type )
	{
		case VARIABLE:
			*result = find_var( token );
			get_token();
			return;
		case NUMBER:
			*result = atoi( token );
			get_token();
			//printf("\ntoken = %s; type = %d\n", token, token_type);//1111
			return;
		default:
			serror( 0 ) ;
	}
}

/* Выполнение специфицированной арифметики */
void arith( char o, int* r, int* h )
{
	register int t, ex;
	
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
			*r = (*r)/(*h);
			break;
		case '%':
			t = (*r)/(*h);
			*r = *r - ( t * (*h) );
			break;
		case '^':
			ex = *r;
			if ( *h == 0 )
			{
				*r = 1;
				break;
			}
			for( t = *h - 1; t > 0; --t ) *r = (*r) * ex;
			break;
	}
}

/* Изменение знака */
void unary( char o, int* r )
{
	if ( o == '-' )
		*r = -(*r);
}


int main()
{
	prog = new char[100];
	strcpy( prog, "9/3-(100+56)"/*" 2 + 2 *	2 "*/ );
	int result;
	get_exp( &result );
	printf( "result = %d",  result );//Увы, указатель prog изменяется, 
	//посему после прогона анализатора мы не можем из него получть исходное выражение.

	return 0;
}


