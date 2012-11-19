#pragma once

#ifndef LEXEMS_SUBSYSTEM
#define LEXEMS_SYBSYSTEM

#include "globals.h"
#include "string_functions.h"
#include "variables.h"

extern char getTokenTypeForIdentifier( char* name );//из подсистемы variables. Без этого заголовка не компилируется.

/*
get_token() считывает следующую лексему и переводит указатель prog на начало следующей лексемы.
пишет результат в глобальные переменные:
 char* token ( текст прочитанного токена )
 char tok ( внутреннее представление токена - число. 0, если нет внутреннего представления )
 char token_type ( тип токена, число )

get_token во всём работает аналогично get_token из Small Basic, кроме:
* пропускает не только табуляции и пробелы, но и переходы на следующую строку.
* возвращает пустую строку в случаях: token_type = ERROR, FINISHED



***ТИПЫ ТОКЕНОВ TOKEN_TYPE***

DELIMITER  . , ; : ( ) [ ]
RESERVED const var integer real array of true false
OPERATOR if then else while for to downto do repeat until := readln writeln read write begin end
OPERATION + - * / div mod < > >= <= <> = and or not ////для операций не будем вводить внутреннее представление.
FUNCTION exp, ln, sin, cos
VARIABLE идентификаторы ( имена ) переменных
CONSTANT идентификаторы констант

REAL_NUMBER 123.09
INT_NUMBER 123

STRING строки.

FINISHED конец кода программы token = 0
ERROR 134abc 123.abc, неизвестный идентификатор token = 0
*/
#include "global_defines.h" //тут описаны define-s для типа и внутреннего представления лексем


extern char token[80]; /* внешнее представление лексемы*/
extern char token_type; /* тип лексемы*/
extern char tok; /* внутреннее представление лексемы */

extern bool SimpleWork; /* режим работы get_token */






//Пропустить пробельные символы.
inline void PassWhite()
{
	while( iswhite(*prog) )
	{
		//if ( *prog == '\n' ) lexem_linenumber ++;//учитываем переход не следующую строку.
		/*
		Сложность реализации определения номера строки кроется в следующем:
		* Виртуозное перемещение указателя prog в ReadVAR и ReadCONST, которое сводит на нет попытки отследить
		 номер строки просто подсчитывая \n
		* Циклы: там тоже происходит прыжок на неопределённое расстояние. И заниматься подсчётом номера строки
		 в реализации цикла как-то совсем не охота.

		Итак, проблема в том, что указатель prog может прыгать непредсказуемо далеко...
		*/
		++prog; /* Пропуск пробелов и переходов на следующую строку.
		Бежим, пока не встретим непробельный символ
		( в том числе конец строки - нулевой байт ) string_functions::iswhite */
	}
}

/* Чтение лексемы */
char get_token()
{
	register char *temp;
	token_type = 0; tok = 0;
	temp = token;


	PassWhite();//Пропустить пробелы и переходы на следующую строку. Учесть возможные переходы на следующую строку ->
	// увеличить номер строки, в которой находится текущая лексема ( lexem_linenumber ).


  //Пропуск комментариев
  if ( *prog == '{' )
  {
    int count = 1;
    while ( count > 0 )
    {
      prog++;
      if ( *prog == '{' ) count ++;
      else if ( *prog == '}' ) count --;
      else if ( *prog == '\0') break;
      //else if ( *prog == '\n' ) lexem_linenumber ++;
    }
    if ( *prog ) prog++;

	PassWhite();
  }

	////////////// Конец программы ///////////////////////////////
	if( *prog == '\0' )
	{
		/* Конец файла */
		*token = 0;
		tok = 0;
		return( token_type = FINISHED );
	}

	///////////// Разбор разделителей DELIMITER. Отслеживаем случай присваивания ( OPERATOR ). //////////////////////////
	if( strchr(".,;:()[]", *prog ) )
	{
		/* Разделитель */
		*temp = *prog;


		bool assign = false;//Признак того, что быть может ожидается присваивание.
		if( *prog == ':' ) assign = true;//Если мы встретили двоеточие, вполне возможно, что это присваивание

		prog++; /* Переход на следующую позицию */
		temp++;

		if( assign && *prog == '=' )
		{
			*temp = *prog;
			prog++;
			temp++;
			*temp = 0;
			tok = ASSIGNMENT;//у присваивания, как у всякого оператора, есть внутреннее представление.
			return ( token_type = OPERATOR ); // это присваивание.
		}

		*temp = 0;
		return ( token_type = DELIMITER );
	}

	/////// Разбор односимвольных и двухсимвольных операций OPERATION. ////////////////////////////////
	if( strchr("+-*/<>=", *prog ) )
	{
		*temp = *prog;


		bool two_symbols = false;//Признак того, что быть может ожидается двусимвольная операция ( >= <= <> )
		if( *prog == '>' || *prog == '<' ) two_symbols = true;//Если мы встретили > или <, то вполне возможно, что это двусимвольная операция

		prog++; /* Переход на следующую позицию */
		temp++;

		if( ( two_symbols && *prog == '=' ) || ( two_symbols && *(prog-1) == '<' && *prog == '>' ) )
		{
			*temp = *prog;
			prog++;
			temp++;
		}

		*temp = 0;
		return ( token_type = OPERATION );
	}
	////////////////////Разбор строк STRING ////////////////////////////
	if ( *prog == '\'' )
	{
		/* строка кавычек */
		prog ++;
		while( *prog != '\'' && ( *prog != '\n' && *prog != '\r' ) ) *temp++ = *prog++;
		if( *prog == '\n' || *prog == '\r' )
		{
			serror("Expected \"'\", found end of line.");
			*token = 0;
			return ( token_type = ERROR );
		}
		prog++; *temp = 0;
		return( token_type = STRING );
	}
	/////////////////// Разбор чисел INT_NUMBER REAL_NUMBER /////////////////////
	if( isdigit(*prog) )
	{
		/* число */
		while( isdigit( *prog ) ) *temp++ = *prog++; //string.h::isdigit возвращает true, если переданный символ - цифра.

		if( *prog == '.' ) // Если остановились на точке, то это дробное число REAL_NUMBER или указание диапазона индексов в массиве.
		{

			if ( *(prog+1) == '.' )//Если и дальше точка, то мы внутри объявления массива: a:array [45..90] ...
			{
				*temp = 0;
				return ( token_type = INT_NUMBER );
			}

			*temp++ = '.';
			prog++;
			while( isdigit( *prog ) ) *temp++ = *prog++;

			if( *(prog-1) == '.' ) // цикл ни разу не выполнился. На входе например 134.abc
			{
				*token = '\0';
				serror("Uncorrect lexem. Expected real number, got something like 123.abc");
				return ( token_type = ERROR );
			}
			if ( !isdelim( *prog ) )
			{
				*token = 0;
				serror("Uncorrect lexem. Expected real number, got something like 123.45abc" );
				return ( token_type = ERROR );
			}

			*temp = '\0';
			return( token_type = REAL_NUMBER );

		}
		else if ( !isdelim( *prog ) ) /// Если мы остановились не на разделителе, то введена кракозябра вида 123abc
		{
			*token = 0;
			serror("Uncorrect lexem. Expected integer number, got something like 123abc" );
			return ( token_type = ERROR );
		}

		//Мы остановились на разделителе. Всё нормально. Это просто целое число INT_NUBMER.


		*temp = '\0';

		return( token_type = INT_NUMBER );
	}

	///// Ну а теперь самое сложное: доразбор OPERATION, разбор RESERVED OPERATOR FUNCTION VARIABLE CONSTANT //////////////

	if( isalpha( *prog ) || *prog == '_' ) // string.h::isalpha - возвращает true, если переданный символ - буква.
	{
		//считываем зарезервированное слово или идентификатор.
		//Для идентификаторов напишем правила построения имён:
		//1) имя идентификатора начинается с английской буквы или символа '_'
		//2) остальными символами могут быть английские буквы, цифры и _.
		//зарезервированные слова - состоят только из английских букв, посему они вписываются в это правило.

		//считываем идентификатор ( или зарезервированное слово ) до тех пор, пока не встретим негодный для имени идентификатора символ ( к коим относятся и разделители ).
		while( isIdentifierSymbol( *prog ) /*&& !isdelim( *prog )*/ ) *temp++ = *prog++;

		//если причиной выхода из цикла был не разделитель, а некорректным символ, то выдаём ошибку.
		if ( !isdelim( *prog ) )
		{
			*token = 0;
			serror("Uncorrect identifier name");
			return ( token_type = ERROR );
		}
		*temp = 0;

		char type;
		tok = look_up( token, &type ); /* Преобразование во внутренний формат в предположении, что мы считали одно из зарезервированных слов ( "зарезервированные слова" - это не только RESERVED, но и OPERATOR, FUNCTION, некоторые OPERATION ). В type будет записан тип лексемы. Если наше предположение неверно - ну что же, значит наверно это переменная или константа. Или невесть что... */
		if ( type != 0 )
			return ( token_type = type );
		else
		{
			if ( SimpleWork ) // Если мы работем в простом режиме - не пытаемся разобраться с неизвестными идентификаторами.
			{
				return ( token_type = IDENTIFIER );
			}
			else // сложный ( нормальный ) режим работы, расказанный в ОПИСАНИИ интерпретатора.
			{
				return ( token_type = getTokenTypeForIdentifier( token ) );//обращение к подсистеме variables
			}
		}


	}


	//Обработка ошибок
	if ( *prog == '"' )
	{
		serror("Unexpected symbol '\"'. Pascal strings are in unary quotes (')");
	}
	//Если мы сюда попали, то совсем всё плохо. Или имя идентификатора начинается с негодящегося символа, или что самое худшее - get_token не может работать с этой строкой в силу того, что в ней есть кириллические символы ( по крайне мере в linux такая проблема есть ).
	else
	{
		serror( "Expected latin symbol. Maybe given lexem with cyrillic symbols" );
	}
	*token = 0;
	return ( token_type = ERROR );


}

//char get_token()
//{
//  char out = get_token0();
//  printf("%s %d %d\n",  token, token_type, tok);
//  getchar();
//  return out;
//}



/* Возврат лексемы во входной поток */
void putback()
{
	char *t;

	t = token;
	for( ; *t; t++ ) prog--;

	if ( token_type == STRING )
	{
	  prog--;
	  prog--;
	  //При считывании строк в token попадает строка без кавычек.
	}
}
#endif
