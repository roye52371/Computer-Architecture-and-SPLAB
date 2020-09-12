#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LowHexa 32  // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
#define HighHexa 126 // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
//int DebugMode=0;
#include <libgen.h> // Prototype for basename() function
#define modulofaAscci 256

typedef struct {
    char debug_mode;
    char file_name[128];
    int unit_size;
    unsigned char mem_buf[10000];
    size_t mem_count;
    char* filenameforFopen;
    int DisplayFlag;
    /*
     .
     .
     Any additional fields you deem necessary
    */
} state;
void SaveIntoFile(state* s);
void quit(state * s); /* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
void ToggleDebugMode(state *s);
void SetFileName(state *s);
void SetUnitSize(state *s);
void LoadIntoMemory(state* s);
void ToggleDisplayMode(state* s);
void MemoryDisplay(state* s);
void print_units(FILE* output, char* buffer, int count,int unit_size);
void print_Dec_units(FILE* output, char* buffer, int count,int unit_size);
char* unit_to_format(int unit);
char* unit_to_Dec_format(int unit);
void write_units(FILE* output, char* buffer, int count,int unit_size);
void MemoryModify(state* s);
struct fun_desc {
    char *name;
    void (*fun)(state*);
};

void map(state* s ,void (*f) (state*)){
    f(s);

}

struct fun_desc menu[]= {{"Toggle Debug Mode",ToggleDebugMode},{"Set File Name",SetFileName},{"Set Unit Size",SetUnitSize},{"Load Into Memory",LoadIntoMemory},{"Toggle Display Mode",ToggleDisplayMode},{"Memory Display",MemoryDisplay},{"Save Into File",SaveIntoFile},{"Memory Modify",MemoryModify},{"Quit",quit},{NULL,NULL}};

int main(int argc, char **argv){

    //int base_len = 5;
    //char* carray=(char*)(malloc(base_len*sizeof(char)));
    state* s= (state*)(calloc(1, sizeof(state)));
    s->debug_mode='0';
    s->unit_size=1;//default
    s->file_name[0]='\0';//putting null for beggining
    s->filenameforFopen=calloc(128, sizeof(char));
    s->DisplayFlag=0;
    s->mem_buf[0]='\0';



    // carray="";
    //carray[0]='\0';

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
        fseek(stdin,0,SEEK_SET);
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
            map(s, menu[number].fun);

            /*  if(decision==5 && *(s+4)!='\n'){
                  while(cleanbuffer2!='\n'){
                      cleanbuffer2=fgetc(stdin);
                  }
              }*/
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



void quit(state* s){
    if(s->debug_mode=='1'){
        printf("quitting\n");
    }
    free(s->filenameforFopen);
    free(s);
    exit(0);
}
void ToggleDebugMode(state *s){
    if(s->debug_mode=='1'){
        s->debug_mode='0';
        printf("Debug flag now off\n");
    }

    else{
        s->debug_mode='1';
        printf("Debug flag now on\n");
    }
}

void SetFileName(state* s){
    printf("please enter a file name\n");
    fgets(s->file_name, 100,stdin);
    if(strlen(s->file_name)>0) {
        s->file_name[strlen(s->file_name) - 1] = '\0';
    }
    strncpy(s->filenameforFopen,s->file_name, sizeof(s->file_name)-1);
    if(s->debug_mode=='1'){
        printf("file name set to %s\n",s->file_name);
    }

}

void SetUnitSize(state *s){
    char strforFunction[1024];// get the function number from the user, max size 1024 from the forum
    printf("please enter a number\n");
    fgets(strforFunction, 25,stdin);// to high to prevent overflow when using fgets afterwards ' a said in the forum(to putt a size less than the array beacause it could be to much

    //printf("Option: ");
    int decision=atoi(strforFunction);
    if(decision==1 || decision==2 || decision==4){
        s->unit_size=decision;
        if(s->debug_mode=='1'){
            printf("set unit size to %d\n",s->unit_size);
        }

    }
    else{
        printf("invalid unit number\n");
    }
}

void LoadIntoMemory(state* s){
    if(strcmp(s->file_name,"")==0){
        fprintf(stderr,"file_name field is NULL\n");
        return;
    }
    else{
        FILE* fp=fopen(s->filenameforFopen,"rb");
        if(fp==NULL){
            fprintf(stderr,"an error accured when tried to open\n");
        }
        else{
            char fileSizeBuffer[10000];
            printf("please enter location and length\n");
            int locat;
            int lengt;
            fgets(fileSizeBuffer, sizeof(fileSizeBuffer),stdin);
            sscanf(fileSizeBuffer,"%X %d",&locat, &lengt);
            if(s->debug_mode=='1'){
                printf("filename:%s , location: %02X, length: %d \n",s->file_name,locat,lengt);
            }
            fseek(fp,locat,SEEK_SET);
            fread(s->mem_buf,s->unit_size,lengt,fp);
            int sum=s->unit_size*lengt;
            printf("Loaded %d units into the memory\n",sum);
        }
        fclose(fp);
    }

}

void ToggleDisplayMode(state* s){
    if(s->DisplayFlag==0){
        s->DisplayFlag=1;
        printf("Display flag now on, hexadecimal representation\n");
    }
    else{
        s->DisplayFlag=0;
        printf("Display flag now off, decimal representation\n");
    }
}
void MemoryDisplay(state* s){
    char fileSizeBuffer[10000];
    printf("please enter u units and addr\n");
    int adrr;
    char* addressArray=calloc(100, sizeof(char));
    int u;
    fgets(fileSizeBuffer, sizeof(fileSizeBuffer),stdin);
    sscanf(fileSizeBuffer,"%d %X", &u,&adrr);

    if(s->DisplayFlag==0){
        if(adrr==0){
            print_Dec_units(stdout,(char*)s->mem_buf,u,s->unit_size);
        }else{
            print_Dec_units(stdout,addressArray,u,s->unit_size);
        }
    }
    else{//hexa print
        if(adrr==0){
            print_units(stdout,(char*)s->mem_buf,u,s->unit_size);
        }
        else{
            print_units(stdout,addressArray,u,s->unit_size);
        }
    }
    free(addressArray);
}

void print_Dec_units(FILE* output, char* buffer, int count,int unit_size) {
    char* end = buffer + unit_size*count;
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(output, unit_to_Dec_format(unit_size), var);
        buffer += unit_size;
    }
}
void print_units(FILE* output, char* buffer, int count,int unit_size) {
    char* end = buffer + unit_size*count;
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(output, unit_to_format(unit_size), var);
        buffer += unit_size;
    }
}
char* unit_to_Dec_format(int unit) {
    static char *Dec_Formats[] = {"%hhd\n", "%hd\n", "No such unit", "%d\n"};
    return Dec_Formats[unit - 1];
}
char* unit_to_format(int unit) {
    static char* formats[] = {"%hhX\n", "%hX\n", "No such unit", "%X\n"};
    return formats[unit-1];
    /* If the above is too confusing, this can also work:
    switch (unit_size) {
        case 1:
            return "%#hhx\n";
        case 2:
            return "%#hx\n";
        case 4:
            return "%#hhx\n";
        default:
            return "Unknown unit";
    }
    */
}
void write_units(FILE* output, char* buffer, int count,int unit_size) {
    fwrite(buffer, unit_size, count, output);
}

void SaveIntoFile(state* s){
    FILE * fpp;

    int srcAdd;
    int trgLoc;
    int leng;
    char* AdresArray=calloc(100,sizeof(char));
    char buff[10000];


    fpp=fopen(s->file_name,"rb+wb");
    if(fpp==NULL){
        if(s->debug_mode=='1'){
            fprintf(stderr,"error accure when open for write and not truncate\n");
        }
        return;
    }
    fseek(fpp,0,SEEK_END);
    int fpSizze=ftell(fpp);
    fseek(fpp,0,SEEK_SET);

    printf("please enter sourceAdress in hex, TargetLocation in hex and length in decimal\n");
    fgets(buff, sizeof(buff), stdin);
    sscanf(buff, "%X %X %d", &srcAdd, &trgLoc, &leng);
    //checking validity
    if (fpSizze < (trgLoc + (leng * s->unit_size))){
        fprintf(stderr, "INVALID TARGET,  trgLOc is bigger than our file descriptor size.\n");
        fclose(fpp);// TO PREVENT UNCONDITIONALJUMP!!!
        return;
    }
    fseek(fpp,trgLoc,SEEK_SET);
    if(srcAdd!=0){
        sprintf(AdresArray,"%X",srcAdd);
        write_units(fpp,(char*)AdresArray,leng,s->unit_size);
    }
    else{
        write_units(fpp,(char*)s->mem_buf,leng,s->unit_size);
    }


    fclose(fpp);
    free(AdresArray);
}

void MemoryModify(state* s){

    int Loc;
    int OurValue;
    char buff[10000];

    printf("please enter location in hex, a Value in hex\n");
    fgets(buff, sizeof(buff), stdin);
    sscanf(buff, "%X %X", &Loc, &OurValue);
    if (strlen((char*)s->mem_buf) < (Loc +  s->unit_size)){
        fprintf(stderr, "INVALID TARGET,  trgLOc is bigger than our file descriptor size.\n");
        // fclose(fpp);// TO PREVENT UNCONDITIONALJUMP!!!
        return;
    }

    if(s->debug_mode=='1'){
        printf("our Location is:%X our Value is:%X\n",Loc,OurValue);
    }
    for (int i = 0; i < s->unit_size ; ++i) {
        s->mem_buf[Loc+i]=OurValue%modulofaAscci;// putting one byte off ascci values
        OurValue=OurValue/modulofaAscci;
    }
    //fseek(fpp,Loc,SEEK_SET);
    //fwrite(&OurValue,1,s->unit_size,fpp);
    //fclose(fpp);

}
