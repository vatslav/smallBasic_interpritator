#ifndef VARS&ARRAYS_H_INCLUDED
#define VARS&ARRAYS_H_INCLUDED

#include "Globals.h"
#include "get_token.h"
#include <map>
#include <string>

using std::string;
using std::make_pair;

//Структура для переменных типа INTEGER
/**
int/float - Значение идентификатора
bool - Константа/переменная*/
struct structVar
{
    //string name;
    void *value; ///указатель на переменнуб или массив
    bool isConst;
    int type;
};

struct array_describer
{
	char basetype;// Базовый тип массива ( INTEGER, REAL или ARRAY )
	int from; // нижний индекс
	int to; // верхний индекс
	void* vars; // ссылка на элементы массива или на array_describer ( если basetype = ARRAY )
};


////Структура для переменных типа REAL
//struct RealVar
//{
//    //string name;
//    float value;
//    bool isConst;
//};

//Для хранения переменных/констант и массивов используем
//ассоциативные массивы (карты/контейнеры)

//std::map <string, IntVar> Integers; //Карта для INTEGER
std::map <string, structVar> var;  //Карта для REAL

std::map <string, structVar>::iterator start = var.begin ();
//std::map <string, RealVar>::iterator start2 = Reals.begin ();
/**
string - Имя идентификатора
int - индекс начала массива
int - индекс конца массива
*/
//std::map <string, int, int> Arrays;    //Карта для массивов

structVar temp;
//RealVar temp2;

void addVar (char* name, int type, void* value, bool constFlag) //Функция добавления идентификатора в карту
{

    if (type == INTEGER) ///Если тип переменной INTEGER
    {
       if (var.count(name) == 0)
       {

           //temp.name = name;
           temp.isConst = constFlag;
           //temp.value = *(int*)(value);
           temp.value = value;
           temp.type = type;
           var[name] = temp;

       }
       else
           printf ("The variable with this name is already exists");
    }
//    else if (type == REAL) ///Если тип переменной REAL
//    {
//       if (Reals.count(name) == 0)
//       {
//           //temp2.name = name;
//           temp2.isConst = constant;
//           temp2.value = value;
//           //temp2.value = value;
//           Reals[name] = temp2;
//       }
//       else
//           printf ("The variable with this name is already exists");
//    }
}

char GetTypeVar (const char* name) //Функция получения типа переменной
{
    if (var.count(name) != 0)
      // if ()
       return INTEGER;
    else if (var.count(name) != 0)
       return REAL;
    else return 0;
}

void PrintMap ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = var.begin (); start != var.end (); start++)
       //printf ("%s\t\t%d\t\t%d - %s\n",start->first.c_str(), start->second.value, GetTypeVar (start->first.c_str() ), typeToName(GetTypeVar (start->first.c_str() )) );
std::cout<<start->first.c_str()<<"\t"<<start->second.value<<"\t"<<GetTypeVar (start->first.c_str() )<<" - "<<typeToName(GetTypeVar (start->first.c_str() ));
//    for (start2 = var.begin (); start2 != var.end (); start2++)
//       printf ("%s\t\t%5.3f\t\t%d - %s\n",start2->first.c_str(), start2->second.value, GetTypeVar (start2->first.c_str() ), typeToName(GetTypeVar (start2->first.c_str() )) );
    //std::cout<<(*start).second.name<<"\t\t"<<(*start).second.value<<"\t\tINTEGER"<<"\n";
}

#endif // VARS&ARRAYS_H_INCLUDED
