#include "operator.h"



extern struct context GLOBALS;//Из подсистемы variables. Глобальный контекст.
/* Инициализация интерпретатора */
void InitInterpreter()
{
	START_SUBSYSTEM_VARIABLES();//запуск подсистемы хранения переменных
}


//int FILESIZE( FILE*file)
//{
//	long current_position = ftell( file );//Сохранили текущую поизицию в файле.
//
//	fseek( file, 0, SEEK_END ); // Переезжаем в конец файла.
//	long size = ftell(file );
//	fseek( file, current_position, SEEK_SET );//Возвращаемся на старую позицию.
//
//	return size;
//}
//


#include "variables.h"
#include "description_part.h"
#include <io.h>



void error_line_out( char* program )
{
  while (*prog!='\n' && prog != program )
  {
     prog--;
  }
  prog++;
  while(*prog!='\n' && prog)
  {
    printf("%c",*prog);
    prog++;
  }
}

int main( int argc, char* argv[] )
{
  FILE *file;
  long nFileLen = 0;
  struct _finddata_t fData;
  int res;

	if (argc<2)
	{
		file = fopen("in.pas","rb");

		if (file==0)
		{
			serror("Can't open file in.pas");
			return 1;
		}
		res = _findfirst("in.pas", &fData);
	}

	if (argc>2)
	{
		printf("usage: %s [filename]", argv[0] );
		return 1;

    if ((file = fopen(argv[1],"rb"))==0)
    {
      serror("Can't open file.");
      return 1;
    }
    res = _findfirst(argv[1], &fData);
	}
  nFileLen = fData.size;

	char* program = (char*)malloc( nFileLen + 1 );
	fread( program, 1, nFileLen, file );

	program[ nFileLen ] = '\0';

  fclose( file );

	prog = program;
	InitInterpreter();

	get_token();
	if( tok == VAR )
	{
	  if ( !ReadVAR() )
	  {
	    serror("Error in VAR");
	    error_line_out( program );
	    return 1;
	  }
	}
//	get_token();
//	if ( token[0] != ';' )
//	{
//	  serror("Expected ;");
//	  error_line_out( program );
//	  return 1;
//	}
//	get_token();
//	if ( tok ==  CONST )
//	{
//	  if ( !ReadCONST() )
//	  {
//	    serror("Error in CONST");
//	    error_line_out( program );
//	    return 1;
//	  }
//	}
//  get_token();
//	if ( token[0] != ';' )
//	{
//	  serror("Expected ;");
//	  error_line_out( program );
//	  return 1;
//	}

	get_token();

	if ( tok == BEGIN )
	{
	  if ( ! ExecCompoundOperator() )
	  {
	    error_line_out( program );
	    return 1;
	  }
	}


	return 0;
}

