#ifndef GLOBAL_DEFINES
#define GLOBAL_DEFINES


/******************** В основном для get_token **********************/
/***типы лексем ( token_type  ): ***/
#define DELIMITER 1
#define RESERVED 2
#define OPERATOR 3
#define OPERATION 4
#define FUNCTION 5
#define VARIABLE 6
#define CONSTANT 7
#define REAL_NUMBER 8
#define INT_NUMBER 9
#define STRING 10
#define FINISHED 11
#define ERROR 12

#define IDENTIFIER 13 //служебный тип токена. Возвращается для инентификаторов при "простом" режиме работы get_token() (* get_token в этом режиме не пытается разобраться с тем, идентификатор ли это переменной, константы или вообще неизвестный идентификатор. Просто говорит - идентификатор. *)

//NB! Про "сложный" и "простой" режим работы get_token читай в начале файла globals.h около объявления bool SimpleWork.

// Напомню, что идентификатор - объявленное пользователем имя. В нашем случае идентификаторы при нормальном ( "сложном" ) режиме работы get_token будут относиться к типу VARIABLE, CONST, ну или ERROR для неизвестных идентификаторов.

//NB! Для Вас этот define не существует и get_token никогда не возвращает токены такого типа. Вы всегда работаете в "сложном" режиме get_token. "Сложный" режим - это поведение, расказанное в ОПИСАНИИ интерпретатора.


/***Внутреннее представление лексем ( tok ): ***
0 - нет представления */
#define WRITELN 10
#define WRITE 11

#define READLN 20
#define READ 21

#define IF 30
#define THEN 31
#define ELSE 32

#define FOR 40
#define TO 41
#define DOWNTO 42
#define DO 43

#define WHILE 44

#define REPEAT 45
#define UNTIL 46

#define BEGIN 50
#define END 51

#define ASSIGNMENT 70 //присваивание :=

#define INTEGER 100 //C int
#define REAL 101 // C float
#define ARRAY 102
#define OF 103


#define CONST 1
#define VAR 2

#define EXP 60
#define LN 61
#define SIN 62
#define COS 63

#define TRUE 1
#define FALSE -1

////////////////////////////////////


//////// Объявления, необходимые для языка С, чтобы работать с типом boolean /////////////////
#define bool int//для C.
#define true 1
#define false 0


#endif //GLOBAL_DEFINES
