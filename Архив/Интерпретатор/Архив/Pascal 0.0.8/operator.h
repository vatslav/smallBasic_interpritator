#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED

#include "get_token.h"
#include "assignment.h"
#include "compound_operator.h"
#include "if.h"
#include "while.h"
#include "repeat.h"
//#include "for.h"
#include "input_output.h"

// «аглушки:

bool ExecFor()
{
  serror("not implemented yet");
	return false;
}

bool PassIf()
{
	return ExecFor();
}
bool PassFor()
{
	return ExecFor();
}
///////////


bool ExecOperator(){

	get_token();
	//printf("%s %d\n", token, token_type);
	if ( token_type == ERROR )
		return false;
	else if ( token_type == FINISHED )
	{
		serror("Unexpected end of file");
		return false;
	}
	else if(token_type == VARIABLE)
	{
		return ExecAssignment();
	}
	else
		switch(tok)
		{
		case BEGIN:
			return ExecCompoundOperator();
			break;
		case IF:
			return ExecIf();
			break;
		case WHILE:
			return ExecWhile();
			break;
		case REPEAT:
			return ExecRepeat();
			break;
		case FOR:
			return ExecFor();
			break;
		case READLN:
			return ExecReadLn();
			break;
		case READ:
			return ExecRead();
			break;
		case WRITELN:
			return ExecWriteLn();
			break;
		case WRITE:
			return ExecWrite();
			break;
		//“еперь особые случаи:
		case UNTIL:
			return false; //ћолча возвращаем false.  ому надо - пусть разбираютс€, что текуща€ лексема - UNTIL или END
			break;
		case END:
			return false;
			break;
		default:
		//≈сли встретили точку с зап€той вместо оператора, то это значит, что в коде понаставлено много лишних
		//точек с зап€той. Ќадо эти лишние точки с зап€той проглотить, но остановитьс€ на предпоследней
		//( ведь после оператора ( дажет такого мнимого ) должна сто€ть точка с зап€той!
			if ( token_type == DELIMITER && token[0] == ';' )
			{
				while( token_type == DELIMITER && token[0] == ';' )
					get_token();
				if ( token_type == ERROR || token_type == FINISHED )
				{
					serror("Error is here");
					return false;
				}
				//»так, мы попали первую лексему после р€да точек с зап€той.
				//“еперь нам нужно вернутьс€ на 2 лексемы назад. “ут putback не поможет -
				//придЄтс€ вручную возвращатьс€.
				while( *prog != ';' )
					prog--;
				//¬ итоге prog встанет на последней точке с зап€той. Ёто значит, что текущей лексемой будет
				// предпоследн€€ точка с зап€той.
				return true;
			}
			else
			{
				serror("Expected operator");
				return false;
			}
			break;
		}
}

bool PassOperator(){

	get_token();
	if ( token_type == ERROR )
		return false;
	else if ( token_type == FINISHED )
	{
		serror("Unexpected end of file");
		return false;
	}
	else if(token_type == VARIABLE)
	{
		return PassAssignment();
	}
	else
		switch(tok)
		{
		case BEGIN:
			return PassCompoundOperator();
			break;
		case IF:
			return PassIf();
			break;
		case WHILE:
			return PassWhile();
			break;
		case REPEAT:
			return PassRepeat();
			break;
		case FOR:
			return PassFor();
			break;
		case READLN:
			return PassReadLn();
			break;
		case READ:
			return PassRead();
			break;
		case WRITELN:
			return PassWriteLn();
			break;
		case WRITE:
			return PassWrite();
			break;
		//case UNTIL: - эти вещи должны перескакивать сами PassRepeat и PassCompoundOperator
		//	return false;
		//	break;
		//case END:
		//	return false;
		//	break;
		default:
		//≈сли встретили точку с зап€той вместо оператора, то это значит, что в коде понаставлено много лишних
		//точек с зап€той. Ќадо эти лишние точки с зап€той проглотить, но остановитьс€ на предпоследней
		//( ведь после оператора ( дажет такого мнимого ) должна сто€ть точка с зап€той!
			if ( token_type == DELIMITER && token[0] == ';' )
			{
				while( token_type == DELIMITER && token[0] == ';' )
					get_token();
				if ( token_type == ERROR || token_type == FINISHED )
				{
					serror("Error is here");
					return false;
				}
				//»так, мы попали первую лексему после р€да точек с зап€той.
				//“еперь нам нужно вернутьс€ на 2 лексемы назад. “ут putback не поможет -
				//придЄтс€ вручную возвращатьс€.
				while( *prog != ';' )
					prog--;
				//¬ итоге prog встанет на последней точке с зап€той. Ёто значит, что текущей лексемой будет
				// предпоследн€€ точка с зап€той.
				return true;
			}
			else
			{
				serror("Expected operator");
				return false;
			}
			break;
		}
}
#endif // OPERATOR_H_INCLUDED
