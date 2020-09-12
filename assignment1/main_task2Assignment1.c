#include <stdio.h>
#include <string.h>
#include <stdlib.h>//for the use of exit(0)
#define	MAX_LEN 34			/* maximal input string size */
					/* enough to get 32-bit string + '\n' + null terminator */
extern int convertor(char* buf);

int main(int argc, char** argv)
{
  char buf[MAX_LEN];
  while(!(strlen(buf)==2 && buf[0]=='q')) {
    fgets(buf, MAX_LEN, stdin);		/* get user input string */ 
    for (int i = 0; i < strlen(buf) - 1; ++i)
    {
    	if((buf[i] < 48 || 57 < buf[i]) && buf[i]!='q'){
    		printf("ERROR\n");
    		exit(0);
    	}
    }
    if( buf[0]!='q')
        convertor(buf);			/* call your assembly function */
  }
  return 0;
}
