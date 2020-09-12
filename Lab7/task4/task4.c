

int the_digitCounter(char* str);

int main(int argc, char** argv){

    int counter = the_digitCounter(&argv[1][0]);

    return 0;
}

int the_digitCounter(char* str){
    int j=0;
    int counter = 0 ;
    while (str[j]!='\0'){
        if(str[j]>=48 && str[j]<=57){
            counter=counter+1;
        }
        j=j+1;
    }
    return counter;
}

