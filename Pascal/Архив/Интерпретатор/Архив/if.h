bool operatorIF()
{
	get_token();
	if(get_logic_exp(tok))
	{
		get_token();
  	if(tok == THEN)
		{
			get_token();
			if(tok != ELSE)
			{
				ExecOperator(tok);
			}
			else
			{
				get_token();
				PassOperator(tok);
			}
		}
		else return 1;
	}
	else
	{
		get_token();
		if(tok == THEN)
		{
			get_token();
			if(tok != ELSE)
			{
				PassOperator(tok);
			}
			else
			{
				get_token();
				ExecOperator(tok);
			}
		}
		else return 1;
	}
	return 0;
}
