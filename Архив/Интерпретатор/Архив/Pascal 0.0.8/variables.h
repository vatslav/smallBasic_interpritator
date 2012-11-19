#pragma once

#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

#include "global_defines.h"
#include "globals.h"
#include <string.h>
#include "get_token.h"

#define NAMELENGTH 20 // длинна имени переменной

extern char get_token();



/*
Описание переменной
*/
struct variable
{
	char name[ NAMELENGTH + 1];//имя переменной в тексте программы.
	char type;//тип переменной ( INTEGER, REAL или ARRAY )
	char isConst; // это константа ( 1, иначе 0 )
	void* var; // указатель на данные для INTEGRER и REAL. Указатель на описатель array_describer для массивов.

	struct variable* next; // указатель на следующую переменную
};

/*
Описание одномерного массива
*/
struct array_describer
{
	char basetype;// Базовый тип массива ( INTEGER, REAL или ARRAY )
	int from; // нижний индекс
	int to; // верхний индекс
	void* vars; // ссылка на элементы массива или на array_describer ( если basetype = ARRAY )
};




/*
Контекст выполнения программы.
В нашем случае есть только один контекст - глобальный.
Элементы списка упорядочены по возрастанию имён переменных
*/
struct context
{
	struct variable *first; //первый элемент списка
	struct variable *ptr; // элемент, к которому обращались в последний раз
};

struct context GLOBALS;

//Запуск подсистемы. Должен выполняться до первого обращения к интерфейсным функциям.
void START_SUBSYSTEM_VARIABLES()
{
	//Подготовка глобального контекста ( инициализация подсистемы variables )
	GLOBALS.first = 0;
	GLOBALS.ptr = 0;
}





//Генерация в динамической памяти структуры variable. Её адекватная инициализация
//( Для переменных типа ARRAY память не выделяется, для INTEGER и REAL - выделяется и обнуляется. )
struct variable* get_struct_variable( char* name, char type )
{
	struct variable *elem = ( struct variable*)malloc( sizeof( struct variable ) );
	elem->type = type;
	memset(  elem->name, 0, NAMELENGTH + 1 ); //забиваем нулям истроку, чтобы не мучиться с концом строки
	strncpy ( elem->name, name, NAMELENGTH );//копируем не более NAMELENGTH символов из имени переменной.

  /* Идея: приведём к нижнему регистру имя переменной, ибо get_token при чтении лексем
  всё приводит к нижнему регистру. Иначе переменная может не найтись в списке переменных.

  Но делать этого мы всё-таки не будем, ибо в штатном режиме работы строки в подсистему
  хранения переменных будут передаваться get_token. */

//  char* p = elem->name;
//  while( *p )
//  {
//    *p = tolower( *p );
//    p++;
//  }

	elem->isConst = 0;

	elem->next = 0;

	if( type == INTEGER )
	{
		elem->var = malloc( sizeof( int ) );
		*(int*)(elem->var) = 0;
	}
	else if ( type == REAL )
	{
		elem->var = malloc( sizeof( float ) );
		*(float*)(elem->var) = 0.0;
	}
	else
		elem->var = 0;


	return elem;
}


/*
Добавление переменной типа INTEGER или REAL в список известных переменных.
Для переменных типа ARRAY происходит создание структуры variable, однако var = 0.
Возвращает признак успеха операции.
name - имя переменной. Значащими являются первые 20 символов. Остальные отрбасываются.
type - тип переменной.
*/
bool addSimpleVariable( char* name, char type )
{
	struct variable *var = get_struct_variable( name, type );

	struct variable *p = 0;//предыдущий элемент списка. В начале списка это 0.
	struct variable *c = GLOBALS.first;//текущий элемент списка
	if ( c == 0 ) // Если в списке нет ни одного элемента
	{
		GLOBALS.first = var; // просто создаём первый элемент
	}
	else
	{
		int cmp = 0;
		while( c ) // пока текущий элемент - не ноль ( признак конца списка )
		{
			cmp = strncmp( c->name, var->name, NAMELENGTH ); // сравниваем текущий и вставляемый
			if ( cmp > 0 ) // если текущий больше ( а список по возрастанию )
			{
				if ( p ) // Если мы не в начале списка, то вставляем элемент после предыдущего.
				{
					var->next = c;
					p -> next = var;
				}
				else // иначе вставляем элемент на первое место.
				{
					var -> next = GLOBALS.first;
					GLOBALS.first = var;
				}
				break; // и выходим из цикла
			}
			else if ( cmp < 0 ) // если же текущий меньше, переходим к следующей итерации
			{
				p = c; // текущий становится предыдущим
				c = c->next; // текущим становится следующий.
			}
			else // в списке содержится элемент с именем, совпадающим со вставляемым элементом.
			//Переменная с таким именем уже есть.
			{
				serror("The variable with this name is already exists");
				return false;
			}
		}
		if ( !c ) p-> next = var; // мы вышли из цикла. Если по причине достижения конца списка <->
		// <-> не нашли элемент, больший вставляемого, вставляем в конец списка.
	}
	GLOBALS.ptr = var; // запоминаем, к какому элементу массива мы последний раз обращались. В данном случае это добавленный элемент.
	return true;

}



struct variable* findStructVariable( char* name )
{
  //! name должно быть в нижнем регистре. Ибо при добавлении переменной её имя приводится
  //! к нижнему регистру.
	int cmp = 0;
	struct variable* c = GLOBALS.ptr;//проверим последнюю переменную.

	if ( c ) // если таковая есть
	{
		cmp = strncmp( c->name, name, NAMELENGTH );
		if( cmp == 0 ) // Если есть совпадение и...
		{
			GLOBALS.ptr = c;
			return c;
		}
	}

	c = GLOBALS.first; // ищем по всему списку
	while ( c ) // бежим до конца списка
	{
		cmp = strncmp( c->name, name, NAMELENGTH );
		if ( cmp > 0 ) break;//ничего уже не найдём.
		if( cmp == 0 )
		{
			GLOBALS.ptr = c;
			return c;
		}
		else c = c->next;
	}

	GLOBALS.ptr = 0;
	serror("Unknown identifier.");
	return 0;


}


/*
Получить тип переменной.
name - имя переменной.

Возвращает ARRAY, INTEGER, REAL. 0 - в случае ошибки.
*/
char getVariableType( char* name )
{
	struct variable * var = findStructVariable( name );
	if ( !var ) return 0;

	if ( var-> isConst )
	{
		serror("This is a constant. Not a variable");
		return 0;
	}

	return var->type;

}


/*
Получить указатель на данные для переменной типа INTEGER
В случае ошибки возвращает 0.
*/
int* getIntegerVariable( char* name )
{
	char type = getVariableType( name ); // Это сработает быстро, если предварительно вызывалась getVariableType()
	if ( !type ) return 0;
	if ( type != INTEGER )
	{
		serror("This is not an INTEGER variable.");
		return 0;
	}

	return (int*)(GLOBALS.ptr->var);
}


/*
Получить указатель на данные для переменной типа INTEGER
В случае ошибки возвращает 0.
*/
float* getRealVariable( char* name )
{
	char type = getVariableType( name );
	if ( !type ) return 0;
	if ( type != REAL )
	{
		serror("This is not an REAL variable.");
		return 0;
	}

	return (float*)(GLOBALS.ptr->var);
}


// Нужна для "сложного" варианта работы get_token(), где надо определять для идентификатора его тип: переменная или константа.
char getTokenTypeForIdentifier( char* name )
{
	struct variable *var = findStructVariable( name );
	if ( !var ) return ERROR;
	if ( var->isConst ) return CONSTANT;
	else return VARIABLE;
}

//Для подсистемы Expressions:

//Используется для того, чтобы получить число по имени переменной.
//Для REAL и INTEGER возвращает структуру variable с заполненными полями type  и var
//Для ARRAY читает индексы и если в результате взятия значения по индексу  получится INTEGER или REAL, возвращает структур variable
//с заполненными полями type и var.
//Если случилась ошибка, возвращает структуру с полем type = 0
//!!! Очень важное замечание.
//Указатель var возвращаемой структуры содержит указатель на область памяти, где содержится КОПИЯ значения указанной переменной,
//то есть изменить значение указанной переменной не получится. Это важно, иначе функция arith подсистемы expressions изменяла бы
// значение переменной.
//!!! Не надо думать об утечках памяти. В expressions освобождение памяти предусмотрено.
struct variable getNumberFromVariable( char * name )
{
	struct variable* v = findStructVariable( name );

	struct variable out;
	out.type = 0;
	out.var = 0;
	out.isConst = 0;//Чтобы не было предупреждений от  компилятора.
	if ( !v )
	{
		return out;
	}

	out.isConst = v->isConst;

	if ( v->isConst == 1 )
	{
		serror("Expected VARIABLE, got CONST");

		return out;
	}
	out.type = v->type;
	out.isConst = 0;
	if( out.type == INTEGER )
	{
		out.var = malloc( sizeof(int) );// создаём область памяти, где будет храниться копия значения переменной
		*(int*)out.var = *(int*)(v->var);// записываем в созданную память значение.
		return out;
	}
	else if ( out.type == REAL )
	{
		out.var = malloc( sizeof(float) );// создаём область памяти, где будет храниться копия значения переменной
		*(float*)out.var = *(float*)(v->var);// записываем в созданную память значение.
		return out;
	}
	else if ( out.type == ARRAY ) //!TODO
	{
		serror("getNumberFromVariable::ARRAYs are not implemented yet");
		out.type = 0;
		return out;
	}
	else
	{
		serror("getNumberFromVariable::Expected INTEGER, REAL or ARRAY");
		out.type = 0;
		return out;
	}
}


//Получаем значение по имени константы. Во всём аналогична getNumberFromVariable, но не переваривает массивы ( ибо для констант не бывает
//массивов )
struct variable getNumberFromConstant( char * name )
{
	struct variable* v = findStructVariable( name );

	struct variable out;
	out.type = 0;
	out.var = 0;
	out.isConst = 1;// надо чем-то заполнить. Чтобы не было предупреждений.
	if ( !v )
	{
		return out;
	}
	out.isConst = v->isConst;

	if ( v->isConst == 0 )
	{
		serror("Expected CONST, got VARIABLE");

		return out;
	}
	out.type = v->type;
	out.isConst = 0;
	if( out.type == INTEGER )
	{
		out.var = malloc( sizeof(int) );// создаём область памяти, где будет храниться копия значения переменной
		*(int*)out.var = *(int*)(v->var);// записываем в созданную память значение.
		return out;
	}
	else if ( out.type == REAL )
	{
		out.var = malloc( sizeof(float) );// создаём область памяти, где будет храниться копия значения переменной
		*(float*)out.var = *(float*)(v->var);// записываем в созданную память значение.
		return out;
	}

	else
	{
		serror("getNumberFromVariable::Expected INTEGER or REAL for CONSTANTs");
		out.type = 0;
		return out;
	}
}



//Конец: для подсистемы expressions
void debug()
//Падает с segmentation fault если переменных нет
{
	printf("_______________________________\n");
	struct variable* c = GLOBALS.ptr;
	printf("!%s %p", c->name, c->var );
	if ( c->type == INTEGER )
		printf(" %d\n", *(int*)(c->var) );
	else if ( c->type == REAL )
			printf(" %f\n", *(float*)(c->var) );
	else if ( c->type == ARRAY )
	{
		printf( " ARRAY " );
		struct array_describer* desc = (struct array_describer*)(c->var);
		printf("%d %d ", desc->from, desc->to );
		if ( desc->basetype == ARRAY )
		{
			desc = ( struct array_describer*)(desc->vars);
			printf("x %d %d", desc->from, desc->to );
		}
		printf("\n");
	}
	else printf("ERROR\n");
	c = GLOBALS.first;
	while ( c )
	{
		printf("%s %p", c->name, c->var );
		if ( c->type == INTEGER )
			printf(" %d\n", *(int*)(c->var) );
		else if ( c->type == REAL )
				printf(" %f\n", *(float*)(c->var) );
		else if ( c->type == ARRAY )
		{
			printf( " ARRAY " );
			struct array_describer* desc = (struct array_describer*)(c->var);
			printf("%d %d ", desc->from, desc->to );
			if ( desc->basetype == ARRAY )
			{
				desc = ( struct array_describer*)(desc->vars);
				printf("x %d %d", desc->from, desc->to );
			}
			printf("\n");
		}

		else printf("ERROR\n");
		c = c -> next;
	}
	printf("_______________________________\n");
}


//Динамически создаёт описатель массива, полностью выполняет инициализацию для одномерных массивов.
//Для двумерных полностью заполняет описатель первого измерения и создаёт пустой описатель второго измерения.
struct array_describer * get_struct_array_describer( int from, int to, char type )
{
	if ( to <= from )
	{
		serror("a ceiling of the array is less or equals a floor");
		return 0;
	}

	struct array_describer* descr = ( struct array_describer*) malloc( sizeof( struct array_describer ) );
	descr->from = from;
	descr->to = to;

	descr->basetype = type;
	if ( descr->basetype == INTEGER )
	{
		descr->vars = malloc( ( to - from + 1 )*sizeof( int ) );
		memset( descr->vars , 0, ( to - from + 1 )*sizeof( int ) );
	}
	else if ( descr->basetype == REAL )
	{
		descr->vars = malloc( ( to - from + 1 )*sizeof( float ) );
		memset( descr->vars , 0, ( to - from + 1 )*sizeof( float ) );
	}
	else if ( descr -> basetype == ARRAY )
	{
		//В случае, если размерность массива больше 1, создадим следующую структуру array_describer, но проинициализируем
		//её нулями.
		struct array_describer* n = ( struct array_describer*) malloc( sizeof( struct array_describer ) );
		n->basetype = 0;
		n-> from = n -> to = 0;
		n -> vars = 0;

		descr -> vars = n;
	}
	else
	{
		serror("Unknown array type");
		return 0;
	}

	return descr;
}


//Служебные процедуры для addArrayVariable ( смотри ниже )
bool readDotDot()
{
	get_token();
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		serror("Expected '..'");
		return false;
	}
	get_token();
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		serror("Expected '..'");
		return false;
	}

	return true;

}

bool readint( int* result )
{
	get_token();
	bool minus = ( token[0] == '-' ) ? true : false;//разрешаются отрицательные нижние индексы.
	if ( minus )
		get_token();
	//итак, сейчас мы должны оказаться на нижнем индексе массива.
	if ( token_type != INT_NUMBER )
	{
		serror("Expected integer number");
		return false;
	}
	*result = atoi( token );
	*result = ( minus==true )? -(*result) : (*result);
	return true;
}

bool readFromTo( int* from, int* to )
{
	if ( !readint( from ) ) return false;
	if ( !readDotDot() ) return false;
	if ( !readint( to ) ) return false;

	if ( *to <= *from )
	{
		serror("a ceiling of the array is less or equals a floor");
		return false;
	}


	return true;
}

/*
Добавление нового массива в подсистему variables. Самостоятельно при помощи get_token считывает описание массива.
Предполагается, что на момент вызове текущей лексемой является имя ":"
В конце работы текущей лексемой будет последняя лексема, связанная с объявлением массива.
name - имя массива.
возвращает true в случае успеха, иначе false.
*/
/*
name:array [ from..to [ , from..to ] ] of type
или
name:array [from .. to ] of array [ from .. to ] of type
*/
bool addArrayVariable( char* name )
{
	//у нас двумерные массивы, посему пока что сделаем всё просто.

	struct array_describer * ARR_DESC = 0;

	get_token();

	if ( tok != ARRAY )
	{
		serror("Expected 'ARRAY'");
		return false;
	}
	get_token();
	if ( token[0] != '[' )
	{
		serror("Expected '['");
		return false;
	}
	int from, to;
	if ( !readFromTo( &from, &to )  ) return false;

	get_token();
	if( token_type == DELIMITER && token[0] == ',' )
	{
		ARR_DESC =  get_struct_array_describer( from, to, ARRAY );
		if ( !ARR_DESC ) return false;

		if ( !readFromTo( &from, &to ) ) return false;
		get_token();
		if ( !((token_type==DELIMITER) && (token[0]==']') ) )
		{
			serror("Expected ']'");
			return false;
		}
		get_token();
		if ( tok != OF )
		{
			serror("Expected 'of' ");
			return false;
		}
		get_token();
		if( token_type == ERROR ) return false;
		if ( tok == INTEGER )
		{
			struct array_describer* s = (struct array_describer*)(ARR_DESC->vars);
			s -> basetype = INTEGER;
			s -> from = from;
			s -> to = to;
			int size = ( ARR_DESC->to - ARR_DESC->from + 1 ) * ( s->to - s->from + 1 ) * sizeof(int );
			s -> vars = malloc( size );
			memset( s->vars , 0, size );
		}

		else if ( tok == REAL )
		{
			struct array_describer* s = (struct array_describer*)(ARR_DESC->vars);
			s -> basetype = REAL;
			s -> from = from;
			s -> to = to;
			int size = ( ARR_DESC->to - ARR_DESC->from + 1 ) * ( s->to - s->from + 1 ) * sizeof(float );
			s -> vars = malloc( size );
			memset( s->vars , 0, size );
		}
		else
		{
			serror("Expected INTEGER or REAL" );
			return false;
		}

	}
	else if ( !(token_type == DELIMITER && token[0] == ']') )
	{
		serror("Expected ']' ");
		return false;
	}
	else
	{
	//NOTE array [from .. to ] - мы стоим на ]

		get_token();
		if ( tok != OF )
		{
			serror("Expected 'of' ");
			return false;
		}
		get_token();

		if ( token_type == ERROR )
			return false;
		if ( tok == ARRAY )
		{
			serror("Only 1 - and 2-dimension arrays are implemented. Creating 2-dimension array - name: array[from1..to1, from2..to2] of <...>");
			return false;
		}

		ARR_DESC = get_struct_array_describer( from, to, tok );
		if ( !ARR_DESC )
			return false;

	}

	//Итак, так или иначе мы получили в ARR_DESC готовый описатель массива, даже выделили память под массив.
	// Осталось сказать подсистеме variables, что есть такая переменная

	bool ok = addSimpleVariable( name, ARRAY );//Пользуемся особенностью поведения addSimpleVariable для ARRAY
	//Вообще говоря это надо бы делать в начале, зачем что-то делать, если имя такое всё равно занято...
	//А то мы уже память выделили, описатели создали...
	if ( !ok )
		return false;

	GLOBALS.ptr->var = (void*)ARR_DESC; //В GLOBALS в поле ptr лежит указатель на переменную, с которой в последний раз работали,
	//то есть на только что добавленную. В добавленной переменной нужно записать указатель на описатель массива.


	return true;

}



//Получить базовый тип массива. Возвращает INTEGER или REAL, 0 в случае ошибки.
char getArrayBaseType( char* name )
{
	struct variable* v = findStructVariable( name );
	if ( !v )
		return 0;

	if ( v->type != ARRAY )
	{
		serror("This is not an array");
		return 0;
	}
	struct array_describer* d = (struct array_describer*) (v->var);

	while(1)
	{
		if ( d->basetype == ARRAY )
		{
			if(!(d->vars))
			{
				serror("variables::Fatal error in getArrayBaseType");
				return 0;
			}
			d = (struct array_describer*) (d->vars);

		}
		else if ( d->basetype == INTEGER || d->basetype == REAL )
			return d->basetype;
		else
		{
			serror("variables::Fatal error 2 in getArrayBaseType");
			return 0;
		}
	}
}

#endif // VARIABLES_H_INCLUDED
