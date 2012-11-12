#ifndef FOR_H_INCLUDED
#define FOR_H_INCLUDED



/*
for идентификатор := <выражение инициализации> to|downto <выражение конечное > do оператор

"идентификатором"  может быть только идентификатор простой переменной: только типа INTEGER
 Элемент массива в качестве счётчика цикла недопустим.
*/

/*
Выполнить оператор for.
На момент вызова текущей лексемой должна быть for.
После окончания - текущей лексемой будет последняя лексема оператора.
*/
bool ExecFor()
{
	get_token();
	//перешли к счётчику.
	if( token_type != VARIABLE )
	{
		serror("Expected counter.");
		return false;
	}
	if ( getVariableType( token ) != INTEGER )
	{
		serror("Only INTEGER variable can be a counter in FOR");
		return false;
	}

	int* counter =  getIntegerVariable(token);
	if ( counter == 0 )
	{
		serror("ERROR in for. Counter ptr is NULL");
		return false;
	}
	get_token();
	if( tok != ASSIGNMENT )
	{
		serror("Expected ASSIGNMENT ':='");
		return false;
	}
	bool ok;
	*counter = get_exp_as_integer( &ok );
	if(!ok) return false;

	get_token();
	if ( tok!= TO && tok != DOWNTO )
	{
		serror("Expected TO or DOWNTO");
		return false;
	}
	bool up = ( tok == TO ) ? true : false;//up = true - счётчик увеличивается ( TO ), иначе уменьшается ( DOWNTO )

	int finish = get_exp_as_integer( &ok );
	if(!ok) return false;

	if ( up )
	{
		if ( finish < *counter )
		{
			serror("finish value of counter is less, then start for TO-for");
			return false;
		};
	}
	else
	{
		if ( finish > *counter )
		{
			serror("finish value of counter is more, then start for DOWNTO-for");
			return false;
		};
	}

	get_token();
	if ( tok != DO )
	{
		serror("Expected DO");
		return false;
	}
	char* begin = prog;//сохраняем позицию. Отсюда начинается итерация цикла.
	while ( 1 )
	{
		if ( !ExecOperator() ) return false;
		*counter = ( up )? *counter+1 : *counter - 1;

		if ( up && *counter > finish ) return true;
		else if ( !up && *counter < finish ) return true;
		else prog = begin;
	}

	serror("ExecFor::Unreachable code is reached");
	return false;
}


bool PassFor()
{
	get_token();
	//перешли к счётчику.
	if( token_type != VARIABLE )
	{
		serror("Expected counter.");
		return false;
	}


	get_token();
	if( tok != ASSIGNMENT )
	{
		serror("Expected ASSIGNMENT ':='");
		return false;
	}
	bool ok;
	pass_exp_as_integer( &ok );
	if(!ok) return false;

	get_token();
	if ( tok!= TO && tok != DOWNTO )
	{
		serror("Expected TO or DOWNTO");
		return false;
	}

	pass_exp_as_integer( &ok );
	if(!ok) return false;


	get_token();
	if ( tok != DO )
	{
		serror("Expected DO");
		return false;
	}
	if(!PassOperator() ) return false;

	return true;
}


#endif // FOR_H_INCLUDED
