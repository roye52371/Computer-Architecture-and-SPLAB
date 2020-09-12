#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#define LowHexa 32  // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
#define HighHexa 126 // 32 is decimal value of char represnted by 0x20 and 126 is for 0x7E
//int DebugMode=0;
#include <libgen.h> // Prototype for basename() function
#include <fcntl.h>
#include <sys/stat.h>
#include <zconf.h>
#include <sys/mman.h>

#define modulofaAscci 256
int currentFd;//at the beggining no valid file
int specialind(int index,Elf32_Sym* beginsymboltalb,int t,char* beginonetable);
void *map_start; /* will point to the start of the memory mapped file */
struct stat fd_stat; /* this is needed to  the size of the file */
Elf32_Ehdr *header; /* this will point to the header structure */
int num_of_section_headers;

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

void quit(state * s); /* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
void ToggleDebugMode(state *s);
void ExamineELFFile(state* s);
void the_PrintSectionNames(state* s);
void the_Print_Symbols(state* s);

struct fun_desc {
    char *name;
    void (*fun)(state*);
};

void map(state* s ,void (*f) (state*)){
    f(s);

}

struct fun_desc menu[]= {{"Toggle Debug Mode",ToggleDebugMode},{"Examine ELF File",ExamineELFFile},{"Print Section Names",the_PrintSectionNames},{"Print Symbols",the_Print_Symbols},{"Quit",quit},{NULL,NULL}};

int main(int argc, char **argv){

    //int base_len = 5;
    //char* carray=(char*)(malloc(base_len*sizeof(char)));
    // map_start=calloc(10, sizeof(char));//allocating enouhgt memory to address returned
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
    munmap(map_start, fd_stat.st_size);//check maybe if unmap succeed
    close(currentFd);
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

void ExamineELFFile(state* s){
    char filename[100];
    printf("please enter a file name\n");
    fgets(filename, 100,stdin);
    if(strlen(filename)>0) {
        filename[strlen(filename) - 1] = '\0';
    }
    // strncpy(s->filenameforFopen,s->file_name, sizeof(s->file_name)-1);
    if(s->debug_mode=='1'){
        printf("file name to examine: %s\n",filename);
    }

    currentFd=open(filename,O_RDWR);
    if(currentFd==-1){
        fprintf(stderr,"fd value is -1 when trying to open\n");
        exit(-1);
    }
    if( fstat(currentFd, &fd_stat) != 0 ) {
        currentFd=-1;
        perror("stat failed");
        exit(-1);
    }
    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, currentFd, 0)) == MAP_FAILED ) {
        perror("mmap failed");
        exit(-4);
    }
    header = (Elf32_Ehdr *) map_start;
    if(header->e_ident[EI_MAG1]!=ELFMAG1 || header->e_ident[EI_MAG2]!=ELFMAG2 || header->e_ident[EI_MAG3]!=ELFMAG3){
        fprintf(stderr,"the magic number/s is consistent with an ELF file\n");
        munmap(map_start, fd_stat.st_size);
        close(currentFd);
        currentFd=-1;
        exit(-1);
    }
    printf("1. Bytes 1,2,3 of the magic number:\t\t\t\t\t%c %c %c\n",header->e_ident[EI_MAG1],header->e_ident[EI_MAG2],header->e_ident[EI_MAG3]);
    printf("2. The data encoding scheme of the object file:\t\t\t\t");
    if(header->e_ident[EI_DATA]==ELFDATANONE){
        printf("Invalid data encoding\n");
    }
    else if(header->e_ident[EI_DATA]==ELFDATA2LSB){
        printf("2's complement, little endian\n");
    }
    else if(header->e_ident[EI_DATA]==ELFDATA2MSB){
        printf("2's complement, big endian\n");
    }
    else{
        fprintf(stderr,"invalid data enciding\n");
        exit(-1);
    }

    printf("3. Entry point:\t\t\t\t\t\t\t\t0x%x\n",header->e_entry);
    printf("4. The file offset in which the section header table resides:\t\t%d\n",header->e_shoff);
    printf("5. The number of section header entries:\t\t\t\t%d\n",header->e_shnum);
    printf("6. The size of each section header entry:\t\t\t\t%d\n",header->e_shentsize);
    printf("7. The file offset in which the program header table resides\t\t%d\n",header->e_phoff);
    printf("8. The number of program header entries:\t\t\t\t%d\n",header->e_phnum);
    printf("9. The size of each program header entry\t\t\t\t%d\n",header->e_phentsize);


}

void the_PrintSectionNames(state* s){

    if(currentFd==-1){
        fprintf(stderr,"currentfd's value is -1\n");
        return;
    }
    Elf32_Shdr* sechead=map_start + header->e_shoff;//getting begin+offset og sechead to enter section headers
    printf("[index]\tsection_name\t\tsection_address\tsection_offset\tsection_size\tsection_type");
    if(s->debug_mode=='1'){
        printf("\tshstrndx\tsectioNameoffsets");
    }
    printf("\n");
    char* beginthe_table_sec=map_start + sechead[header->e_shstrndx].sh_offset;//offsetbegin+offsetof section header table index name
    for (int j = 0; j <header->e_shnum ; j=j+1) {// running on number of section tables
        char* Currsecnames=beginthe_table_sec+sechead[j].sh_name;
        printf("[%d]\t%-17s\t%08x\t%06x\t\t%06x\t\t",j,Currsecnames,sechead[j].sh_addr,sechead[j].sh_offset,sechead[j].sh_size);
        ///begin checking what wsection type
        if(sechead[j].sh_type==SHT_NULL){
            printf("NULL");
        }
        else if(sechead[j].sh_type==SHT_PROGBITS){
            printf("PROGBITS");
        }
        else if(sechead[j].sh_type==SHT_SYMTAB){
            printf("SYMTAB");
        }
        else if(sechead[j].sh_type==SHT_STRTAB){
            printf("STRTAB");
        }
        else if(sechead[j].sh_type==SHT_RELA){
            printf("RELA");
        }
        else if(sechead[j].sh_type==SHT_HASH){
            printf("HASH");
        }
        else if(sechead[j].sh_type==SHT_DYNAMIC){
            printf("DYNAMIC");
        }else if(sechead[j].sh_type==SHT_NOTE){
            printf("NOTE");
        }else if(sechead[j].sh_type==SHT_NOBITS){
            printf("NOBITS");
        }else if(sechead[j].sh_type==SHT_REL){
            printf("REL");
        }else if(sechead[j].sh_type==SHT_SHLIB){
            printf("SHLIB");
        }else if(sechead[j].sh_type==SHT_DYNSYM){
            printf("DYNSYM");
        }else if(sechead[j].sh_type==SHT_INIT_ARRAY){
            printf("INIT_ARRAY");
        }else if(sechead[j].sh_type==SHT_FINI_ARRAY){
            printf("FINI_ARRAY");
        }else if(sechead[j].sh_type==SHT_PREINIT_ARRAY){
            printf("PREINIT_ARRAY");
        }else if(sechead[j].sh_type==SHT_GROUP){
            printf("GROUP");
        }else if(sechead[j].sh_type==SHT_SYMTAB_SHNDX){
            printf("SYMTAB_SHNDX");
        }else if(sechead[j].sh_type==SHT_NUM){
            printf("NUM");
        }
        else if(sechead[j].sh_type==SHT_LOOS){
            printf("LOOS");
        }else if(sechead[j].sh_type==SHT_GNU_LIBLIST){
            printf("GNU_LIBLIST");
        }
        else if(sechead[j].sh_type==SHT_CHECKSUM){
            printf("CHECKSUM");
        }else if(sechead[j].sh_type==SHT_LOSUNW){
            printf("LOSUNW");
        }else if(sechead[j].sh_type==SHT_SUNW_move){
            printf("SUNW_move");
        }else if(sechead[j].sh_type==SHT_SUNW_COMDAT){
            printf("SUNW_COMDAT");
        }else if(sechead[j].sh_type==SHT_SUNW_syminfo){
            printf("SUNW_syminfo");
        }else if(sechead[j].sh_type==SHT_GNU_verdef){
            printf("GNU_verdef");
        }else if(sechead[j].sh_type==SHT_GNU_verneed){
            printf("GNU_verneed");
        }else if(sechead[j].sh_type==SHT_GNU_versym){
            printf("GNU_versym");
        }
        else if(sechead[j].sh_type==(int)1879048182){//GNU_HAH type number, did not find it in elf.h
            printf("GNU_HASH");
        }
        else if(sechead[j].sh_type==SHT_HISUNW){
            printf("HISUNW");
        }else if(sechead[j].sh_type==SHT_HIOS){
            printf("HIOS");
        }else if(sechead[j].sh_type==SHT_LOPROC){
            printf("LOPROC");
        }
        else if(sechead[j].sh_type==SHT_HIPROC){
            printf("HIPROC");
        }
        else if(sechead[j].sh_type==SHT_LOUSER){
            printf("LOUSER");
        }
        else if(sechead[j].sh_type==SHT_HIUSER){
            printf("HIUSER");
        }
        else{
            printf("no section type found");
        }

        //end of checking what section type
        if(s->debug_mode=='1'){
            char* k="";
            printf("\t\t%d\t\t\t%06x",header->e_shstrndx,sechead[header->e_shstrndx].sh_offset+sechead[j].sh_offset);
        }

        printf("\n");

    }


}

void the_Print_Symbols(state* s){

    if(currentFd==-1){
        fprintf(stderr,"currentfd's value is -1\n");
        return;
    }
    Elf32_Shdr* sechead=map_start + header->e_shoff;//getting begin+offset og sechead to enter section headers

    char* beginthe_table_sec=map_start + sechead[header->e_shstrndx].sh_offset;//offsetbegin+offsetof section header table index name
    for (int j = 0; j <header->e_shnum ; j=j+1) {
        if(sechead[j].sh_type==SHT_SYMTAB ||  sechead[j].sh_type==SHT_DYNSYM  ){//the symbol type we have
            Elf32_Sym* beginsymboltalb= map_start+sechead[j].sh_offset;
            int size=sechead[j].sh_size;
            if(s->debug_mode=='1') {
                printf("\tsize_of_symbol_table:%d\t\tnumber_of_sybmols_therein:%d\n",size,size/ sechead[j].sh_entsize);
            }
            printf("[index]\tvalue\t\tsection_index\tsection_name\tsymbol_name\n");
            char* beginonetable=map_start+sechead[sechead[j].sh_link].sh_offset;
            int t=0;
            while(t<size/ sechead[j].sh_entsize) {
                if (!specialind(beginsymboltalb[t].st_shndx,beginsymboltalb,t,beginonetable)){
                    //if 0 than not special

                    printf("[%d]\t%08x\t%d\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
                           beginsymboltalb[t].st_shndx,
                           beginthe_table_sec + sechead[beginsymboltalb[t].st_shndx].sh_name,
                           beginonetable + beginsymboltalb[t].st_name);

                }
                //if 1 than special and no need to print because printed allready
                t=t+1;
            }


        }
    }


}

int specialind(int index,Elf32_Sym* beginsymboltalb,int t,char* beginonetable){
    if(index==SHN_UNDEF){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "UND",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }
    else if(index==SHN_LORESERVE){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "LORESERVE",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }
    else if(index==SHN_LOPROC){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "LOPROC",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }
    else if(index==SHN_BEFORE){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "BEFORE",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }
    else if(index==SHN_HIPROC){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "HIPROC",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }
    else if(index==SHN_LOOS){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "LOOS",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }
    else if(index==SHN_HIOS){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "HIOS",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }
    else if(index==SHN_ABS){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "ABS",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }else if(index==SHN_XINDEX){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "XINDEX",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }else if(index==SHN_AFTER){
        printf("[%d]\t%08x\t%s\t\t%s\t\t%s\t\t\n", t, beginsymboltalb[t].st_value,
               "AFTER",
               "",
               beginonetable + beginsymboltalb[t].st_name);
        return 1;

    }

    else {
        return 0;
    }
}
