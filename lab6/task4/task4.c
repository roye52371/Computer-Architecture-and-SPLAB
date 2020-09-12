
#include <stdio.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
typedef struct string_pair{
    char* name;
    char* value;
    struct string_pair *next;
}string_pair;
void execute(cmdLine* line);
int secondSon(cmdLine* line,int isVARexist,int ProgramId,int isPipe,int pipefd[],int isDebug);
int replacingVariables(cmdLine* line,int isVARexist,int j);
void addToPairList(string_pair** list, char* name ,char* value);
int checkAndUpdate(string_pair* list, char* name ,char* value,int isUpdate);
void printthePairList(string_pair* theprocess_list, int indexList);
string_pair* findPairwithName(string_pair* list,char* name);
void freePairList(string_pair* process_list);
string_pair** pointerToLink=NULL;
int main(int argc, char *argv[]) {
    int isDebug=0;// flag for -D mode FILE *fp
    for(int i=1; i<argc; i++){

        if(strcmp(argv[i],"-D")==0){
            isDebug=1;
            printf("-D\n");
        }

    }

    pointerToLink=calloc(1, sizeof(string_pair*));
    int ProgramId=5;// randon initiallization
    int ProgramId2=5;
    if(isDebug){
        fprintf(stderr,"ProgramID of Father-%d\n\n",getpid());
    }
    int pipefd[2];


    while(1) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(1);
        }
        int isVARexist=1;
        int isVARexist2=1;
        //int isExecutable=1;
        int isPipe=0;
        char dirctorypath[PATH_MAX];
        getcwd(dirctorypath, PATH_MAX);
        printf("%s\n", dirctorypath);
        char oneLine[2048];
        fgets(oneLine, 2048, stdin);
        if(isDebug){
            fprintf(stderr,"\nExecuting command-%s\n",oneLine);
        }

        if (memcmp(oneLine, "quit", 4) == 0) {
            freePairList(*pointerToLink);
            free(pointerToLink);
            exit(0);
        }

        cmdLine *line = parseCmdLines(oneLine);

        if(line->next!=NULL){
            isPipe=1;
        }
        int j=0;
         if (memcmp(line->arguments[0], "set", 3) != 0){
            isVARexist=replacingVariables(line,isVARexist,j);
         }
        if(isPipe){
             if (memcmp(line->arguments[0], "set", 3) != 0){
                isVARexist2=replacingVariables(line->next,isVARexist2,j);
             }
        }
        if (memcmp(line->arguments[0], "set", 3) == 0){
            oneLine[strlen(oneLine)-1]='\0';// dont want \n in value
            addToPairList(pointerToLink,line->arguments[1],oneLine+5+strlen(line->arguments[1]));
            // isExecutable=0;

        }
        else if (memcmp(line->arguments[0], "vars", 4) == 0){
            printthePairList(*pointerToLink,1);
            // isExecutable=0;

        }

        else if (memcmp(line->arguments[0], "cd", 2) == 0){
            //isExecutable=0;
            char* directoryAsked= line->arguments[1];
            if(memcmp(line->arguments[1],"~",1)==0 && line->argCount==2){
                directoryAsked=getenv("HOME");
                if(directoryAsked==NULL){
                    fprintf(stderr,"cd faild, in case using getenv to convert %s\n",line->arguments[1]);
                }
            }

            int isError = chdir(directoryAsked);
            if (isError == -1) {
                fprintf(stderr, "cd faild: %s, no such file or directory\n", line->arguments[1]);
            }

        }


        else{
            if(isVARexist) {
                ProgramId = fork();
                if (ProgramId == 0) {
                    if(isPipe){
                        close(STDOUT_FILENO);
                        dup(pipefd[1]);
                        close(pipefd[1]);
                    }

                    if (isDebug) {
                        fprintf(stderr, "ProgramIDChild at child-%d\n\n", ProgramId);
                    }
                    if (line->inputRedirect != NULL) {
                        fclose(stdin);
                        FILE *read;
                        read = fopen(line->inputRedirect, "r");
                        if (read == NULL) {
                            fprintf(stderr, "myshell messsage error: no such file %s\n", line->inputRedirect);
                        }
                    }
                    if (line->outputRedirect != NULL) {
                        fclose(stdout);
                        FILE *write;
                        write = fopen(line->outputRedirect, "a");
                        if (write == NULL) {
                            fprintf(stderr, "myshell messsage error: error accure when trying to write to %s\n",
                                    line->outputRedirect);
                        }
                    }
                    execute(line);
                    _exit(0);//if execvp in executed succeed then we will not return to this line in the code, so if failed we will get here anyway
                }
                if (isDebug) {
                    fprintf(stderr, "ProgramIDChild at father-%d\n\n", ProgramId);
                }


            }
        }

        if(isPipe){
            close(pipefd[1]);
        }

        if(isPipe){
            ProgramId2=secondSon(line->next,isVARexist2,ProgramId2,isPipe,pipefd,isDebug);
        }

        if(isPipe){
            close(pipefd[0]);
        }




        if (line->blocking != 0) {
            int the_status = 0;
            waitpid(ProgramId, &the_status, 0);// equivalent to wait(&ProgramId)
        }
        if(isPipe){
            if (line->next->blocking != 0) {
                int the_status2 = 0;
                waitpid(ProgramId2, &the_status2, 0);// equivalent to wait(&ProgramId)
            }
        }


        freeCmdLines(line);
    }
    return 0;
}


void execute(cmdLine* line){
    int retVal= execvp(line->arguments[0],line->arguments);
    if(retVal==-1){
        freeCmdLines(line);
        perror("execute failed");
        exit(1);
    }

}




void addToPairList(string_pair** list, char* name ,char* value){
    int isUpdate=checkAndUpdate(*list,name,value,0);


    if(isUpdate==0) {
        string_pair* link= calloc(1, sizeof(string_pair));
        link->name=calloc(1, strlen(name)+1);
        link->value=calloc(1, strlen(value)+1);
        strcpy(link->name,name);
        strcpy(link->value,value);
        link->next =*list;
        *list=link;
    }

}

int checkAndUpdate(string_pair* list, char* name ,char* value, int isUpdate){
    if(list==NULL){
        return isUpdate;
    }
    else{
        if( strcmp(list->name,name)==0) {
            //char* temp=list->value;
            free(list->value);
            list->value=calloc(1, strlen(value)+1);
            strcpy(list->value,value);
            isUpdate = 1;
            return isUpdate;
        }

        return checkAndUpdate(list->next,name,value,isUpdate);

    }
}

void printthePairList(string_pair* theprocess_list, int indexList){
    if(theprocess_list!=NULL) {
        printf("%d. name: %s\tvalue: %s\n", indexList, theprocess_list->name,theprocess_list->value);

        printthePairList(theprocess_list->next,indexList);
    }

}

void freePairList(string_pair* process_list){
    if(process_list!=NULL){
        freePairList(process_list->next);
        free(process_list->name);
        free(process_list->value);
        free(process_list);
    }

}

string_pair* findPairwithName(string_pair* list,char* name){
    if(list==NULL){
        return list;
    }
    else{
        if(strcmp(list->name,name)==0){
            return list;
        }
        return findPairwithName(list->next,name);

    }
}

int replacingVariables(cmdLine* line,int isVARexist,int j){
    while(line->arguments[j]!=NULL && isVARexist){
        if(memcmp(line->arguments[j],"$",1)==0){
            string_pair* link=findPairwithName(*pointerToLink,line->arguments[j]+1);//taking the var name
            if(link==NULL){
                fprintf(stderr,"activating a variable that does not exist\n");
                isVARexist=0;
            }
            else{
                char* copyval=calloc(1, strlen(link->value)+1);
                strcpy(copyval,link->value);
                int sizeToken=0;
                for (int k = 0; k <strlen(link->value) ; k=k+1) {
                    if(link->value[k]==' '){
                        sizeToken=sizeToken+1;
                    }
                }
                if(sizeToken>0){
                    sizeToken=sizeToken+1;// there is one more words than spaces
                }
                char* tok;
                tok=strtok(copyval," ");

                int oldArgCount=line->argCount;
                if(sizeToken>0) {
                    line->argCount = line->argCount + sizeToken - 1;
                }
                for (int i = oldArgCount-1; i >j ; i=i-1) {
                    // line->arguments[i+sizeToken]=line->arguments[i];
                    int p=i+sizeToken-1;
                    replaceCmdArg(line,p,line->arguments[i]);
                }
                int t=j;
                while(tok!=NULL) {
                    replaceCmdArg(line, t, tok);
                    tok=strtok(NULL," ");
                    t = t + 1;
                }
                free(copyval);
            }

        }

        j=j+1;
    }
    return isVARexist;
}


int secondSon(cmdLine* line,int isVARexist,int ProgramId,int isPipe,int pipefd[],int isDebug){
    if(isVARexist) {
        ProgramId = fork();
        if (ProgramId == 0) {
            if(isPipe){
                close(STDIN_FILENO);
                dup(pipefd[0]);
                close(pipefd[0]);
            }

            if (isDebug) {
                fprintf(stderr, "ProgramIDChild at child-%d\n\n", ProgramId);
            }
            if (line->inputRedirect != NULL) {
                fclose(stdin);
                FILE *read;
                read = fopen(line->inputRedirect, "r");
                if (read == NULL) {
                    fprintf(stderr, "myshell messsage error: no such file %s\n", line->inputRedirect);
                }
            }
            if (line->outputRedirect != NULL) {
                fclose(stdout);
                FILE *write;
                write = fopen(line->outputRedirect, "a");
                if (write == NULL) {
                    fprintf(stderr, "myshell messsage error: error accure when trying to write to %s\n",
                            line->outputRedirect);
                }
            }
            execute(line);
            _exit(0);//if execvp in executed succeed then we will not return to this line in the code, so if failed we will get here anyway
        }
        if (isDebug) {
            fprintf(stderr, "ProgramIDChild at father-%d\n\n", ProgramId);
        }


    }
    return ProgramId;
}
