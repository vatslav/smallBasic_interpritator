#ifndef EXPRESSIONS_H_INCLUDED
#define EXPRESSIONS_H_INCLUDED
////Арифметические выражения.

/*

Выражение = Терм [ + Тер ][- Терм]
Терм = Фактор [ * Фактор ][ / Фактор ][ div Фактор ][ mod Фактор ]
Фактор = [+|-]Значение
Значение = Переменная|Число|(Выражение)

*/
//для отладки убираем var*  на var

bool level2(struct Var*), level3(struct Var*), level4(struct Var*);
bool level5(struct Var*), unary( char, struct Var* ), arith( char, struct Var*, struct Var*);
bool primitive( struct Var * );

//levelI: При вызове текущей лексемой должна быть первая лексема обрабатываемой части выражения.
//При успешном завершении текущей лексемой будет следующая лексема после обрабатываемой части выражения




/**@function get_exp_abstract
расчитывает значение выражения.
@param result - указатель на существующую ( пустую ) структуру Var, в которую и будет записан результат.*/
bool get_exp_abstract( Var* result )
{

  result->value = 0;
  get_token();//первый вызов, после := ?

  if( !*token )
  {
    serror("Неверная лексема");
    return false;
  }
  bool resultExpes = level2( result );
  if(!resultExpes) return false;
  putback(); /* Возвращает последнюю считанную лексему обратно во входной поток */ //зачем?
  return true;
}

/**@function get_exp_as_integer
обработчик выражений  в которых оба учатника типа integer */
int get_exp_as_integer( bool *ok )
{
	struct Var result;//вообще-то надо обнулить указатель, но за нас это сделает get_exp_abstract
	bool good = get_exp_abstract( &result );
	if(!good)
	{
		*ok = false;
		return 0;
	}

	*ok = true;

	if ( result.type == INTEGER )
		return (result.value);
	else if (result.type == REAL )
	{
		serror("Expected INTEGER result, got REAL");
		*ok = false;
		return 0;
	}
	else
	{
		serror("get_exp_as_integer::Expression result is neither INTEGER nor REAL");
		*ok = false;
		return 0;
	}

}



/**@function get_exp_as_real
обработчик выражений  в которых оба учатника типа real */

float get_exp_as_real( bool *ok )
{
	struct Var result;//вообще-то надо обнулить указатель, но за нас это сделает get_exp_abstract
	bool good = get_exp_abstract( &result ); //
	if(!good)
	{
		*ok = false;
		return 0;
	}

	*ok = true;

	if ( result.type == INTEGER )
		return (float)(result.value);
	else if (result.type == REAL )
		return (float)(result.value);
	else
	{
		serror("get_exp_as_real::Expected INTEGER or REAL in struct Var");
		*ok = false;
		return 0;
	}

}





/**@function pass_exp_as_real
Функции пропуска выражения*/
void pass_exp_as_real( bool *ok )
{
	*ok = true;
	get_token();
	if ( strchr( ",.:;", token[0] ) ||
		token_type == OPERATOR || token_type == STRING || token_type == FINISHED ||
			token_type == ERROR || ( token_type == RESERVED && tok!=TRUE && tok != FALSE ) )
	{
		serror("Bad expression");
		*ok = false;
		return;
	}

	do
	{
		get_token();
	}
	while( !( strchr( ",.:;", token[0] ) ||
		token_type == OPERATOR || token_type == STRING || token_type == FINISHED ||
			token_type == ERROR || ( token_type == RESERVED && tok!=TRUE && tok != FALSE ) ) );
	putback();
}
void pass_exp_as_integer(bool*ok)
{
	pass_exp_as_real( ok );
}

void pass_logic_exp( bool*ok )
{
	pass_exp_as_real( ok );
}




/**@function level2
 Сложение или вычитание двух термов */
bool level2( struct Var* result )
{
  register char op;
  struct Var hold;

  bool ok = level3( result );
  if(!ok) return false;

  while( ( op = *token) == '+' || op == '-' )
  {
    get_token();
    ok = level3( &hold );///т.к. тут не *, то нужен ли &
	if(!ok) return false;

    //printf("result=%d, hold=%d\n", *(int*)(result->var), (hold.type) );
    //fflush(stdout);

    ok = arith( op, result, &hold );
   	if(!ok) return false;
  }

  return true;
}


///* Вычисление произведения или частного двух факторов. Ещё целочисленного деления и остатка от деления. */
bool level3( struct Var result )
{
  register char op;
  struct Var hold;hold.value = 0;

  bool ok = level4( ttemp );
  if( !ok ) return false;

  while( ( op = *token) == '*' || op == '/' || ( token_type == OPERATION && (op == 'd' || op == 'm')) )//d, m - div, mod
  {
    get_token();
    ok = level4( &hold );
    if ( !ok ) return false;

    ok = arith( op, result, &hold );
    if( !ok ) return false;

  }

  return true;
}
//
//
//
///* Унарный + или - */
//bool level4( struct Vars result )
//{
//	register char op;
//
//	op = 0;
//	if ( ( token_type == OPERATION ) && ( *token == '+' || *token == '-' ) )
//	{
//		op = *token;
//		get_token();
//	}
//	bool ok = level5( result );
//	if( !ok ) return false;
//
//	if( op )
//		ok = unary( op, result );
//	if ( !ok ) return false;
//
//	return true;
//}
//
///* Обработку выражения в круглых скобках или получение числа. */
//bool level5( struct Vars result )
//{
//	bool ok;
//	if( ( *token == '(' ) && ( token_type == DELIMITER ) )
//	{
//		get_token();
//		ok = level2( result );
//		if ( !ok ) return false;
//
//		if ( *token != ')' )
//		{
//			serror("Expected ')'");
//			return false;
//		}
//		get_token();
//	}
//	else
//	{
//		ok = primitive( result );
//		if(!ok) return false;
//	}
//
//	return true;
//}

/* Определение значения переменной по её имени */
//bool primitive( struct Vars result )
//{
//	//printf("primitive: token=%s; type=%d\n", token, token_type );//111111111
//	switch( token_type )
//	{
//		case VARIABLE:
//		//getNumberFromVariable из подсистемы variables по имени переменной возвращает
//		//структуру Var, содержащую тип и значение переменной, для массиво автоматически
//		//считывает квадратные скобки и если это элемент массива ( индексов столько, сколько надо ), возвращает его описание, иначе
//		//возвращает пустую структуру Var.
//			*result = getNumberFromVar( token );
//			if ( result->type == 0 ) return false;//если не удалось получить значение.
//			//Здесь важно понимать, что ПЕРЕМЕННАЯ с таким именем точно существует ( иначе ругалась бы get_token, но она
//			//сказала, что это Var ). Посему единственная возможность нештатной ситуации - неправильное количество
//			//индексов или некорркетные значения индексов для переменной-массива.
//			get_token();
//			return true;
//			break;
//		case CONSTANT:
//			*result = getNumberFromConstant( token );
//			//Раз get_token сказала, что это CONSTANT, значит такая константа точно есть. А это значит, что ошибки точно не будет.
//			//Вот мы ничего и не проверяем.
//			//!!!Полученная структура ни коим образом не связана с той, что хранится в подсистеме Vars.
//			//!!!Указатель var указывает на область памяти, где лежит КОПИЯ значения из подсистемы Vars.
//			//Почему так? Да потому, что если бы result.var указывал на область памяти, где хранится именно значение
//			//переменной ( а не копия значения ), функция arith это значение бы переписала. И такую ошибку отловить очень
//			//трудно...
//			get_token();
//			return true;
//			break;
//		case INT_NUMBER:
//			result -> type = INTEGER;
//			int* d = (int*)malloc( sizeof( int ) );
//			*d = atoi( token );
//			result -> var = d;
//
//			get_token();
//
//			return true;
//			break;
//		case REAL_NUMBER:
//			result -> type = REAL;
//			float* dd = (float*)malloc( sizeof( float ) );
//			*dd = atof( token );
//			result -> var = dd;
//
//			get_token();
//
//			return true;
//			break;
//		case FUNCTION:
//			{
//				//printf("primitive::FUNCTION\n");
//				//fflush(stdout);
//				bool ok = ExecBuiltInMath( result );
//
//				if(!ok) return false;
//				else return true;
//			}
//			break;
//		default:
//			serror( "Expected identifier or Var"  ) ;
//			return false;
//	}
//}
//
//////// служебные функции /////////
////выполнение операций для INTEGER
//bool int_arith( char o, int* r, int* h )
//{
//
//
//	switch( o )
//	{
//		case '-':
//			*r = *r - *h;
//			break;
//		case '+':
//			*r = *r + *h;
//			break;
//		case '*':
//			*r = *r * *h;
//			break;
//		//case '/': // это обрабатывается в вызывающей процедуре.
//		case 'd':
//
//			if ( *h == 0 )
//			{
//				serror("Division by 0");
//				return false;
//			}
//			*r = (*r)/(*h);
//			break;
//		case 'm':
//
//			*r = (*r) % (*h);
//			break;
//        default:
//            serror("int_arith::unreachable section is reached");
//            return false;
//            break;
//	}
//	return true;
//}
//
////выполнение операций для REAL
//bool float_arith( char o, float* r, float* h )
//{
//
//
//	switch( o )
//	{
//		case '-':
//			*r = *r - *h;
//			break;
//		case '+':
//			*r = *r + *h;
//			break;
//		case '*':
//			*r = *r * *h;
//			break;
//		case '/':
//			if ( *h == 0 )
//			{
//				serror("Division by 0");
//				return false;
//			}
//			*r = (*r)/(*h);
//			break;
//		case 'm':
//			serror("Expected integer Vars for 'mod'");
//			return false;
//			break;
//		case 'd':
//      serror("Expected intger Vars for 'div'");
//      return false;
//      break;
//        default:
//            serror("float_arith::reached unreachable code.");
//            return false;
//            break;
//	}
//
//	return true;
//}
//
////приведение типа INTEGER к типу REAL
//
//
//
///* Выполнение специфицированной арифметики.
//Результат записывается в r */
//bool arith( char op, struct Var* r, struct Var* h )
//{
//
//
//	if ( r -> type != h-> type )//если нужно приведение типов.
//	{
//		struct variable* lowest;
//		if( r->type == INTEGER ) lowest = r;
//		else if ( h->type == INTEGER ) lowest = h;
//		else
//		{
//			serror("arith::Expected struct variable with type = INTEGER or REAL" );
//			return false;
//		}
//
//		lowest -> type = REAL;
//		int d = *(int*)(lowest->var);
//		free( lowest->var );
//		lowest -> var = malloc( sizeof( float ) );
//		*(float*)(lowest->var) = (float)d;
//
//	}
//
//
//	if ( r-> type == INTEGER )
//	{
//	    if ( op == '/' )
//	    //При делении integer-ов в Паскале не происходит отбрасывания дробной части,
//	    //а просиходит переход к типу данных real.
//	    {
//	        r->type = REAL;
//	        float result = (float)(r->value) / (h->value));
//	        free( r-> var );
//	        r->var = malloc ( sizeof( float ) );
//	        *((float*)(r->var)) = result;
//	        return true;
//	    }
//	    else return int_arith( op, (int*)(r->var), (int*)(h->var) );
//	}
//
//	else if ( r -> type == REAL )
//		return float_arith( op, (float*)(r->var), (float*)(h->var) );
//	else
//	{
//		serror("arith::Expected struct variable with type = INTEGER or REAL (2)");
//		return false;
//	}
//
//
//}
//
///* Изменение знака */
//bool unary( char o, struct variable* r )
//{
//	if ( o == '-' )
//	{
//		if ( r-> type  == INTEGER )
//			*(int*)(r->var) = -*(int*)(r->var);
//		else if ( r->type == REAL )
//			*(float*)(r->var) = -*(float*)(r->var);
//		else
//		{
//			serror("unary: Expected struct variable with type = INTEGER or REAL");
//			return false;
//		}
//	}
//	return true;
//}
//







#endif // EXPRESSIONS_H_INCLUDED
