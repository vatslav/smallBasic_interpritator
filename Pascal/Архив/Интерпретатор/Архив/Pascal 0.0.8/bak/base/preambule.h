///#include "setjmp.h"
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

#define DELIMITER 1
#define VARIABLE 2
#define NUMBER 3
#define COMMAND 4
#define STRING 5
#define QUOTE 6
#define EOL 9
#define FINISHED 10

/*extern*/ char* prog; /*буфер анализируемого выражения*/
///extern jmp_buf e_buf; /* буфер среды функции longjmp() */
/*extern*/ int variables[26] =  /* переменные */ {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0 };

/*extern*/ struct commands{
    char command[20];
    char tok;
};
commands table[] = { {"", 0 } };
//{ /* Таблица обрабатывает комманды, введённые на нижнем регистре */
	//{ "print", PRINT },
	//{ "input", INPUT },
	//{ "if", IF },
	//{ "then", THEN },
	//{ "goto", GOTO },
	//{ "for", FOR },
	//{ "next", NEXT },
	//{ "to", TO },
	//{ "gosub", GOSUB },
	//{ "return", RETURN },
	//{ "end", END },
	//{"", END }
//};//!!!!!!

/*extern*/ char token[80]; /* внешнее представление лексемы*/
/*extern*/ char token_type; /* тип лексемы*/
/*extern*/ char tok; /* внутреннее представление лексемы */


////////////////////////STRING FUNCTIONS///////////////////////////////
/* Возвращает "истину", если "c" разделитель */
bool isdelim(char c)
{
	if( strchr(" ;,+-<>/*%^=()", c ) || c == 9 || c == '\r' || c == 0 )
		return true;
	else
		return false;
}

/* Возвращает 1, если "с" пробел или табуляция */
int iswhite( char c )
{
	if( c == ' ' || c == '\t' ) return 1;
	else return 0;
}



/* Выдать сообщение об ошибке*/
void serror( int error )
{
	static char *e[] = {
		/* 0*/ "Синтаксическая ошибка",
		/* 1*/ "Непарные круглые скобки",
		/* 2*/ "Это не выражение",
		/* 3*/ "Предполагается символ равенства",
		/* 4*/ "Не переменная",
		/* 5*/ "Таблица меток переполнена",
		/* 6*/ "Дублирование меток",
		/* 7*/ "Неопределённая метка",
		/* 8*/ "Необходим оператор THEN",
		/* 9*/ "Необходим оператор TO",
		/*10*/ "Уровень вложенности цикла FOR слишком велик",
		/*11*/ "NEXT не соответствует FOR",
		/*12*/ "Уровень вложенности GOSUB слишком велик",
		/*13*/ "RETURN не соответсвует GOSUB"
	};

	printf( "!!!%s\n", e[error] );
	///longjmp( e_buf, 1 ); /* Возврат в точку сохранения */
}


/* Поиск соответсвия внутреннего формата для текущей лексемы в таблице лексем. */
char look_up( char* s )
{
	register int i;
	char *p;

	/* преобразование к ниженму регистру */
	p = s;
	while( *p ){ *p = tolower(*p); p++; }

	/* просматривается, если лексема обнаружена в таблице */
	for( i = 0; *table[i].command; i++ )
		if( !strcmp( table[i].command, s ) ) return table[i].tok;
	return 0; /* Нераспознанная команда */
}



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

/* Поиск значения переменной */
int find_var( char* s )
{
	if ( ! isalpha( *s ) )
	{
		serror(4); /* Не переменная */
		return 0;
	}
	return variables[ toupper(*token) - 'A' ];
}

/* Возврат лексемы во входной поток */
void putback()
{
	char *t;

	t = token;
	for( ; *t; t++ ) prog--;
}

