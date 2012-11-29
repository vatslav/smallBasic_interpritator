#ifndef VARS&ARRAYS_H_INCLUDED
#define VARS&ARRAYS_H_INCLUDED

#include "Globals.h"
#include "get_token.h"
#include <map>
#include <string>
#include <stdlib.h>

using std::string;
using std::map;
using std::cout;
using std::cin;
using std::endl;


/**@struct array
 Структура для переменных типа INTEGER/REAL
int type - Тип идентификатора
float value - Значение идентификатора
bool isConst - Константа/переменная
*/
struct Var
{
    int type;
    float value;
    bool isConst;
};

/**@struct array
Структура для массивов типа INTEGER/REAL
int type - Тип массива
bool isBig - Многомерный/одномерный массив
int from - нижняя граница массива
int to - верхняя граница массива
float *el - указатель на 1 элемент массива
};
*/
struct Array
{
    int type;
    bool isBig;
    int from;
    int to;
    float *el;

//Для хранения переменных/констант и массивов используем
//ассоциативные массивы (карты/контейнеры)
///Карта для идентификаторов типа INTEGER/REAL
map <string, Var> Vars;
///Карта для массивов типа INTEGER/REAL
map <string, Array> Arrays;

///Итераторы для работы с картами в цикле
map <string, Var>::iterator start = Vars.begin ();
map <string, Array>::iterator start2 = Arrays.begin ();

//Временные объекты наших структур удалил т.к. используем локальные
//Var temp;
//Array temp2;

/**@fuction AddVar
Функция добавления переменной/константы в карту

@param name - Имя переменной/константы
@param type - Тип переменной/константы
@param value - Значение переменой/константы
@param constant - Характеристика (true - константа, false - переменная)
@result сообщает 1, в случае успешного добавления
*/
bool AddVar (char* name, int type, float value, bool constant)
{
   Var temp;
   if (Vars.count(name) == 0) //Если в карте нет переменной с таким именем
   {
       temp.isConst = constant;
       temp.type = type;
       temp.value = value; //Приводим к типу INTEGER
       Vars[name] = temp;   //Кидаем в карту новую пееменную/константу
       return 1;
   }
   else //Иначе сообщаем, что в карте есть переменная/константа с таким именем
       {
            serror("The variable/constant with this name is already exists");
           return 0;
       }


}

/**@fuction GetTypeVar
Функция получения типа переменной/константы по ее символьному имени
@param name - Имя переменной/константы
@return - Тип переменной/константы
*/
int GetTypeVar (const char* name)
{
    if (Vars.count(name) != 0) //Находим переменную/константу в карте
    {
        //start->second = Vars[name]; //Ставим итератор на нашу находку
        //return start->second.type;  //Возвращаем тип
        return Vars[name].type;
    }
    else //Иначе оповещаем о том, что не нашли ничего с таким именем
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

/**@fuction getValue
Функция получения значения переменной/константы по ее символьному имени

@param name - Имя переменной/константы
@return - Значение переменной/константы
*/
float getValue (const char* name)
{
    if (Vars.count(name) != 0) //Находим переменную/константу в карте
    {
        //start->second = Vars[name]; //Ставим итератор на нашу находку
        //return start->second.value;  //Возвращаем значение
        return Vars[name].value;
    }
    else //Иначе оповещаем о том, что не нашли ничего с таким именем
    {
        printf ("The variable/constant with this name isn't exists");
        return -1.0;
    }
}

/**@fuction isConst
Функция проверки на константу

@param name - Имя переменной/константы
@return - Характеристика (true - константа, false - переменная)
*/
bool isConst (const char* name)
{
    if (Vars.count(name) != 0) //Находим переменную/константу в карте
    {
        //start->second = Vars[name]; //Ставим итератор на нашу находку
        //return start->second.isConst; //Возвращаем характеристику (true - константа, false - переменная)

        return Vars[name].isConst;
    }
    else //Иначе оповещаем о том, что не нашли ничего с таким именем
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

/**@fuction fuction
Функция вывода содержания всех карт на экран (для теста)*/
void fuction ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = Vars.begin (); start != Vars.end (); start++)

        if (start->second.type == INTEGER)
            printf ("%s\t\t%.0f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );
        //std::cout<<start->first.c_str()<<"\t\t"<<start->second.value<<"\t\t"<<GetTypeVar (start->first.c_str() )<<" - "<<typeToName(GetTypeVar (start->first.c_str() ))<<"\n";
        else if (start->second.type == REAL)
            printf ("%s\t\t%5.3f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );

}
/**@function ReadFromTo
чтение границы массива (любой)
@result возвращает границу или -1, в случае ошибки*/
int ReadFromTo ()
{
    get_token(); //Считываем границу массива
    ///если границ - число
	if (token_type == INTEGER)
    {
        if (atoi(token) < 0)
        {
            printf ("Граница массива должна быть >= 0\n");
            return -1;
        }

        else
            return atoi(token);
        //temp2.from = atoi(token);
    }
    ///если граница - переменнвя
    if (token_type == VARIABLE)
    {

        if (GetTypeVar (token) != INTEGER)
        {
            printf ("Граница массива должна быть типа INTEGER\n");
            return -1;
        }


        if (isConst (token) != true)
        {
            printf ("Переменная не может быть границей массива\n");
            return -1;
        }

        if (getValue (token) < 0)
        {
            printf ("Граница массива должна быть >= 0\n");
            return -1;
        }
            //std::cout<<5;//getValue(token);
        //std::cout<<"call "<<(int) getValue(token)<<std::endl;

        return (int) getValue(token);

                    //temp2.from = atoi(token)

//
    }
}
/**@function AddArray
добавляет переменные в карту переменных
@param name - символьное имя переменной
@result не чего, но не стоит ли сделать bool?
*/
void AddArray (const char* name)
{
   int from, to;

    get_token();  //Считываем служебное слово "array"

    if ( tok != ARRAY )
	{
		printf ("Expected 'ARRAY'");
		return;
	}

	get_token(); //Считываем "["

	if ( token[0] != '[' )
	{
		printf ("Expected '['");
		return;
	}

	from = ReadFromTo (); //Считывание нижней границы массива
	std::cout<<from;




    get_token(); //Считываем разделители границ массива ".."
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		printf ("Expected '..'");
		return;
	}

	get_token();
	if ( ! ((token_type == DELIMITER) && (token[0] == '.' ) ) )
	{
		printf("Expected '..'");
		return;
	}

	to = ReadFromTo (); //Считывание верхней границы массива
//	std::cout<<to;

    if (to <= from)
    {
        printf("Верхняя граница массива должна быть больше нижней");
        return;
    }
    else

    return;
}

#endif // VARS&ARRAYS_H_INCLUDED
