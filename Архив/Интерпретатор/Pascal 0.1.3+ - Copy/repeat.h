#ifndef REPEAT_H_INCLUDED
#define REPEAT_H_INCLUDED

#include "globals.h"
#include "get_token.h"
#include "operator.h"

bool ExecRepeat()
{
    bool finish = false;
    bool ok;
    char* q = prog;//Запомнили начало цикла

    do
    {
        while(1)
        {
            ok = ExecOperator();
            if (ok)
            {
                get_token();
                if ( tok == UNTIL ) break;//Если вместо ожидаемой ; получили конец цикла

                if (*token!=';')
                {
                    serror(" Expected ; ");
                    return false;
                }
            }
            else
            {
            	if ( tok == UNTIL ) break;//Если после точки с запятой нам попался не следующий оператор, а
            	//конец цикла, ExecOperator ведёт себя именно так: возвращает false, а текущей лексемой оставляет until
            	else return false;
            }
        }


        bool result=get_logic_exp (&ok);
        if (ok)
        {
            if (result)
                return true;
            else
            {
                prog=q; // переходим к началу цикла.
                finish=false;
            }
        }
        else return false;
    }
    while (finish == false);

    serror("repeat.h::error. Unreachable code is reached");
    return false;
}


// Глупенькая реализация пропуска оператора.
bool PassRepeat()
{
	int until_count = 1;//Нужно найти ещё один UNTIL.
	while ( until_count > 0 )
	{
		get_token();
		if( tok == REPEAT )
			until_count ++;
		else if ( tok == FINISHED )
		{
			serror("Unexpected end of file");
			return false;
		}
		else if ( tok == ERROR )
			return false;

		else if ( tok == UNTIL )
			until_count -- ;
	}

	bool ok;
	pass_logic_exp( &ok );
	if( !ok ) return false;
	else return true;
}




#endif // REPEAT_H_INCLUDED
