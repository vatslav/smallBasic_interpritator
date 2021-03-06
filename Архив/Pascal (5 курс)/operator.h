#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED

#include "get_token.h"
#include "assignment.h"
#include "compound_operator.h"
#include "if.h"
#include "while.h"
#include "repeat.h"
#include "for.h"
#include "input_output.h"




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
	else if ( token_type == CONSTANT )
	{
	  serror("Expected variable, not conatant");
	  return false;
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
		//Теперь особые случаи:
		case UNTIL:
			return false; //Молча возвращаем false. Кому надо - пусть разбираются, что текущая лексема - UNTIL или END
			break;
		case END:
			return false;
			break;
		default:
		//Если встретили точку с запятой вместо оператора, то это значит, что в коде понаставлено много лишних
		//точек с запятой. Надо эти лишние точки с запятой проглотить, но остановиться на предпоследней
		//( ведь после оператора ( дажет такого мнимого ) должна стоять точка с запятой!
			if ( token_type == DELIMITER && token[0] == ';' )
			{
				while( token_type == DELIMITER && token[0] == ';' )
					get_token();
				if ( token_type == ERROR || token_type == FINISHED )
				{
					serror("Error is here");
					return false;
				}
				//Итак, мы попали первую лексему после ряда точек с запятой.
				//Теперь нам нужно вернуться на 2 лексемы назад. Тут putback не поможет -
				//придётся вручную возвращаться.
				while( *prog != ';' )
					prog--;
				//В итоге prog встанет на последней точке с запятой. Это значит, что текущей лексемой будет
				// предпоследняя точка с запятой.
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
		else if ( token_type == CONSTANT )
	{
	  serror("Expected variable, not constant");
	  return false;
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
		//Если встретили точку с запятой вместо оператора, то это значит, что в коде понаставлено много лишних
		//точек с запятой. Надо эти лишние точки с запятой проглотить, но остановиться на предпоследней
		//( ведь после оператора ( дажет такого мнимого ) должна стоять точка с запятой!
			if ( token_type == DELIMITER && token[0] == ';' )
			{
				while( token_type == DELIMITER && token[0] == ';' )
					get_token();
				if ( token_type == ERROR || token_type == FINISHED )
				{
					serror("Error is here");
					return false;
				}
				//Итак, мы попали первую лексему после ряда точек с запятой.
				//Теперь нам нужно вернуться на 2 лексемы назад. Тут putback не поможет -
				//придётся вручную возвращаться.
				while( *prog != ';' )
					prog--;
				//В итоге prog встанет на последней точке с запятой. Это значит, что текущей лексемой будет
				// предпоследняя точка с запятой.
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
