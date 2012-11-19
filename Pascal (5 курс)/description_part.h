#pragma once
#ifndef DESCRIPTION_PART
#define DESCRIPTION_PART
#include "global_defines.h"
#include "globals.h"
#include "variables.h"


//Читает одну строчку объявления: список_имён:ТИП
//При вызове текущей лексемой должна быть лексема ПЕРЕД началом объявления
//( то есть prog должен указывать на начало объявления )
//После окончания текущей лексемой является последняя лексема объявления
//( всегда  ; )
bool ReadVAR_string()
{

	char* firstIdentificator = prog;//Prog указывает на начало первого
	//идентификатора. Запомним это место.

	get_token();//переходим к первому идентификатору.

	do //В начале цикла мы висим не первом идентификаторе. И вообще
	//в начале каждой итерации текущей лексемой является идинтефикатор.
	{
		if ( token_type != IDENTIFIER )
		{
			serror("Expected identifier name");
			return false;
		}
		get_token();//переходим к лексеме после идентификатора.

		if ( token_type == DELIMITER && token[0] == ',' )
		//Коли список идентификаторов продолжается, переходим к следующему
			get_token();

		else if ( !( token_type==DELIMITER && token[0] == ':') )
		{
			serror("Expected ',' or ':'");
			return false;
		}

	}
	while( !( token[0] == ':' && token_type == DELIMITER ));
	//Бежим по циклу пока не дойдём до конца списка идентификаторов

	//Итак, мы прощёлкали список идентификаторов. Теперь надо понять,
	//что это такое объявляется - простые переменные ( INTEGER, REAL )
	//или массивы.
	get_token();
	if ( tok == INTEGER || tok == REAL )
	{
		//Так что объявлять то надо - INTEGER или REAL
		char type = tok;
		//Не зря мы запоминали позицию первого токена! Теперь прыгаем
		//к началу списка идентификаторов.
		prog = firstIdentificator;
		do //В этом цикле мы снова бежим по списку идентификаторов.
		//На на этот раз можно забыть про проверку ошибок - мы всё уже проверили выше.
		{
			get_token();//Попали на идентификатор.
			if( ! addSimpleVariable( token, type ) ) return false;
			get_token();//Попали на ',' или на ':'
		}
		while ( token[0] != ':' );
		get_token();//Снова прочитали INTEGER или REAL
		get_token();
		if ( !( token_type == DELIMITER && token[0] == ';' ) )
		{
			serror("Expected ';'");
			return false;
		}
	}
	else if ( tok == ARRAY )
	{
		putback();//Это важно для addArrayVariable
		char* BeginForAddArrayVariable = prog;//Запомнили лексему, которая должна
		//быть текущей при вызове BeginForAddArrayVariable

		char ident[20]; ident[19] = 0;


		bool exit = false;
		do
		{

			prog = firstIdentificator; //Возврат в список идентификаторов.
			//Текущей лексемой является резделительная ',' ( или ':' ) -
			//Правда ничего о том, что это конкретно, мы узнать не можем...
			//в token, token_type, tok лежит совсем не то...

			get_token();//Попали на идентификатор.
			//Скопируем имя идентификатора в переменную ident.
			//Почему мы не передаём в addArrayVariable просто token?
			//Да потому, что addArrayVariable использует get_token, следовательно
			//в процессе работы addArrayVariable token будет изменён.
			//А вот выше с addSimpleVariable такие фокусы проходили, ибо
			//она не вызывает get_token.
			strcpy( ident, token );

			get_token();//Попали на ',' или ':'
			if ( token[0] == ':' ) exit = true;

			firstIdentificator = prog;//Запомнили, где надо искать следующий идентификатор.
			prog = BeginForAddArrayVariable;//Вернулись к концу списка идентификаторов.
			if ( !addArrayVariable( ident )) return false ;
		}
		while ( exit == false );

		get_token();//После отработки addArrayVariable следующей лексемой должна быть ';'
		if ( !( token_type == DELIMITER && token[0] == ';' ) )
		{
			serror("Expected ';'");
			return false;
		}


	}
	else
	{
		serror("Expected variable type: INTEGER, REAL or ARRAY <...>");
		return false;
	}


	return true;

}


//Читает секцию VAR
//При вызове текущей лексемой должна быть лексема VAR
//После окончания текущей лексемой является последняя лексема секции
//( всегда  ; )
bool ReadVAR()
{
	SimpleWork = true;//Переходим в простой режим работы get_token, ибо
	//мы будем читать идентификаторы, о которых подсистема variables
	//ещё ничего не знает.
	bool ok;
	do
	{
		ok = ReadVAR_string();
		if ( !ok )
			return false;

		//Итак, мы на точке с запятой. Теперь надо понять, будет ли ещё
		//одно объявление или нет.
		get_token();
		if ( tok == BEGIN || tok == CONST || tok == VAR )
		{
			putback();
			SimpleWork = false;
			return true;
		}
		else if ( token_type != IDENTIFIER )
		{
			serror("Expected identifier");
			return false;
		}
		putback();//В любом случае забегая вперед надо не забыть вернуться назад.
	}
	while ( 1 );

	SimpleWork = false;//Возвращаемся к нормальному режиму работы get_token

	return true;
}


//Читает секцию CONST
//При вызове текущей лексемой должна быть лексема CONST
//После окончания текущей лексемой является последняя лексема секции
//( всегда  ; )
bool ReadCONST()
{
  /*
  CONST
  ident1 = value1;
  ident2 = value2;
  ...
  */
  SimpleWork = true;
  while (1)
  {
    get_token();
    if ( tok == BEGIN || tok == VAR || tok == CONST )
    {
      putback();
      SimpleWork = false;
      return true;
    }
    else if ( token_type != IDENTIFIER )
    {
      serror("Expected identifier");
      return false;
    }
    else
    {
      char ident[100];
      char value[100];
      strcpy( ident, token );
      get_token();
      if (!( token_type == OPERATION && token[0] == '=' ))
      {
        serror("Expected '='");
        return false;
      }
      get_token();
      //Вот тут-то мы и проверим, что стоит справа от оператора присваивания.
      //Ибо addConstant проверками не занимается.
      if ( token_type != INT_NUMBER && token_type != REAL_NUMBER )
      {
        serror("Expected integer or real number");
        return false;
      }
      strcpy( value, token );
      if ( ! addConstant( ident, value ) ) return false;
      get_token();
      if ( token[0] != ';' )
      {
        serror("Expected ';'");
        return false;
      }

    }
  }
}





#endif
