#include <stdio.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
void execute(cmdLine* line);
int main(int argc, char *argv[]) {
    int isDebug=0;// flag for -D mode FILE *fp
    for(int i=1; i<argc; i++){
        
        if(strcmp(argv[i],"-D")==0){
            isDebug=1;
            printf("-D\n");
		}
        
    }
    

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
        
        if (memcmp(oneLine, "quit", 4) == 0)
            exit(0);
        
        cmdLine *line = parseCmdLines(oneLine);
        if (memcmp(oneLine, "cd ", 3) == 0){
            int isError=chdir(line->arguments[1]);
            if(isError==-1){
                fprintf(stderr,"cd faild: %s, no such file or directory\n",line->arguments[1]);
            }
        }
        else{
            ProgramId=fork();
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
            
            int the_status=0;
            if(line->blocking!=0){
                waitpid(ProgramId,&the_status,0);// equivalent to wait(&ProgramId)
                //printf("waiting-block-%d\n",line->blocking);
                //sleep(2);
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
