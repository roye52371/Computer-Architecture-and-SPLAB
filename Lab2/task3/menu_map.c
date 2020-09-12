#include <stdlib.h>
#include <stdio.h>
#define LowHexa 32  // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
#define HighHexa 126 // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E

struct fun_desc {
    char *name;
    char (*fun)(char);
};

char encrypt(char c); /* Gets a char c and returns its encrypted form by adding 3 to its value.
          If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c); /* Gets a char c and returns its decrypted form by reducing 3 to its value.
            If c is not between 0x20 and 0x7E it is returned unchanged */
char dprt(char c); /* dprt prints the value of c in a decimal representation followed by a
           new line, and returns c unchanged. */
char cprt(char c); /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns
                    the value of c unchanged. */
char my_get(char c); /* Ignores c, reads and returns a character from stdin using fgetc. */

char quit(char c); /* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */

char censor(char c) {
    if(c == '!')
        return '.';
    else
        return c;
}

void map(char *array, int array_length, char (*f) (char)){
   
    for (int i = 0; i <array_length ; i=i+1) {
        *(array+i)=f(*(array+i));
    }
    
}

struct fun_desc menu[]= {{"Censor",censor},{"Encrypt",encrypt},{"Decrypt",decrypt},{"Print dec",dprt},{"Print string",cprt},{"Get string",my_get},{"Quit",quit},{"junk",menu},{NULL,NULL}};

int main(int argc, char **argv){

    int base_len = 5;
    char* carray=(char*)(malloc(base_len*sizeof(char)));
   // carray="";
   carray[0]='\0';

    //char* tmpTopreventMemoryLose;
   // struct fun_desc menu[]= {{"censor",censor},{"encrypt",encrypt},{"decrypt",decrypt},{"dprt",dprt},{"cprt",cprt},{"my_get",my_get},{"quit",quit},{"junk",menu},{NULL,NULL}};
    int bounds=sizeof(menu)/ sizeof(struct fun_desc)-2;//with Null and counting from 0 so one less more
    int looptillExit=1;
    int decision;
    //int loopcounter=0;
    char strforFunction[1024];// get the function number from the user, max size 1024 from the forum 
    while(looptillExit==1) {
        printf("Please choose a function:\n");
        for (int i = 0; i < bounds + 1; i=i+1) {// without NULL and we need index of bounds so write bounds+1
            printf("%d) %s\n", i, menu[i].name);

        }
        printf("Option: ");
        fgets(strforFunction, 25,stdin);// to high to prevent overflow when using fgets afterwards ' a said in the forum(to putt a size less than the array beacause it could be to much
       
        //printf("Option: ");
        decision=atoi(strforFunction);
      /*  char cleanbuffer='0';
        if(decision==5){
            while(cleanbuffer!='\n'){
                cleanbuffer=fgetc(stdin);
            }
        }*/
        
        //printf("Option: %d\n", decision);
        if (decision >= 0 && decision <= bounds) {
            printf("Within bounds\n");
            int number = decision;
            map(carray, base_len, menu[number].fun);
            char cleanbuffer2='0';
            if(decision==5 && *(carray+4)!='\n'){
                while(cleanbuffer2!='\n'){
                    cleanbuffer2=fgetc(stdin);
                }
            }
            printf("DONE.\n\n");
        } else {
            printf("Not within bounds\n");
            exit(0);
        }
        
        //loopcounter=1;
       // free(carray);
    }
    return 0;
}
char encrypt(char c){
    if(c>=LowHexa && c<=HighHexa){   // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
        c=c+3;
    }
    return c;
}

char decrypt(char c){
    if(c>=LowHexa && c<=HighHexa){   // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
        c=c-3;
    }
    return c;
}

char dprt(char c){
    printf("%d\n",(int)c);
    return c;
}
char cprt(char c){
    if(c>=LowHexa && c<=HighHexa){// 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
       // char t= atoi(c)+'0';// print the char that has the vlue that c represent(c represent number)
        printf("%c\n",c);
    }
    else{
        printf(".\n");
    }
    return c;
}

char my_get(char c){
    char ch= fgetc(stdin);
    return ch;
}


char quit(char c){
    if(c=='q'){
        exit(0);
    }
    else
        return c;
}
