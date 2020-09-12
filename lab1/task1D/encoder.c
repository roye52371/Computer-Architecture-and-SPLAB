#include <stdio.h>
#include <string.h>
// declaring assistance function
void workfunction(int argc, char **argv,int keyCounter,int isDebug, int isOfile,int isMinusifile,int isencrypted,int indexoffile,int indexofMinusi,int idexofe,int isPlus,int isMinus,int idexforCycle,char* filename,char* filenametoReadFrom,int letter, FILE *fp,FILE *fi);

int main(int argc, char **argv) {
    int isDebug=0;// flag for -D mode FILE *fp
    int isOfile=0;//flag for -o mode
    int isMinusifile=0;//flag for -i mode
    int isencrypted=0;//flag for e mode
    int indexoffile=0;// index of -o mode in command line
    int indexofMinusi=0;// index of -i mode in command line
    int idexofe=0;// index of e mode in command line
    int isPlus=0;//flag for +e mode
    int isMinus=0;////flag for -e mode
    int idexforCycle=2;//after operator and e, cycle for round robin while encrypting chars
    int isErrorfile=0;// check if error accured
	int pd=0;
	//int po=0;
	//int pe=0;
	//int pi=0;
	
    for(int i=1; i<argc; i++){
        if(strncmp(argv[i],"-o",2)==0){
            isOfile=1;
            indexoffile=i;
	
        }
	if(strncmp(argv[i],"-i",2)==0){
            isMinusifile=1;
            indexofMinusi=i;
	
        }

        if(strcmp(argv[i],"-D")==0){
            isDebug=1;
		if(pd==0){
		printf("-D\n");
		pd=1;
	}
	
        }
        if( strncmp(argv[i],"+e",2)==0) {
            isencrypted = 1;
            idexofe = i;
            isPlus = 1;
        }
        if(strncmp(argv[i],"-e",2)==0) {
            isencrypted = 1;
            idexofe= i;
            isMinus = 1;
        }

    }

    int keyCounter= strlen(argv[idexofe]);// length of key in e operation
    FILE *fp;//for -o
    FILE *fi;//for -i
    int letter=0;// reading char by char from file, initalized now beacuse if not i get warning


    char* filename=argv[indexoffile]+2;// when -o
    char* filenametoReadFrom=argv[indexofMinusi]+2;// when -i

    if(isOfile){// open or create filel when -o mode (writing to it)
        fp = fopen(filename, "w");
    }
    if(isMinusifile)//// open or create filel when -i mode(reading from it), throwing error if does not exist
    {
        fi = fopen(filenametoReadFrom, "r");
        if(fi==NULL)
        {
            fprintf(stderr,"file does not exist");
            isErrorfile=1;// error accured
        }

    }



    if(isErrorfile!=1){// if ERROR eccured we need to exit so we do nothing
        workfunction(argc, argv,keyCounter,isDebug, isOfile,isMinusifile,isencrypted,indexoffile,indexofMinusi,idexofe,isPlus,isMinus,idexforCycle,filename,filenametoReadFrom,letter, fp,fi);
    }


    return 0;
}
// assistance function:

void workfunction(int argc, char **argv,int keyCounter, int isDebug, int isOfile,int isMinusifile,int isencrypted,int indexoffile,int indexofMinusi,int idexofe,int isPlus,int isMinus,int idexforCycle,char* filename,char* filenametoReadFrom,int letter,FILE *fp,FILE *fi){
    do{
    if(isMinusifile!=1){
        letter=fgetc(stdin);
	}
    if(isMinusifile==1){
        letter=fgetc(fi);
	}

        int oldchar = letter;

        if (isencrypted == 0 && letter >= 65 && letter <= 90)// need only to turn to apper case
            letter = letter + 32;
       if (isDebug == 1 && isencrypted == 0  &&letter != EOF) {//need only id -D was given
            if(letter == 10)
            {
                fprintf(stderr, "\n");
            }
            else {
                fprintf(stderr, "%d" "\t" "%d\n", oldchar, letter);
            }
        }
        if (isencrypted == 1) {
            char ch;
            if (letter == '\n')
                ch = '\n';
            if (isPlus && letter != '\n') {
                ch = (char) (letter + ((int) (argv[idexofe][idexforCycle]) - 'A'+1));
            } else if (isMinus == 1 && letter != '\n')
                ch = (char) (letter - ((int) (argv[idexofe][idexforCycle]) - 'A'+1));
            idexforCycle = idexforCycle + 1;
            if (idexforCycle == keyCounter)//beacuse idecofcycle begin from 2
                idexforCycle = 2;//back to start of KEY
            if (isDebug == 1  && letter != EOF) {
                if (letter == 10) {
                    fprintf(stderr, "\n");
                }
                else {
                    fprintf(stderr, "%d" "\t" "%d\n", oldchar, (int) ch);
                }
            }
            if (isOfile == 0 && letter != EOF)
                fprintf(stdout, "%c", ch);
            if (isOfile == 1 && letter != EOF) {

                fp = fopen(filename, "a");// a is for adding to file when the file allready have text and not empty
                fprintf(fp, "%c", ch);
                fclose(fp);
            }

            if (letter == '\n') {
                //printf("\n");//check if needed
                idexforCycle = 2;
            }
        } else//not encrypted
        if (isOfile == 0 && isencrypted == 0 && letter != EOF)
            fprintf(stdout, "%c", (char) letter);
        if (isOfile == 1 && isencrypted == 0 && letter != EOF) {
            fp = fopen(filename, "a");
            fprintf(fp, "%c", (char) letter);
            fclose(fp);
        }



}while(letter!=EOF);// the case of letter!=EOF when we read from file and not from stdin


}
