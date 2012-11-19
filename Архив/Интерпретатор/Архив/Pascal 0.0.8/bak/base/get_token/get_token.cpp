
#include "preambule.h"

extern char* prog; /*буфер анализируемого выражения*/
///extern jmp_buf e_buf; /* буфер среды фуункции longjmp() */
extern int variables[26]; /* переменные */
//extern struct commands{
    //char command[20];
    //char tok;
//};
extern commands table[];


extern char token[80]; /* внешнее представление лексемы*/
extern char token_type; /* тип лексемы*/
extern char tok; /* внутреннее представление лексемы */



/* Чтение лексемы */
char get_token()
{
	register char *temp;
	token_type = 0; tok = 0;
	temp = token;

	if( *prog == '\0' )
	{
		/* Конец файла */
		*token = 0;
		tok = FINISHED;
		return( token_type = DELIMITER );
	}
	while( iswhite(*prog) ) ++prog; /* Пропуск пробелов */

	if( *prog == '\r' )
	{
		/* конец строки программы */
		++prog; ++prog;
		tok = EOL; *token = '\r';
		token[1] = '\n'; token[2] = 0;

		return (token_type = DELIMITER );
	}

	if( strchr("+-*^/%=;(),<>", *prog ) )
	{
		/* Разделитель */
		*temp = *prog;
		prog++; /* Переход на следующу позицию */
		temp++;
		*temp = 0;
		return ( token_type = DELIMITER );
	}

	if ( *prog == '"' )
	{
		/* строка кавычек */
		prog ++;
		while( *prog != '"' && *prog != '\r' ) *temp++ = *prog++;
		if( *prog == '\r' ) serror(1);
		prog++; *temp = 0;
		return( token_type = QUOTE );
	}

	if( isdigit(*prog) )
	{
		/* число */
		while( !isdelim( *prog ) ) *temp++ = *prog++;
		*temp = '\0';

		return( token_type = NUMBER );
	}

	if( isalpha( *prog ) )
	{
		/* переменная или команда */
		while( !isdelim( *prog ) ) *temp++ = *prog++;
		token_type = STRING;
	}

	*temp = '\0';
/* просматривается, если строка - переменная или команда */
	if ( token_type == STRING )
	{
		tok = look_up( token ); /* Преобразование во внутренний формат */ //Пока что таблица table не заполнена, посему look_up === 0
		if ( !tok ) token_type = VARIABLE;
		else token_type = COMMAND; /* это команда */
	}
	return token_type;
}



int main()
{
	prog =  new char[100];
	strcpy( prog, "Repeat a=a+b Until ;");
	//scanf("%s", prog );
	while( 1 )
	{
		get_token();
		printf("token_type = %d, token = %s, tok = %d\n", token_type, token, tok );

		getchar();
	}

	return 0;
}

