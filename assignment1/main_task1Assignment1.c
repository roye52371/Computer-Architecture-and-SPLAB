#include <stdio.h>
#define MAX_LEN 12			/* maximal input string size */


char c_checkValidity(int x, int y);
extern void assFunc(int number1,int number2);

int main(int argc, char** argv) {
    char fgs[MAX_LEN];
    int number1;
    fgets(fgs,MAX_LEN,stdin);//beacause fgets aadd to last ccharcater /000 and we need at most 10 charcters
    sscanf(fgs,"%d",&number1);
    //printf("%d\n",number1);
    //fgetc(stdin);
    char fgs1[MAX_LEN];
    int number2;
    fgets(fgs1,MAX_LEN,stdin);//beacause fgets aadd to last ccharcater /000 and we need at most 10 charcters
    sscanf(fgs1,"%d",&number2);
     //printf("%d\n",number2);
    assFunc(number1,number2);/* call your assembly function */
    return 0;
}

char c_checkValidity(int x, int y){
    if(x>=y)
        return 1;
    else
        return 0;
}
