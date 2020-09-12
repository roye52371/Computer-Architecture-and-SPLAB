
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;
virus* readVirus(FILE* file);
void printVirus(virus* virus, FILE* output);
void PrintHex(unsigned char* buffer, unsigned short length,FILE *file);
typedef struct link link;
link* LoadSignatures();
struct link {
    link *nextVirus;
    virus *vir;
};
void virus_free(virus* vr);
void list_print(link *virus_list, FILE* file);
/* Print the data of every link in list to the given stream. Each item followed by a newline character. */

link* list_append(link* virus_list, virus* data);
/* Add a new link with the given data to the list
   (either at the end or the beginning, depending on what your TA tells you),
   and return a pointer to the list (i.e., the first link in the list).
   If the list is null - create a new entry and return a pointer to the entry. */

void list_free(link *virus_list); /* Free the memory allocated by the list. */

void quite();

void detect_virus(char *buffer, unsigned int size, link *virus_list);

int main(int argc, char **argv) {


    //char* tmpTopreventMemoryLose;
    // struct fun_desc menu[]= {{"censor",censor},{"encrypt",encrypt},{"decrypt",decrypt},{"dprt",dprt},{"cprt",cprt},{"my_get",my_get},{"quit",quit},{"junk",menu},{NULL,NULL}};
    //int bounds=sizeof(menu)/ sizeof(struct fun_desc)-2;//with Null and counting from 0 so one less more
    int bounds=4;
    int looptillExit=1;
    int decision;
    link* link=NULL;
    //int loopcounter=0;
    char strforFunction[1024];// get the function number from the user, max size 1024 from the forum
    while(looptillExit==1) {
        printf("Please choose a function:\n");
        printf("1) Load signatures\n");
        printf("2) Print signatures\n");
        printf("3) Detect viruses\n");
        printf("4) Quit\n");
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
        if (decision >= 1 && decision <= bounds) {
            printf("Within bounds\n\n");
           // char cleanbuffer2='0';
            if(decision==1){
                if(link!=NULL){
                    list_free(link);// if tried second time to load new file now
            }
                 link=LoadSignatures();
                char cleanbuffer2='0';
                while(cleanbuffer2!='\n'){
                    cleanbuffer2=fgetc(stdin);
                }
            }
            else if(decision==2){
                if(link!=NULL)
                    list_print(link,stdout);
               // list_free(link);
            }
            else if(decision==3){

                char buffer[625<<4];//10kchars(10k bytes)
                if(argc>=2) {
                    FILE *fs;
                    fs = fopen(argv[1], "r");
                    if (fs == NULL) {
                        fprintf(stderr, "file does not exist\n");

                    }
                    else{
                        int length = 0;
                        while (fgetc(fs) != EOF) {
                            length = length + 1;
                        }
                        // unsigned char buffer[length];
                        fseek(fs, 0, SEEK_SET);
                        fread(buffer,length,1,fs);
                        unsigned int minSize=0;
                        if(length<10000){
                            minSize=length;
                        }
                        else{
                            minSize=10000;
                        }
                        detect_virus(buffer, minSize, link);
                    }
                    fclose(fs);
                }
                else{
                    fprintf(stderr, "this suspect file does not exist\n");
                }

            }
            else// 4
            {
                list_free(link);
                quite();
            }
            printf("DONE.\n\n");
        } else {
            printf("Not within bounds\n\n");
            exit(0);
        }

        //loopcounter=1;
        // free(carray);
    }

    return 0;
}//63 68 65 63 6B AA DD 4D 79 0C 48 65 78
// assistance function:
virus* readVirus(FILE* file){
   virus* virus_var= malloc(sizeof(virus));
    if(fread(virus_var,18,1,file)!=1){
    free(virus_var);   
	return NULL;
    }
	//printf("%d",virus_var->SigSize);
    //unsigned char sigi[size];
	virus_var->sig=malloc(sizeof(virus_var->sig)*virus_var->SigSize);
    if(fread(virus_var->sig,virus_var->SigSize,1,file)!=1){
    free(virus_var->sig);  
    free(virus_var);
	return NULL;
    }
    //virus_var->sig=sigi;
    return virus_var;
}
void printVirus(virus* virus, FILE* output){
    fprintf(output,"Virus name: ");
    fprintf(output,"%s",virus->virusName);
    fprintf(output,"\n");
    fprintf(output,"Virus size: ");
    fprintf(output,"%d",virus->SigSize);
    fprintf(output,"\n");
    fprintf(output,"Signatures:\n");
    //fprintf(output,"%02X",virus->sig);
    PrintHex(virus->sig,virus->SigSize, output);
    fprintf(output,"\n");

}
void PrintHex(unsigned char* buffer, unsigned short length,FILE *file){
    for (int i = 0; i < length ; i=i+1) {
        fprintf(file,"%02X",buffer[i]);
        if(i!= length-1)
            fprintf(file," ");
    }
    //fprintf(file," \n");

}



void list_print(link *virus_list, FILE* file){
    link *virus_temp=virus_list;
    while(virus_temp!=NULL){
        printVirus(virus_temp->vir,file);
        fprintf(file,"\n");
       // virus_free(virus_temp->vir);
        virus_temp=virus_temp->nextVirus;
    }
   // list_free(virus_temp);
}

link* list_append(link* virus_list, virus* data) {
   link* NewLink=malloc(sizeof(link));
    NewLink->nextVirus=virus_list;
    NewLink->vir=data;
    return NewLink;

}


void list_free(link *virus_list){
    if(virus_list==NULL)
        return;
    /*
    else {
        link *currentLink = virus_list;

        do {
            link* nextLink = virus_list->nextVirus;
            virus_free(currentLink->vir);
            free(currentLink);
            currentLink = nextLink;
        }while (currentLink != NULL);
    }
    */
    else {
        list_free(virus_list->nextVirus);
        virus_free(virus_list->vir);
        free(virus_list);
    }
}
void virus_free(virus* vr){
    free(vr->sig);
    free(vr);

}
link* LoadSignatures(){
    printf("please enter filename to Load signatures from:");
    char inputfileName[128];
    scanf("%s",inputfileName);
    FILE *fi;
    fi=fopen(inputfileName,"r");
    if(fi==NULL) {
        fprintf(stderr, "file does not exist\n");
       // exit(0);
        return NULL;
    }
    int length=0;
    while(fgetc(fi)!=EOF){
        length=length+1;
    }
    // unsigned char buffer[length];
    fseek(fi, 0, SEEK_SET);
    link* virus_link=NULL;
    virus* virus_var=NULL;
    while((virus_var=readVirus(fi))!=NULL){
        virus_link=list_append(virus_link,virus_var);

    }
    fclose(fi);
    return virus_link;

}

void quite(){
    exit(0);
}
void detect_virus(char *buffer, unsigned int size, link *virus_list){
    //unsigned int startingByte=0;
    if(virus_list!=NULL) {
        for (int startingByte = 0; startingByte <size-virus_list->vir->SigSize+1 ; startingByte=startingByte+1)
        {
            if (memcmp(buffer + startingByte, virus_list->vir->sig, virus_list->vir->SigSize) == 0) {
                fprintf(stdout, "starting byte in buffer:%d\nvirus name:%s\nsize of virus signature:%d\n\n", startingByte,
                        virus_list->vir->virusName, virus_list->vir->SigSize);
            }

        }
        detect_virus(buffer,size,virus_list->nextVirus);
    }

}

