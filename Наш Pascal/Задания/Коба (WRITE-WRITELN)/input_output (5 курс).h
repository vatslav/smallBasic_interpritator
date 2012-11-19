#ifndef INPUT_OUTPUT_H_INCLUDED
#define INPUT_OUTPUT_H_INCLUDED



bool ExecWrite()  /*WRITE*/
{
    get_token();  /*считываем лексему*/
    if (token[0] == '(')  /*если разделитель*/
    {
        while (token[0] != ')')  /*до тех пор, пока не найдём закрывающуюся скобку*/
        {
            get_token();  /*считываем лексему*/
            if (token_type == FINISHED)  /*если конец текста*/
            {
                serror("Error! Unexepcted quit!");  /*выдаём ошибку о непредвиденном конце*/
                return false;
            }
            else if ( token_type == ERROR )
                return false;
            else if ( token_type == DELIMITER && token[0] == ')' )
                //Если скобки были пусты.
                break;
            /*идём дальше*/
            if (token_type == STRING)  /*если строка*/
            {
                printf("%s",token);  /*печатаем её*/
            }
            else  /*если не строка*/
            {
                putback();  /*возвращаемся на предыдущую лексему*/
                bool temp;
                float val = get_exp_as_real(&temp);  /*пытаемся записать полученное в 'val' и
                                                   получаем в 'temp' результат выполнения*/
                if (temp)  /*если запись выполена успешно*/
                {
                    if (val == (int)val)  /*проверяем, целое число или нет*/
                        printf("%d", (int)val);  /*и если целое, выводим его как целое
                                         /без точки и знаков после неё/*/
                        else printf("%f", val);  /*иначе просто выводим как есть*/
                }
                else
                {
                    serror("Error! Illegal expression!");  /*если записать не получилось,
                                                           выдаём ошибку*/
                    return false;
                }
            }
            get_token();  /*считываем лексему*/
            if (token[0] != ',' && token[0] != ')')  /*если надежда получить запятую или скобку не оправдалась*/
            {
                serror ("Error! Illegal expression! /DELIMITER expected/");
                /*выходим с ошибкой о надежде на разделитель*/
                return false;
            }
        }
        return true;  /*всё хорошо*/
    }
    else if (token_type == DELIMITER && token[0] == ';')
        /*если разделителя в виде открывающейся скобки нет, но стоит точка с запятой*/
    {
        /*//проверяем на случай 'writeln;' или даже внезапно 'write;',
        хотя во втором случае можно выдать Warning о бессмысленности конструкции*/
        putback();//Ведь мы должны закончить на последней лексеме оператора, а точка с запятой к операторам отношения
        //не имеет.
        return true;/*просто выходим*/
    }
    else /*если вообще ничего*/
    {
        serror("Error! DELIMITER ';' or '(' expected!");  /*то это абзац какой-то:
                              выводим ошибку, что хотели видеть хотя бы ';' или '('*/
        return false;
    }
}



bool ExecWriteLn()  /*WRITELN*/
{
    if (ExecWrite())  /*выполняем WRITE и смотрим, как она завершится*/
    {
        /*если всё хорошо, осуществляем переход на следующую строку*/
        printf("\n");
        return true;
    }
    else return false;  /*иначе выходим, ошибку укажет WRITE*/
}

//////////////////////////////////////////

// Относительно особенностей работы операторов чтения и реализации этих особенностей смотри проект
// GetCharDiscovery папки CodeBlocksProjects.

bool ExecRead()  /*READ*/
{
    char type;
    get_token();  /*считываем лексему*/
    if (token[0] == '(')  /*если открывающаяся скобка*/
    {
        while (token[0] != ')')  /*до тех пор, пока не найдём закрывающуюся скобку*/
        {
            get_token();
            if (token_type == VARIABLE)  /*если переменная*/
            {
                type = getVariableType(token);
                if ( type == INTEGER )
                {
                    int *data = getIntegerVariable( token );
                    if ( data == 0 ) return false;
                    scanf("%d", data );
                }
                else if ( type == REAL )
                {
                    float* data = getRealVariable( token );
                    if ( data == 0 ) return false;
                    scanf("%f", data );
                }
                else if ( type == ARRAY )
                {
// TODO (and#1#): ARRAYS
                    serror("ARRAYS not implemented yet");
                    return false;
                }
                else
                {
                    serror("variable expected");
                    return false;
                }
            }
            else if ( token_type == DELIMITER && token[0] == ')' )//пустые скобки
            {
                //Согласно проекту GetCharDiscovery из папки CodeBlocksProjects по вводе последнего параметра функции
                //readln мы должны проглотить из входного потока всем символы до конца строки ( чтобы они не
                //всплыли при следующем вызове readln ).
				while( getchar() != '\n' );

                return true;
            }

            else
            {
                serror("Error! Variable expected!");  /*если не переменная, выводим ошибку*/
                return false;
            }
            //Прочитаем , или )
            get_token();
            if ( token_type == DELIMITER && token[0] == ')' )
            {
				while( getchar() != '\n' );
				return true;
            }
			else if ( token[0] != ',' )
			{
				serror("Expected ')' or ','");
				return false;
			}

        }
    }
    else
        if (token[0] == ';')  /*если точка с запятой*/
        {
            putback();
            while( getchar() != '\n' );
        }
        else
        {
            serror("Error! DELIMITER '(' expected!");  /*если нет открывающейся скобки,
                                                       выходим с ошибкой*/
            return false;
        }
    return true;
}

bool ExecReadLn()  /*READLN*/
{
    return ExecRead();  /*пока с файлами не работаем, просто заглушка ( при вводе с клавиатуры нет разницы между read и readln )*/
}





//Простенькие функции пропуска операторов. Не отслеживают всех ошибок - ну и ладно!
bool PassWrite()
{
	get_token();
	if ( token_type == DELIMITER && token[0] == '(' )
	{
		while ( token[0] != ')' )
		{
			get_token();
			if ( token_type == FINISHED )
			{
				serror("Unexpected end of file");
				return false;
			}
			else if ( token_type == ERROR )
				return false;
		}
		return true;
	}
	else if ( token_type == DELIMITER && token[0] == ';' )
	{
		putback();
		return true;
	}
	else
	{
		serror("Expected ( or ; ");
		return false;
	}
}
bool PassWriteLn()
{
	return PassWrite();
}

bool PassRead()
{
	return PassWrite();
}
bool PassReadLn()
{
	return PassWrite();
}



#endif // INPUT_OUTPUT_H_INCLUDED
