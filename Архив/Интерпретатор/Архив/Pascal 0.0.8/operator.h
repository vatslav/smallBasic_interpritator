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

// ��������:

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
		//������ ������ ������:
		case UNTIL:
			return false; //����� ���������� false. ���� ���� - ����� �����������, ��� ������� ������� - UNTIL ��� END
			break;
		case END:
			return false;
			break;
		default:
		//���� ��������� ����� � ������� ������ ���������, �� ��� ������, ��� � ���� ������������ ����� ������
		//����� � �������. ���� ��� ������ ����� � ������� ����������, �� ������������ �� �������������
		//( ���� ����� ��������� ( ����� ������ ������� ) ������ ������ ����� � �������!
			if ( token_type == DELIMITER && token[0] == ';' )
			{
				while( token_type == DELIMITER && token[0] == ';' )
					get_token();
				if ( token_type == ERROR || token_type == FINISHED )
				{
					serror("Error is here");
					return false;
				}
				//����, �� ������ ������ ������� ����� ���� ����� � �������.
				//������ ��� ����� ��������� �� 2 ������� �����. ��� putback �� ������� -
				//������� ������� ������������.
				while( *prog != ';' )
					prog--;
				//� ����� prog ������� �� ��������� ����� � �������. ��� ������, ��� ������� �������� �����
				// ������������� ����� � �������.
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
		//case UNTIL: - ��� ���� ������ ������������� ���� PassRepeat � PassCompoundOperator
		//	return false;
		//	break;
		//case END:
		//	return false;
		//	break;
		default:
		//���� ��������� ����� � ������� ������ ���������, �� ��� ������, ��� � ���� ������������ ����� ������
		//����� � �������. ���� ��� ������ ����� � ������� ����������, �� ������������ �� �������������
		//( ���� ����� ��������� ( ����� ������ ������� ) ������ ������ ����� � �������!
			if ( token_type == DELIMITER && token[0] == ';' )
			{
				while( token_type == DELIMITER && token[0] == ';' )
					get_token();
				if ( token_type == ERROR || token_type == FINISHED )
				{
					serror("Error is here");
					return false;
				}
				//����, �� ������ ������ ������� ����� ���� ����� � �������.
				//������ ��� ����� ��������� �� 2 ������� �����. ��� putback �� ������� -
				//������� ������� ������������.
				while( *prog != ';' )
					prog--;
				//� ����� prog ������� �� ��������� ����� � �������. ��� ������, ��� ������� �������� �����
				// ������������� ����� � �������.
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
