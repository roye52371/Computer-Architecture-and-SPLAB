
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int isDebug=0;// flag for -D mode FILE *fp
    for(int i=1; i<argc; i++){

        if(strcmp(argv[i],"-d")==0){
            isDebug=1;
            printf("-d\n");
        }

    }
    int pipefd[2];
   
    // char * const arguments[3];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    if(isDebug){
        fprintf(stderr,"parent_process forking\n");
    }
    int ProgramId1=fork();
    if(isDebug){
        fprintf(stderr,"parent_process>created process with id:%d\n",ProgramId1);
    }
    if(ProgramId1==0) {
        if(isDebug){
            fprintf(stderr,"child1>redirecting stdout to the write end of the pipe\n");
        }
        close(STDOUT_FILENO);
         dup(pipefd[1]);
        close(pipefd[1]);
        // char** child1;
        char* the_child1[]={"ls","-l",NULL};
        /*
        child1[0] = "ls";
        child1[1] = "-l";
        child1[2] = NULL;
         */
        if(isDebug){
            fprintf(stderr,"going to execute cmd: ls -l\n");
        }
        int retVal = execvp(the_child1[0], the_child1);
        if (retVal == -1) {
            perror("execute failed");
            exit(1);
            // exit(0);
        }
    }
    
    if(isDebug){
        fprintf(stderr,"parent_process>closing the write end of the pipe\n");
    }
     close(pipefd[1]);
     
    int ProgramId2 = fork();
    if(isDebug){
        fprintf(stderr,"parent_process>created process with id:%d\n",ProgramId2);
    }
    if (ProgramId2 == 0) {
        if(isDebug){
            fprintf(stderr,"child2>redirecting stdin to the read end of the pipe\n");
        }
        close(STDIN_FILENO);
        dup(pipefd[0]);
        close(pipefd[0]);
        //char** child2;
        char* the_child2[]={"tail","-n","2",NULL};
        /*
        child2[0] = "tail";
        child2[1] = "-n";
        child2[2] = "2";
        child2[3] = NULL;
         */
        if(isDebug){
            fprintf(stderr,"child2>going to execute cmd:tail -n 2\n");
        }
        int retVal = execvp(the_child2[0], the_child2);
        if (retVal == -1) {
            perror("execute failed");
            exit(1);
            // exit(0);
        }
    }
    
    if(isDebug){
        fprintf(stderr,"parent_process>closing the read end of the pipe\n");
    }
    close(pipefd[0]);
    
    int status1 = 0;
    int status2 = 0;
    
    if(isDebug){
        fprintf(stderr,"parent_process>waiting for child processes to terminate\n");
    }
    waitpid(ProgramId1, &status1, 0);
    waitpid(ProgramId2, &status2, 0);
    
    if(isDebug){
        fprintf(stderr,"parent_process>exiting\n");
    }


    return 0;
}


//#include "LineParser.h"
