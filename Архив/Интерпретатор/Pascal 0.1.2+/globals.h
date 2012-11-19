#pragma once

#ifndef GLOBALS_SUBSYSTEM
#define GLOBALS_SYBSYSTEM

//#include <math.h>
#include <ctype.h> //тут лежа функции tolower toupper
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "global_defines.h"

char* prog; /*буфер анализируемого выражения. Указатель на текущую лексему в исходном коде программы на Паскале. get_token двигает этот указатель. */

/************** Переменные, в которые get_token() пишет результат *****/
char token[80]; /* внешнее представление лексемы*/
char token_type; /* тип лексемы*/
char tok; /* внутреннее представление лексемы */

/******* Режимы работы get_token ***/
bool SimpleWork;
/* Нужно вот зачем: get_token при встрече идентификатора по умолчанию должна разбираться с тем, что это за идентификатор: VARIABLE CONSTANT или FUNCTION.
Если идентификатор не относится ни к одному из этих типов, то это неизвестный идентификатор - надо выдавать ошибку ERROR.
Но ведь при разборе описательных секций VAR и  CONST никаких переменных и констант ещё нет и надо просто молча считывать идентификатор.

Так вот, у нас есть два режима работы get_token:
SimpleWork = false -  сложный, когда мы по-честному работаем так, как заявлено в документации,
SimpleWork = true - и простой, когда для всех неизвестных идентификаторов мы возвращаем тип VARIABLE ( имея ввиду просто некий идентификатор ).

Этой переменной манипулируют функции ReadVAR и ReadCONST, в начале своего исполнения устанавливае её в true, а в конце - в false.*/

/************** Зарезервированные слова  Паскаля *****************/

struct commands{
    char command[20];
    char tok;
};

//RESERVED: const var integer real array of true false
struct commands RESERVED_table[] =
{

	{ "const", CONST },
	{ "var", VAR },
	{ "integer", INTEGER },
	{ "real", REAL },
	{ "array", ARRAY },
	{ "of", OF },
	{ "true", TRUE },
	{ "false", FALSE },
	{ "", 0 }//просто признак конца массива

};

//OPERATOR:  if then else while for to downto do repeat until := readln writeln read write begin end
struct commands OPERATOR_table[] =
{
	{ "if", IF },
	{ "then", THEN },
	{ "else", ELSE },
	{ "while", WHILE },
	{ "for", FOR },
	{ "to", TO },
	{ "downto", DOWNTO },
	{ "do", DO },
	{ "repeat", REPEAT },
	{ "until", UNTIL },
	{ ":=", ASSIGNMENT },
	{ "readln", READLN },
	{ "writeln", WRITELN },
	{ "read", READ},
	{ "write", WRITE },
	{ "begin", BEGIN },
	{ "end", END },
	{ "", 0 }//просто признак конца массива

};

struct commands FUNCTION_table[] = {
	{ "exp", EXP },
	{ "ln", LN },
	{ "sin", SIN },
	{ "cos", COS },
	{ "", 0 }//просто признак конца массива
};

struct commands WORDS_OPERATION_table[] = { // таблица тех операций, который выражаются словами.
	 //у операций нет внутреннего представления. Хотя можно было бы ввести... Но раз договорились не будем.
	 // К счастью о сути всех операций можно догадаться по первой букве ( исключение составляют > <, для которых есть >= <= <> - но и эта проверка невелика ). Вобщем
	 // и без внутреннего представления мы достигнем той же эффективности.
	{ "div" , 0 },
	{ "mod", 0 },
	{ "and", 0 },
	{ "or", 0 },
	{ "not", 0 },
	{ "", 0 } // просто признак конца массива

};

/* Поиск соответсвия внутреннего формата для текущей лексемы в таблице лексем.
s - лексема
по адресу token_type - записывает тип лексемы (RESERVED OPERATOR FUNCTION OPERATION или 0 в случае невозможности определить тип лексемы ) */
char look_up( char* s, char* token_type )
{
	register int i;
	char *p;

	/* преобразование к ниженму регистру */
	p = s;
	while( *p ){ *p = tolower(*p); p++; }

	for( i = 0; *RESERVED_table[i].command; i++ )
		if( !strcmp( RESERVED_table[i].command, s ) )
		{
			*token_type = RESERVED;
			return RESERVED_table[i].tok;
		}

	for( i = 0; *OPERATOR_table[i].command; i++ )
		if( !strcmp( OPERATOR_table[i].command, s ) )
		{
			*token_type = OPERATOR;
			return OPERATOR_table[i].tok;
		}

	for( i = 0; *FUNCTION_table[i].command; i++ )
		if( !strcmp( FUNCTION_table[i].command, s ) )
		{
			*token_type = FUNCTION;
			return FUNCTION_table[i].tok;
		}

	for( i = 0; *WORDS_OPERATION_table[i].command; i++ )
		if( !strcmp( WORDS_OPERATION_table[i].command, s ) )
		{
			*token_type = OPERATION;
			return WORDS_OPERATION_table[i].tok; // это вобщем то безсмысленно - ибо всегда будет 0. Но главное, что *token_type = OPERATION - этого знания нам достаточно.
		}


	*token_type = 0;
	return 0; /* Нераспознанная команда */
}

/****************** Вывод ошибок на экран ************************/
void serror( char* error )
{
	//пока всё незамысловато, но позже будет хитрее.
	printf( "%s\n", error );
}

#endif
