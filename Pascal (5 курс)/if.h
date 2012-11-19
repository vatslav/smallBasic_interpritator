#include "operator.h"

extern bool PassOperator();

bool ExecIf()
{
	bool ok;
	bool result = get_logic_exp( &ok );
	if ( !ok )
	{
	  return false;
	}

	if( result )
	{
		get_token();
  	if(tok == THEN)
		{
			get_token();
			if(tok != ELSE)
			{
				putback();
				if ( ! ExecOperator() ) return false;
				//Пропускаем ветку else
				get_token();
				if ( tok == ELSE )
				{
				  if ( !PassOperator() ) return false;
				  else return true;
				}
				else
				{
            putback();
            return true;
				}

			}
			else
			{
				return PassOperator(tok);
			}
		}
		else
		{
		  serror("Expected THEN");
		  return false;
		}
	}
	else
	{
		get_token();
		if(tok == THEN)
		{
			get_token();
			if(tok != ELSE)
			{
				putback();
				if( ! PassOperator(tok) ) return false;

				get_token();
				if ( tok == ELSE )
				{
				  if ( !ExecOperator() ) return false;
				}
				else
				{
				  putback();
				}
				return true;
			}
			else
			{
				if ( !ExecOperator() ) return false;
				return true;
			}
		}
		else
		{
		  serror("Expected THEN");
		  return false;
		}
	}

}



bool PassIf()
{
  bool ok;
  pass_logic_exp( &ok );
  if(!ok) return false;
  get_token();
  if ( tok != THEN )
  {
      serror("Expected THEN");
      return false;
  }
  if ( ! PassOperator() ) return false;
  get_token();
  if ( tok != ELSE )
  {
      putback();
      return true;
  }
  else
  {
      if ( !PassOperator() ) return false;
      else return true;
  }

}
