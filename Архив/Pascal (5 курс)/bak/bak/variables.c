#include <string.h>
#include "preambule.h"

/*структура для хранения переменой.*/
struct variable
{
	char name[20];//имя переменной. Строка. ( FIXME надо перевести на динамические рельсы )
	char type; // внутреннее представление типа переменной. Для пользовательских типов ( описанных в секции type ) также устанавливается внутреннее представление.
	int size; //количество байт, выделенных под переменную.
	void* var;//указатель на саму переменную.

};

/* Максимально допустимое количество переменных в контексте*/
#define CONTEXTSIZE 100 //FIXME временно. Надо конечно динамическую структуру делать.
/*Структура для хранения переменных  текущего контекста( локальных переменных данной функции, процедуры / глобальных переменных ). */
struct context
{
	struct variable vars[ CONTEXTSIZE ];//память под 100 переменных
	int end; //номер свободного элемента в массиве.
};

//*инициализация  структуры context*/
struct context getStructContext( void )
//NB! обратить особое внимание при переовде struct context на динамические рельсы
{
	struct context o;
	o.end = 0;
	return o;
}


#define RecursionLimit 100 /*максимально допустимая глубина рекурсии.*/

/*стек переменных программы
( элемент - массив локальных переменных ). Нулевой элемент - глобальные переменные
*/
//FIXME для поддержки этого стека вполне можно было бы использовать список.
struct context var_stack[ RecursionLimit ];
/*номер текущего элемента в var_stack <-> текущая глубина рекурсии <-> текущий контекст */
int var_context = 0;

/*инициализация массива глобальных переменных.*/
//FIXME Пока поддерживаются только глобальные переменные. - в массиве только один элемент
void initVar_stack()
{
	var_stack[0] = getStructContext();
}

/* Добавить новую переменную в текущий контекст ( то есть в текущий элемент массива var_stack )*/
//FIXME пока поддерживаются только типы integer, real
bool addVariable( char* name, char* type /* строковое представление типа переменной*/ )
{
	struct variable VAR0;

	if ( find_var( name, &VAR0 ) )
	{
		serror(11);
		return false;
	}

	int end = var_stack[ var_context ].end;

	if ( end >= CONTEXTSIZE )
	{
		serror( 13 ); //ограничение реализации. Максимум мощности контекста.
		return false;
	}


	strcpy( VAR0.name, name );

//FIXME К нижнему регистру то преобразовывать надо, но не так. Вдруг мы передали константную строку в качестве типа? В этом случае при попытке *p = ... программа падает
///	char *p;
///	/* преобразование к ниженму регистру типа переменной*/
///	p = type;
///	while( *p ){ *p = tolower(*p); p++; }


	if ( !strcmp( type, "integer" ) )
	{
		VAR0.size = sizeof( int );
		VAR0.type = INTEGER;
		VAR0.var = malloc( sizeof( int ) ); //FIXME при очистке контекста не забывать освобождать память.
	}
	else if ( !strcmp( type, "real" ) )
	{
		VAR0.size = sizeof( double );
		VAR0.type = REAL;
		VAR0.var = malloc( sizeof( double ) );
	}
	else
	{
		serror(12);//ещё не реализованная возможность
		return false;
	}

	var_stack[ var_context ].vars[ end ] = VAR0;
	var_stack[ var_context ].end ++;


	return true;
}

/* Поиск значения переменной. Если переменная найдена, по адресу out ( = *out ) будет записан указатель на структуру variable, описывающую эту переменную.
 * Если переменная не найдена - out := 0  (* то есть мы передаём указатель по ссылке. Приходится пользоваться двойным указателем. *)
 * (* Нам хочется, чтобы пользователь имел доступ к хранящейся в var_stack переменной, посему мы и возвращаем указатель на структуру variable, а
 * не просто делаем копию этой структуры ( тогда хватило бы variable* ) *)
 * */
bool find_var( char* s, struct variable** out )
{
	if ( ! isalpha( *s ) )
	{
		serror(4); /* Не переменная */
		return false;
	}
	register int i = 0;
	//ищем в локальных переменных
	for ( i = 0; i < var_stack[ var_context ].end; i++ )
	{
		if ( !strcmp( var_stack[ var_context ].vars[i].name, s ) )
		{
			*out = &(var_stack[ var_context ].vars[i]);
			return true;
		}
	}
	//не нашли - ищем в глобальных переменных
	for ( i = 0; i < var_stack[0].end; i++ )
	{
		if ( !strcmp( var_stack[0].vars[i].name, s ) )
		{
			*out = &(var_stack[0].vars[i]);
			return true;
		}
	}
	*out = 0;
	return false;	 //вовзращаем признак того, что мы не нашли переменную.
}

/* Получаем значение числовой переменной ( integer или real )
 возвращает тип переменной, по адресу out помещает указатель на значение переменной.
 Если переменная не типа integer или real, или вообще не найдена, возвращает 0*/
char find_numeric_var ( char* s, void** out )
{
	*out = 0;

	struct variable* V;
	bool found = find_var( s, &V );
	if ( !found ) return 0;
	if ( V->type != INTEGER && V->type != REAL ) return 0;

	*out = V->var;
	return V->type;
}




int main()
//пока что примерно так можно присвоить значение переменной. И вывести его на экран.
{
	addVariable( "a10", "integer" );//FIXME пока приходится тип указывать в нижнем регистре...
	addVariable( "b34", "real" );


	void* o;
	bool r = find_numeric_var( "b34", &o );

	printf("%d %p\n", r, o );

	*(double*)o = 4.57; // вот собственно присваивание.
	//!!! o - указывает на область памяти, где хранится значение переменной
	// ( туда же указывает соответсвующий указатель из var_stack ).
	// Посему изменение значения по этому адресу приводит к тому, что изменяется и значение в var_stack
	//аналогично для find_var мы получаем указатель на структуру variable, на которую есть ссылка и из var_stack.
	r = find_numeric_var( "a10", &o ); //для чистоты эксперимента перезапишем указатель o.
	printf("%d\n", (*(int*)o) );//а теперь выведем на экран значение.

	return 0;
}
