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
string - Имя идентификатора
int/float - Значение идентификатора
bool - Константа/переменная*/
struct IntVar
{
    string name;
    int value;
    bool isConst;
};

//Структура для переменных типа REAL
struct RealVar
{
    string name;
    float value;
    bool isConst;
};

//Для хранения переменных/констант и массивов используем
//ассоциативные массивы (карты/контейнеры)

std::map <string, IntVar> Integers; //Карта для INTEGER
std::map <string, RealVar> Reals;  //Карта для REAL

std::map <string, IntVar>::iterator start = Integers.begin ();
std::map <string, RealVar>::iterator start2 = Reals.begin ();
/**
string - Имя идентификатора
int - индекс начала массива
int - индекс конца массива
*/
std::map <string, int, int> Arrays;    //Карта для массивов

IntVar temp;
RealVar temp2;

void AddVar (char* name, char type, int value,/*void* value,*/ bool constant) //Функция добавления идентификатора в карту
{
    if (type == INTEGER) ///Если тип переменной INTEGER
    {
       if (Integers.count(name) == 0)
       {
           temp.name = name;
           temp.isConst = constant;
           //temp.value = *(int*)(value);
           temp.value = value;
           Integers[name] = temp;
       }
       else
           printf ("The variable with this name is already exists");
    }
    else if (type == REAL) ///Если тип переменной REAL
    {
       if (Reals.count(name) == 0)
       {
           temp2.name = name;
           temp2.isConst = constant;
           temp2.value = *(float*)(value);
           Reals[name] = temp2;
       }
       else
           printf ("The variable with this name is already exists");
    }
}

char GetTypeVar (char* name) //Функция получения типа переменной
{
    if (Integers.count(name) != 0)
      // if ()
       return INTEGER;
    else if (Reals.count(name) != 0)
       return REAL;
    else return 0;
}

void PrintMap ()
{
    printf ("Name\t\tValue\t\tType\n\n");
    for (start = Integers.begin (); start != Integers.end (); start++)
    //   printf ("%s\t%d\tINTEGER",/*(*start).first*/(*start).second.name/*start->second.value*/);
    std::cout<<(*start).second.name<<"\t\t"<<(*start).second.value<<"\t\tINTEGER"<<"\n";
}



#endif // VARS&ARRAYS_H_INCLUDED
