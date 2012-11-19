#ifndef COMPOUND_OPERATOR_H_INCLUDED
#define COMPOUND_OPERATOR_H_INCLUDED

#include "operator.h"

extern bool ExecOperator();

bool ExecCompoundOperator()
{
    while (tok != END)  /*пока не найдём 'end'*/
    {
        bool ok = ExecOperator();
        if (!ok)
        {
            //В эту ветку мы можем попасть, если после последнего оператора не стоит ; ( что допустимо )
            if ( tok == END ) return true; //Такова особенность работы ExecOperator.

            else return false;
        }/*выполняем его*/
        get_token();
        if ( token_type == ERROR ) return false;
        else if ( token_type == FINISHED )
        {
            serror("Unexpected end of file");
            return false;
        }
        else if ( !( token_type == DELIMITER && token[0] == ';' ) && tok != END ) return false;
    }
    return true;
}

/*Комментарий:
Если внутри составного оператора попадается ещё один составной оператор, он распознаётся как оператор,
функция, разбирающая и выполняющая операторы, соображает, что оператор составной и снова вызывает нашу
функцию для разбора и выполнения составного оператора.
Так что в нашей функции рассматривать этот случай отдельно не является необходимым.
*/



bool PassCompoundOperator()  /*пропуск составного оператора*/
{
// int count=0;  /*вводим счётчик*/
// while (count != 0)  /*пока найденные begin'ы не совпадут по количеству с end'ами*/
    //Шедевр, Лёша!!! Этот цикл ни разу не выполнится)

	int count = 1;//У нас уже есть begin!
	while ( count != 0 )
    {
        get_token();
        if (tok == BEGIN) count++;  /*если нашли begin, увеличиваем счётчик*/
        else if (tok == END)   count--;  /*если end - уменьшаем*/
        /*т. о., при нахождении такого же к-ва end'ов, как и begin'ов,
             счётчик обнулится*/
        else if (token_type == FINISHED && count != 0)
        {
            serror("Error! Expected 'end'.");
            return false;
        }
        else if ( token_type == ERROR )
			return false;
    }
    return true;
}
#endif // COMPOUND_OPERATOR_H_INCLUDED
