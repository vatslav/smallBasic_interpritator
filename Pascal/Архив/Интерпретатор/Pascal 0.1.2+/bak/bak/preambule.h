
#ifndef PREAMBULE
#define PREAMBULE

#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELIMITER 1
#define IDENTIFIER 2 //Вместо VARIABLE мы будем возвращать IDENTIFIER, ибо get_token не должна разбираться с тем, что это - имя переменной, вызов функции.
#define NUMBER 3
#define SYSTEM 4 //зарезервированные слова
#define STRING 5
#define QUOTE 6
#define EOL 9
#define FINISHED 255 // Выход за границы типа char. = -1

#define ERROR 11 //Возвращается get_token в случае обнаружения ошибки.

/*extern*/ char* prog; /*буфер анализируемого выражения*/

	
/*extern*/ struct commands{
    char command[20];
    char tok;
};


#define WRITELN 10
#define WRITE 11

#define READLN 20
#define READ 21

#define IF 30
#define THEN 31

#define FOR 40
#define TO 41
#define DO 42

#define WHILE 43

#define REPEAT 44
#define UNTIL 45

#define BEGIN 50
#define END 51

#define INTEGER 100 //C int
#define REAL 101 // C double
#define CHAR 102

#define PROGRAM 1
#define VAR 2
#define TYPE 3
#define FUNCTIONS 4
#define PROCEDURES 5



struct commands table[] = //{ {"", 0 } };
{ /* Таблица обрабатывает комманды, обработанные на нижнем регистре */ 
	{ "writeln", WRITELN },
	{ "write", WRITE },
	{ "readln", READLN },
	{ "read", READ},
	{ "if", IF },
	{ "then", THEN },
	
	{ "for", FOR },	
	{ "to", TO },
	{ "do", DO },
	
	{ "while", WHILE },
	
	{ "repeat", REPEAT },
	{ "until", UNTIL },
	//{ "return", RETURN },
	
	{ "begin", BEGIN },
	{ "end", END },
	
	
	{ "program", PROGRAM },
	{ "var", VAR },
	{ "type", TYPE },
	{ "functions", FUNCTIONS },
	{ "procedures", PROCEDURES },	
	
	{ "integer", INTEGER },
	{ "real", REAL },
	{ "char", CHAR },
	//{"", END }	
	{ "", FINISHED }
};

/*extern*/ char token[80]; /* внешнее представление лексемы*/
/*extern*/ char token_type; /* тип лексемы*/
/*extern*/ char tok; /* внутреннее представление лексемы */


////////////////////////STRING FUNCTIONS///////////////////////////////


#define bool int
#define true 1
#define false 0

/* Возвращает "истину", если "c" разделитель */
bool isdelim(char c)
{
	if( strchr(" ;:.,+-<>/*%^=()", c ) || c == 9 || c == '\r' || c == 0 ) // добавили . и : в качестве разделителя
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
		/*11*/ "Переменная с таким именем уже есть",
		/*12*/ "Не реализованная возможность",
		/*13*/ "Ограничение реализации ( максимум мощности контекста ИЛИ ... )"
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







/* Возврат лексемы во входной поток */
void putback()
{
	char *t;
	
	t = token;
	for( ; *t; t++ ) prog--;
}

#endif //PREAMBULE
