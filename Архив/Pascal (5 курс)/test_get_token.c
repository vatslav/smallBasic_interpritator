#include <stdio.h>
#include "get_token.h"
#include "expressions.h"
#include "variables.h"


/*

Напишу замечание относительно работы с подсистемой variables вкупе с get_token

get_token приводит все считанные лексемы к нижнему регистру. А подсистема variables
при хранении переменных учитывает регистр.

Поэтому не стоит удивляться, что не работает такой код:
addSimpleVariable("VaRiAbLe", INTEGER );
char* example = "VaRiAbLe";
prog = example;
get_token();
printf("token='%s'; token_type=%d; tok=%d", token, token_type, tok );

Сей код выдаст Unknown identifier.
Почему?
Потому что при чтении строки example слово VaRiAbLe будет преобразовано к variable.
Но для подсистемы хранения переменных важен регистр. Она не знает переменной variable
( get_token попытается выяснить, что это за идентификатор "variable", а для этого нужно
 найти описание переменной с таким именем. Но переменной с таким именем нет, есть лишь
 с именем VaRiAbLe. Посему определение типа идентификатора упадёт с ошибкой ).



 В ОБЩЕМ ПРАВИЛО
 При тестировании подсистемы хранения переменных имена переменных нужно задавать в НИЖНЕМ регистре.
*/


int main()
{
  START_SUBSYSTEM_VARIABLES();


  addSimpleVariable( "hello", INTEGER );
	addSimpleVariable( "a", REAL );

	addSimpleVariable( "willbeconstant", INTEGER );
	addSimpleVariable( "ar", ARRAY );//заглушка.

	//Так делать нельзя. Это временный костыль, ибо пока что мы не умеем создавать константы:
  struct variable* t = findStructVariable( "willbeconstant" );

  t->isConst = 1;
  //Вы не видели того, что только что написано.

  SimpleWork = false; // Нормальный режим работы get_token. Как в документации: для идентификаторов производится разбор: переменная или константа.

  char* example =
  "var\n"
  "ar:array[1..10] of integer;\n"
  "begin\n"
  "hello := 45;\n"
  "a := WillBeConstant + hello;\n"
  "end.";

  printf("%s\n", example );
	prog = example;

	while( 1 )
	{
		get_token();
		printf("token='%s'; token_type=%d; tok=%d;", token, token_type, tok );
		getchar();
	}

	return 0;

}





#ifdef OLDVERSION
int main()
{
	SimpleWork = true; // Пока другого режима работы get_token у нас нет. В этом режиме не разбирается тип идентификаторов.
	/*char* example =
	"VAR\n"
	"	i:INTEGER, r:real\n"
	"BEGIN\n"
	"i := 13;\n"
	"if i >= 4.8 then writeln( i );\n"
	"end.";
	*/
	char* example =
	//"DELIMITER  . , ; : ( ) [ ] \n"
	//"RESERVED const var integer real array of true false\n"
	//"OPERATOR if then else while for to downto do repeat until := readln writeln read write begin end\n"
	//"OPERATION + - * / div mod < > >= <= <> = and or not \n"
	//"FUNCTION exp, ln, sin, cos\n"
	"VARIABLE a10 hello_45 _var\n"
	"CONSTANT tytytyt\n"
	"REAL_NUMBER 123.09 456.89 -78.8 \n"
	"INT_NUMBER 123 -678\n"
	"STRING 'test string' 'строчка символов !'\n"
	"ERROR 134abc 123.abc \n"
	"FINISHED";




	printf("%s\n", example );
	prog = example;

	while( 1 )
	{
		get_token();
		printf("token='%s'; token_type=%d; tok=%d", token, token_type, tok );
		getchar();
	}




	return 0;
}
#endif
