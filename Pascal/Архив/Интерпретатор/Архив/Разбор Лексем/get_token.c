
#include "preambule.h"

extern char* prog; /*буфер анализируемого выражения*/

//extern struct commands{
    //char command[20];
    //char tok;
//};
extern struct commands table[];


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

	if( strchr(".+-*/=;(),<>:", *prog ) )
	{
		/* Разделитель */
		*temp = *prog;


		bool assign = false;//Признак того, что быть может ожидается присваивание.
		if( *prog == ':' ) assign = true;//Если мы встретили двоеточие, вполне возможно, что это присваивание

		prog++; /* Переход на следующу позицию */
		temp++;

		if( assign && *prog == '=' )
		{
			*temp = *prog;
			prog++;
			temp++;
		}

		*temp = 0;
		return ( token_type = DELIMITER );
	}

	if ( *prog == '\'' )
	{
		/* строка кавычек */
		prog ++;
		while( *prog != '\'' && *prog != '\r' ) *temp++ = *prog++;
		if( *prog == '\r' ) serror(1);
		prog++; *temp = 0;
		return( token_type = QUOTE );
	}

	if( isdigit(*prog) )
	{
		/* число */
		while( isdigit( *prog ) ) *temp++ = *prog++;

		if( *prog == '.' ) // Если остановились на точке, то это дробное число
		{
			*temp++ = '.';
			prog++;
			while( isdigit( *prog ) ) *temp++ = *prog++;

			if( *(prog-1) == '.' ) // цикл ни разу не выполнился. На входе например 134.abc
			{
				*token = '\0';
				return ( token_type = ERROR );
			}
			if ( !isdelim( *prog ) )
			{
				*token = 0;
				return ( token_type = ERROR );
			}

		}
		else if ( !isdelim( *prog ) ) /// Если мы остановились не на разделителе, то введена кракозябра вида 123abc
		{
			*token = 0;
			return ( token_type = ERROR );
		}

		//Мы остановились на разделителе. Всё нормально. Это просто чилсо.


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
		if ( !tok ) token_type = IDENTIFIER;
		else token_type = COMMAND; /* это команда */
	}
	return token_type;
}



int main()
{
	prog =  (char*)calloc( sizeof( char ) , 100 );
	strcpy( prog,

	"program FirstProgram;\r\n"
	"var a,i:integer\r\n"
	"begin\r\n"
	"writeln( 'Vvedite chislo:', 111.78 );\r\n"
	"for i:=1 to 10 do writeln(i);\r\n"
	"write( 'Poka' );\r\n"
	"readln( a )\r\n"
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

