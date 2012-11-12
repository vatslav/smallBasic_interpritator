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
			  printf("get_exp: before level2: token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();

  if( !*token )
  {
    serror(2);
    return;
  }
  level2( result );
			printf("get_exp: after level2, finish: result = %d; token = %s; token_type = %d; tok = %d\n", *result, token, token_type, tok );getchar();
  putback(); /* Возвращает последнюю считанную лексему обратно во входной поток */
}

/* Сложение или вычитание двух термов */
void level2( int* result )
{
  register char op;
  int hold;
			  printf("level2: before level3: token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();
  level3(result);

  while( ( op = *token) == '+' || op == '-' )
  {
    get_token();
			  printf("level2: before processing second operand: token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();
    level3( &hold );
			  printf("level2: after processing next operand: hold = %d, token = %s; token_type = %d; tok = %d\n", hold, token, token_type, tok );getchar();
    arith( op, result, &hold );
  }
			printf("level2: finish: result = %d; token = %s; token_type = %d; tok = %d\n", *result, token, token_type, tok );getchar();
}

/* Вычисление произведения или частного двух факторов */
void level3( int* result )
{
  register char op;
  int hold;
			  printf("level3: before level4: token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();
  level4( result );
   while( ( op = *token) == '*' || op == '/' || op == '%' )
  {
    get_token();
			  printf("level3: before processing next operand: token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();
    level4( &hold );
			  printf("level3: after processing next operand: hold = %d, token = %s; token_type = %d; tok = %d\n", hold, token, token_type, tok );getchar();
    arith( op, result, &hold );
  }
			printf("level3: finish: result = %d; token = %s; token_type = %d; tok = %d\n", *result, token, token_type, tok );getchar();
}

/* Обработка степени числа ( целочисленной ) */
void level4( int* result )
{
	int hold;
			  printf("level4: before level5: token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();
	level5( result );
	if( *token == '^' )
	{
		get_token();
		level4( &hold );
		arith( '^', result, &hold );
	}
			printf("level4: finish: result = %d; token = %s; token_type = %d; tok = %d\n", *result, token, token_type, tok );getchar();
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
			  printf("level5: before level6: token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();
	level6( result );

	if( op )
		unary( op, result );
			printf("level5: finish: result = %d; token = %s; token_type = %d; tok = %d\n", *result, token, token_type, tok );getchar();
}

/* Обработку выражения в круглых скобках */
void level6( int* result )
{
	if( ( *token == '(' ) && ( token_type == DELIMITER ) )
	{
		get_token();
			  printf("level6: before processing (...): token = %s; token_type = %d; tok = %d\n", token, token_type, tok );getchar();
		level2( result );
		if ( *token != ')' )
			serror(1);
		get_token();
			  printf("level6: after processing (...): result = %d, token = %s; token_type = %d; tok = %d\n", *result, token, token_type, tok );getchar();
	}
	else
		primitive( result );
}

/* Определение значения переменной по её имени */
void primitive( int* result )
{
						printf("primitive begin: token=%s; type=%d\n", token, token_type );getchar();
	switch( token_type )
	{
		case VARIABLE:
			*result = find_var( token );
			get_token();
						printf("primitive finish: result = %d; token=%s; type=%d\n", *result, token, token_type );getchar();
			return;
		case NUMBER:
			*result = atoi( token );
			get_token();
						printf("primitive finish: result = %d; token=%s; type=%d\n", *result, token, token_type );getchar();
			return;
		default:
			serror( 0 ) ;
	}
}

/* Выполнение специфицированной арифметики */
void arith( char o, int* r, int* h )
{
	register int t, ex;
						printf("arith: result %c hold = %d %c %d ", o, *r, o, *h );
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
						printf("= %d\n", *r );getchar();
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
	printf("expression: %s\n", prog);

	int result;
	get_exp( &result );
	printf( "result = %d",  result );//Увы, указатель prog изменяется,
	//посему после прогона анализатора мы не можем из него получть исходное выражение.

	return 0;
}


