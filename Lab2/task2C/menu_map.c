#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LowHexa 32 // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
#define HighHexa 126 // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E

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

int main(int argc, char **argv){

    int base_len = 5;
    char arr1[base_len];
    map(arr1, base_len, my_get);
    map(arr1,base_len,quit);
    map(arr1, base_len, encrypt);
    map(arr1, base_len, dprt);
    map(arr1, base_len, decrypt);
    map(arr1, base_len, cprt);

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
