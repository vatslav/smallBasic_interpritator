#include <iostream>
#include "get_token.h"
//#include "mathematica.h"

using namespace std;

int main()
{
    prog = "(1243+236)+43.36";
    printf("String = %s\n\nToken\tToken_type\tTok\n",prog);
    while(1)
	{
		get_token();
		printf("%s \t%d \t\t%d", token, token_type, tok );
		getchar();
	}
   // get_token ();
    return 0;
}
