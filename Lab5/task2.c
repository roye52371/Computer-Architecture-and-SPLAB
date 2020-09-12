#include <stdio.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0
typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;
process** processLIST=NULL;
void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
//void addtheProcess(process* theprocess_list, process* newProcess);
void printProcessList(process** process_list);
void printtheProcessList(process* theprocess_list,int indexList);
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updatetheProcessList(process *theprocess_list);
void updateProcessStatus(process* process_list, int pid, int status);
void printProcessList(process** process_list);
void cleanFreshTerminated(process** process_list);
void checkAndupdateStatus(int the_status, process* theprocess_list, int pid);
void execute(cmdLine* line);

int main(int argc, char *argv[]) {
    int isDebug=0;// flag for -D mode FILE *fp
    for(int i=1; i<argc; i++){

        if(strcmp(argv[i],"-D")==0){
            isDebug=1;
            printf("-D\n");
        }

    }
    //processLIST=NULL;
    processLIST=calloc(1,sizeof(process*));
    int ProgramId=5;// randon initiallization
    if(isDebug){
                fprintf(stderr,"ProgramID of Father-%d\n\n",getpid());
            }
            
    while(1) {
        char dirctorypath[PATH_MAX];
        getcwd(dirctorypath, PATH_MAX);
        printf("%s\n", dirctorypath);
        char oneLine[2048];
        fgets(oneLine, 2048, stdin);
        if(isDebug){
            fprintf(stderr,"\nExecuting command-%s\n",oneLine);
        }



        cmdLine *line = parseCmdLines(oneLine);

        if (memcmp(oneLine, "quit", 4) == 0) {
            freeProcessList(*processLIST);
            freeCmdLines(line);
            free(processLIST);
            exit(0);
        }
        if (memcmp(oneLine, "proc", 4) == 0) {
            printf("index:\tPID:\tStatus:\tCMDLine+arguements:\n");
            printProcessList(processLIST);
            freeCmdLines(line);
        }
        else if (memcmp(oneLine, "suspend ", 8) == 0){
            int pid=atoi(line->arguments[1]);
            kill(pid,SIGTSTP);
            //updateProcessStatus(*processLIST,pid,SUSPENDED);
            freeCmdLines(line);
        }
        else if (memcmp(oneLine, "kill ", 5) == 0){
            int pid=atoi(line->arguments[1]);
            kill(pid,SIGINT);
            //updateProcessStatus(*processLIST,pid,TERMINATED);
            freeCmdLines(line);
        }
        else if (memcmp(oneLine, "wake ", 5) == 0){
            int pid=atoi(line->arguments[1]);
            kill(pid,SIGCONT);
            //updateProcessStatus(*processLIST,pid,RUNNING);
            freeCmdLines(line);
        }
        else if (memcmp(oneLine, "cd ", 3) == 0){
            int isError=chdir(line->arguments[1]);
            if(isError==-1){
                fprintf(stderr,"cd faild: %s, no such file or directory\n",line->arguments[1]);
            }
            freeCmdLines(line);
        }
        else{
            ProgramId=fork();
            addProcess(processLIST,line,ProgramId);
            if(ProgramId==0 ){
                if(isDebug){
                    fprintf(stderr,"ProgramIDChild at child-%d\n\n",ProgramId);
                }
                execute(line);
                _exit(0);//if execvp in executed succeed then we will not return to this line in the code, so if failed we will get here anyway
            }
            if(isDebug){
                fprintf(stderr,"ProgramIDChild at father-%d\n\n",ProgramId);
            }

            if(line->blocking!=0){
                // printf("%d",ProgramId);
                int the_status=0;
                waitpid(ProgramId,&the_status,0);// waiting and using status
                checkAndupdateStatus(the_status,*processLIST,ProgramId);
                //printf("waiting-block-%d\n",line->blocking);
                //sleep(2);
            }
        }
        //freeCmdLines(line);
    }
    return 0;
}


void execute(cmdLine* line){
    int retVal= execvp(line->arguments[0],line->arguments);
    if(retVal==-1){
        freeProcessList(*processLIST);
        free(processLIST);
        perror("execute failed");
        exit(1);
    }

}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process* process1=calloc(1,sizeof(process));
    process1->next=NULL;
    process1->cmd=cmd;
    process1->pid=pid;
    process1->status=RUNNING;// not sure abput it
    //process_list=&process1;
    process1->next=*process_list;
    *process_list=process1;
   /*
    if(*process_list==NULL)//if pointing to nothing
        *process_list=process1;//then nnow point at a process

    else{
        addtheProcess(*process_list,process1);
    }
    */
}
/*
void addtheProcess(process* theprocess_list, process* newProcess){
    if(theprocess_list->next==NULL){
        theprocess_list->next=newProcess;
    }
    else{
        addtheProcess(theprocess_list->next,newProcess);
    }
}
*/
void printProcessList(process** process_list){
    updateProcessList(process_list);
    if(process_list!=NULL) {//if pointing to nothing
        printtheProcessList(*process_list, 1);
        cleanFreshTerminated(process_list);//afterprinting clean freesh terminated processes
    }
}

void printtheProcessList(process* theprocess_list, int indexList){
    if(theprocess_list!=NULL) {
        char* status=NULL;
        if(theprocess_list->status==-1){
            status="TERMINATED";
        }
        else if(theprocess_list->status==0){
            status="SUSPENDED";
        } else{
            status="RUNNING";
        }
        printf("%d\t%d\t%s\t", indexList, theprocess_list->pid, status);
        int j=0;
        while(theprocess_list->cmd->arguments[j]!=NULL){
            printf("%s",theprocess_list->cmd->arguments[j]);
            if(theprocess_list->cmd->arguments[j+1]!=NULL){
                printf(" ");
            }
            j=j+1;
        }
        /* for (int i = 0; i <strlen((theprocess_list->cmd->arguments)) && theprocess_list->cmd->arguments[i]!=NULL ; i=i+1) {
             printf("%s",theprocess_list->cmd->arguments[i]);
         }*/
        printf("\n");
        indexList=indexList+1;
        printtheProcessList(theprocess_list->next,indexList);
    }

}

void freeProcessList(process* process_list){
    if(process_list!=NULL){
        freeProcessList(process_list->next);
        freeCmdLines(process_list->cmd);
        free(process_list);
    }

}



void updateProcessStatus(process* process_list, int pid, int status){
    if(process_list!=NULL){
        if(process_list->pid==pid){
            process_list->status=status;
        }
        updateProcessStatus(process_list->next,pid,status);
    }
    // printf("\n%d\n",process_list->status);
}

void cleanFreshTerminated(process** process_list){

    process* the_Head=*process_list;
    process* current=*process_list;
    //process* next=current->next;
    process* prev=the_Head;
    while(current!=NULL){
        if(current->status==TERMINATED){
            //deep check if current is the head
            if(current==the_Head){// if current is NULL then i am the head
                process* temp=current->next;
                *process_list=temp;//make the pointer to point the new head
                prev=temp;

                freeCmdLines(current->cmd);
                free(current);
                the_Head=temp;
                current=temp;
            }
            else {
                process* temp=current->next;
                prev->next = temp;
                freeCmdLines(current->cmd);
                free(current);
                current=temp;

            }
        }
        else {
            //if(current!=the_Head) {//CHECK IF NEED TO CHANGE TO "current!=the_Head"
            prev = current;
            //}
            current = current->next;
        }
    }
}

void updateProcessList(process **process_list){
    if(process_list!=NULL){
        updatetheProcessList(*process_list);
    }
}

void updatetheProcessList(process *theprocess_list){
    int PID=0;
    int the_status;
    if(theprocess_list!=NULL){
        if((PID=waitpid(theprocess_list->pid,&the_status,WNOHANG | WCONTINUED  | WUNTRACED ))>0){// trying without checking !(theprocess_list->cmd->blocking)&&
           checkAndupdateStatus(the_status,theprocess_list,theprocess_list->pid);
        }
        updatetheProcessList(theprocess_list->next);
    }

}


void checkAndupdateStatus(int the_status, process* theprocess_list,int pid){
    if(WIFSIGNALED(the_status)|| WIFEXITED(the_status))
        updateProcessStatus(theprocess_list,pid,TERMINATED);
    else if(WIFSTOPPED(the_status))
        updateProcessStatus(theprocess_list,pid,SUSPENDED);
    else if(WIFCONTINUED(the_status))
        updateProcessStatus(theprocess_list,pid,RUNNING);
}
