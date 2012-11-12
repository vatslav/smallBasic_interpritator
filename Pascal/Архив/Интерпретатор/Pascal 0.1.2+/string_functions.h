#ifndef STRING_FUNCTIONS
#define STRING_FUNCTIONS

#include <string.h>

/* Возвращает 1, если "с" пробел или табуляция или \n или \r */
int iswhite( char c )
{
	if( c == ' ' || c == '\t' || c == '\n' || c == '\r' ) return 1;
	else return 0;
}


/* Возвращает "истину", если "c" "разделитель" -  в смысле нечто, что может быть DELIMETER или OPERATION, ну ещё \r\n\t да пробел - всякие пробельные символы. Ну и напоследок признак конца строки 0 */
int isdelim(char c)
{
	if( strchr(".,;:()[]+-*/<>=\r\n\t ", c ) || c == 0 )
		return 1;
	else
		return 0;
}

/* возвращает true, если данные символ может содержаться в имени идентификатора ( или зарезервированного слова ). */
//То есть возвращает true при c in { a..z, A..Z, 0..9, _ }
int isIdentifierSymbol( char c )
{
	return ( isalpha(c) || isdigit(c) || c == '_' );
}

#endif
