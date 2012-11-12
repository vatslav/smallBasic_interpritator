#ifndef WHILE_H_INCLUDED
#define WHILE_H_INCLUDED


#include "operator.h"

extern bool PassOperator();

bool ExecWhile()
{
    bool finish = false;;
    bool ok;
    char* q = prog;

    do
    {
       // while (1); - оставим этот шедевр Вити для истории
        bool result=get_logic_exp (&ok);
        if (ok)
        {
            get_token();
            if ( tok != DO )
            {
            	serror("Expected DO");
            	return false;
            }

            if (result == false )
            {
                if ( !PassOperator() ) return false;
                else return true;
            }
            else
            {
                if ( !ExecOperator() ) return false;
                prog=q;
                finish=false;
            }
        }
        else return false;

    }
    while (finish == false);


	serror("while.h::error. Unreachable code is reached");
    return false;
}


bool PassWhile()
{
	bool ok;
	pass_logic_exp( &ok );
	if(!ok) return false;
	get_token();
	if ( tok != DO )
	{
		serror("Expected DO");
		return false;
	}
	if (!PassOperator() ) return false;
	return true;
}


#endif // WHILE_H_INCLUDED
