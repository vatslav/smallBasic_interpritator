#ifndef ASSIGNMENT_H_INCLUDED
#define ASSIGNMENT_H_INCLUDED


#include "operator.h"
#include "expressions.h"

bool ExecSimpleAssignment();


// Заглушка.
// TODO (and#1#): ExecAssignmentForArrays для строк массивов и для двумерных массивов. Пока реализовано только для элементов массивов.
bool ExecAssignmentForArrays()
{
    char type = getArrayBaseType( token );
    //Тут мы уже не проверяем на ошибки - это сделано в ExecAssignment
    if ( type == INTEGER )
    {
        int* data = getIntegerArrayValue( token );
        if ( !data ) return false;

        get_token();
        if ( tok != ASSIGNMENT )
        {
            serror("Expected ':='");
            return false;
        }
        bool ok;
        int result;
        result = get_exp_as_integer( &ok );
        if ( !ok )
            return false;
        else
            *data = result;
        return true;
    }
    else
    {
        float* data = getRealArrayValue( token );
        if ( !data ) return false;

        get_token();
        if ( tok != ASSIGNMENT )
        {
            serror("Expected ':='");
            return false;
        }
        bool ok;
        float result;
        result = get_exp_as_real( &ok );

        if ( !ok )
            return false;
        else
            *data = result;
        return true;
    }

}


bool ExecAssignment()  /*присваивание /':='/*/
{
    char type = getVariableType(token);
    if (type == 0)  /*если ошибка*/
    {
        serror("Assignment error: expected identifier of variable.");
        return false;
    }
    else
        if (type == ARRAY)
            return ExecAssignmentForArrays();  /*реализует Илья*/
        else
            return ExecSimpleAssignment();  /*вот эту функцию надо реализовать*/
}  /*(!) мы не перешли на следующую лексему.*/



//Эвристическая функция пропуска присваивания. В общем просто и ненадёжно)
bool PassAssignment()
{
	while(1)
	{
		get_token();
		if ( tok == UNTIL || tok == END || token[0] == ';' )
		{
			putback();
			return true;
		}
		else if ( token_type == STRING )
		{
			serror("Expected correct assignment");
			return false;
		}
		else if ( token_type == FINISHED )
		{
			serror("Unexpected end of file");
			return false;
		}
		else if ( token_type == ERROR )
			return false;

	}

}

bool ExecSimpleAssignment() /*поехали!*/
{
    //Лёше: Поехали - поехали )))

    char type = getVariableType( token );
    //Тут мы уже не проверяем на ошибки - это сделано в ExecAssignment
    if ( type == INTEGER )
    {
        int* data = getIntegerVariable( token );
        if ( !data ) return false;

        get_token();
        if ( tok != ASSIGNMENT )
        {
            serror("Expected ':='");
            return false;
        }
        bool ok;
        int result;
        result = get_exp_as_integer( &ok );
        if ( !ok )
            return false;
        else
            *data = result;
        return true;
    }
    else
    {
        float* data = getRealVariable( token );
        if ( !data ) return false;

        get_token();
        if ( tok != ASSIGNMENT )
        {
            serror("Expected ':='");
            return false;
        }
        bool ok;
        float result;
        result = get_exp_as_real( &ok );

        if ( !ok )
            return false;
        else
            *data = result;
        return true;
    }

}













#endif // ASSIGNMENT_H_INCLUDED
