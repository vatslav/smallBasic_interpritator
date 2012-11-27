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
/**
int type - Тип идентификатора
float value - Значение идентификатора
bool isConst - Константа/переменная
*/

//Структура для переменных типа INTEGER/REAL
struct Var
{
    int type;
    float value;
    bool isConst;
};

/**
int type - Тип массива
bool isBig - Многомерный/одномерный массив
int from - нижняя граница массива
int to - верхняя граница массива
float *el - элементы массива
*/
//Структура для массивов типа INTEGER/REAL
struct Array
{
    int type;
    bool isBig;
    int from;
    int to;
    float *el; ///указатель на 1 элемент массива
};

//Для хранения переменных/констант и массивов используем
//ассоциативные массивы (карты/контейнеры)

map <string, Var> Vars; //Карта для идентификаторов типа INTEGER/REAL
map <string, Array> Arrays; //Карта для массивов типа INTEGER/REAL

//Итераторы для работы с картами в цикле
map <string, Var>::iterator start = Vars.begin ();
map <string, Array>::iterator start2 = Arrays.begin ();

//Временные объекты наших структур
Var temp;
Array temp2;

/**
Функция добавления переменной/константы в карту

name - Имя переменной/константы
type - Тип переменной/константы
value - Значение переменой/константы
constant - Характеристика (true - константа, false - переменная)
*/
bool AddVar (char* name, int type, float value, bool constant)
{
   if (Vars.count(name) == 0) //Если в карте нет переменной с таким именем
   {
       temp.isConst = constant;
       temp.type = type;
       if (type == INTEGER)     //Если тип переменной/константы INTEGER
           temp.value = (int)value; //Приводим к типу INTEGER
       else if (type == REAL)   //Если тип переменной/константы REAL
           temp.value = value;  //Оставляем тип REAL
       Vars[name] = temp;   //Кидаем в карту новую пееменную/константу
       return 1;
   }
   else //Иначе сообщаем, что в карте есть переменная/константа с таким именем
       {
            printf ("The variable/constant with this name is already exists");
           return 0;
       }


}

/**
Функция получения типа переменной/константы

name - Имя переменной/константы
return - Тип переменной/константы
*/
int GetTypeVar (const char* name)
{
    if (Vars.count(name) != 0) //Находим переменную/константу в карте
    {
        start->second = Vars[name]; //Ставим итератор на нашу находку
        return start->second.type;  //Возвращаем тип
    }
    else //Иначе оповещаем о том, что не нашли ничего с таким именем
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

/**
Функция получения значения переменной/константы

name - Имя переменной/константы
return - Значение переменной/константы
*/
float getValue (const char* name)
{
    if (Vars.count(name) != 0) //Находим переменную/константу в карте
    {
        start->second = Vars[name]; //Ставим итератор на нашу находку
        return start->second.value;  //Возвращаем значение
    }
    else //Иначе оповещаем о том, что не нашли ничего с таким именем
    {
        printf ("The variable/constant with this name isn't exists");
        return -1.0;
    }
}

/**
Функция проверки на константу

name - Имя переменной/константы
return - Характеристика (true - константа, false - переменная)
*/
bool isConst (const char* name)
{
    if (Vars.count(name) != 0) //Находим переменную/константу в карте
    {
        start->second = Vars[name]; //Ставим итератор на нашу находку
        return start->second.isConst; //Возвращаем характеристику (true - константа, false - переменная)
    }
    else //Иначе оповещаем о том, что не нашли ничего с таким именем
    {
        printf ("The variable/constant with this name isn't exists");
        return 0;
    }
}

///Функция вывода содержания всех карт на экран (для теста)
void PrintMap ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = Vars.begin (); start != Vars.end (); start++)

        if (start->second.type == INTEGER)
            printf ("%s\t\t%.0f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );
        //std::cout<<start->first.c_str()<<"\t\t"<<start->second.value<<"\t\t"<<GetTypeVar (start->first.c_str() )<<" - "<<typeToName(GetTypeVar (start->first.c_str() ))<<"\n";
        else if (start->second.type == REAL)
            printf ("%s\t\t%5.3f\t\t%d - %s\n", start->first.c_str(), getValue(start->first.c_str()), GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );

}

int ReadFromTo ()
{
    get_token(); //Считываем границу массива
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
        std::cout<<(int) getValue(token)<<std::endl;


        return (int) getValue(token);

                    //temp2.from = atoi(token)


    }
}

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

	//get_token(); //Считываем границу массива

	/*
	if (token_type == INTEGER)
        if (atoi(token) >= 1)

            std::cout<<token;
            //return atoi(token);
            //temp2.from = atoi(token);
     //   else*/
      //  {
      //      printf ("Граница массива должна быть >= 1\n");
      //      return;// -1;
      //  }

	from = ReadFromTo (); //Считывание нижней границы массива
	cout<<"Мы все еще живы!";
	//std::cout<<from;

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
    printf ("Все ОК!");
    cout<<"Мы все еще живы!";
    return;
}

#endif // VARS&ARRAYS_H_INCLUDED
