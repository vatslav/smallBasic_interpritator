#include "operator.h"



extern struct context GLOBALS;//Из подсистемы variables. Глобальный контекст.
/* Инициализация интерпретатора */
void InitInterpreter()
{
    START_SUBSYSTEM_VARIABLES();//запуск подсистемы хранения переменных
}


//#ifdef WIN
//#include <io.h>
//int win_filesize( char* name )
//{
//	struct _finddata_t fData;
//	int res = _findfirst(argv[1], &fData);
//	return fData.size;
//}
//#endif

int FILESIZE( FILE*file)
{
    long current_position = ftell( file );//Сохранили текущую поизицию в файле.

    fseek( file, 0, SEEK_END ); // Переезжаем в конец файла.
    long size = ftell(file );
    fseek( file, current_position, SEEK_SET );//Возвращаемся на старую позицию.

    return size;
}



#include "variables.h"
#include "description_part.h"




void error_line_out( char* program )
{
    printf("Error is in line: ");
    while (*prog!='\n' && prog != program )
    {
        prog--;
    }
    if (*prog!='\0' && prog != program) prog++;
    while(*prog!='\n' && *prog)
    {
        printf("%c",*prog);
        prog++;
    }
    while( getchar() != '\n' );//Что-то похожее на readln;
}

int main( int argc, char* argv[] )
{
    FILE *file=0;
    long nFileLen = 0;
    //struct _finddata_t fData;
    //int res;

    if (argc<2)
    {
        file = fopen("in.pas","rb");

        if (file==0)
        {
            serror("Can't open file in.pas");
            while( getchar() != '\n' );
            return 1;
        }
        //res = _findfirst("in.pas", &fData);
    }

    else if (argc>2)
    {
        printf("usage: %s [filename]", argv[0] );
        while( getchar() != '\n' );
        return 1;
    }
    else if ((file = fopen(argv[1],"rb"))==0)
    {
        serror("Can't open file.");
        while( getchar() != '\n' );
        return 1;
    }
        //res = _findfirst(argv[1], &fData);

    //nFileLen = fData.size;
    nFileLen = FILESIZE( file );

    char* program = (char*)malloc( nFileLen + 1 );
    fread( program, 1, nFileLen, file );

    program[ nFileLen ] = '\0';


    fclose( file );

    prog = program;
    InitInterpreter();

    get_token();
    if ( token_type == ERROR )
    {
        error_line_out( program );
        return 1;
    }
    while ( tok!=BEGIN)
    {
        if( tok == VAR )
        {
            if ( !ReadVAR() )
            {
                serror("Error in VAR");
                error_line_out( program );
                return 1;
            }
            get_token();
        }

        else if ( tok ==  CONST )
        {
            if ( !ReadCONST() )
            {
                serror("Error in CONST");
                error_line_out( program );
                return 1;
            }
            get_token();
        }
        else if ( tok != BEGIN )
        {
        	serror("Expected BEGIN section!");
					error_line_out( program );
					return 1;
        }
    }

        if ( ! ExecCompoundOperator() )
        {
            error_line_out( program );
            return 1;
        }

        get_token();
        if (*token != '.' )
        {
					serror("Expected  '.'   !");
					error_line_out( program );
					return 1;
        }

    return 0;
}

