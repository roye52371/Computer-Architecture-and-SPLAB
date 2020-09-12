#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1
#define STDIN 0
#define STDERR 2
#define dont_care 0 
extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
     int isDebug=0;
    int isOfile=0;
    int isMinusifile=0;
    int indexoffile=0;
    int indexofMinusi=0;
	int pd=0;
    int i=1;
    for(i=1; i<argc; i=i+1){
        if(strncmp(argv[i],"-o",2)==0){
            isOfile=1;
            indexoffile=i;
            int descriptorWrite=system_call(4,STDOUT,"-o\n",3);
             if(descriptorWrite==-1 || descriptorWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
	
        }
	if(strncmp(argv[i],"-i",2)==0){
            isMinusifile=1;
            indexofMinusi=i;
            int descriptorWrite=system_call(4,STDOUT,"-i\n",3);
            if(descriptorWrite==-1 || descriptorWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        }

        if(strcmp(argv[i],"-D")==0){
            isDebug=1;
            if(pd==0){
                int descriptorWrite=system_call(4,STDOUT,"-D\n",3);
                pd=1;
                if(descriptorWrite==-1 || descriptorWrite==0){
                    system_call(1,0x55,dont_care,dont_care);
                }
            }
	
        }
        

    }

    
    int descriptorFp=STDOUT;
    int descriptorFi=STDIN;

    char* filename="stdout";
    char* filenametoReadFrom="stdin";
    

    if(isOfile){
        filename=argv[indexoffile]+2;
        descriptorFp=system_call(5,filename,64 | 1,0777);
        char* str=itoa(descriptorFp);
         if(isDebug){
            int descriptorERRWrite=system_call(4,STDERR,"5\t",2);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
            descriptorERRWrite=system_call(4,STDERR,str,strlen(str));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
                }
            descriptorERRWrite=system_call(4,STDERR,"\n",1);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
        }
        if(descriptorFp<0){
            system_call(4,STDOUT,"error while tring to create file to write to\n",45);
            system_call(1,0x55,dont_care,dont_care);
        }
    
    }
    if(isMinusifile)
    {   filenametoReadFrom=argv[indexofMinusi]+2;
        descriptorFi=system_call(5,filenametoReadFrom,0,0777);
        char* str=itoa(descriptorFi);
         if(isDebug){
            int descriptorERRWrite=system_call(4,STDERR,"5\t",2);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
            descriptorERRWrite=system_call(4,STDERR,str,strlen(str));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
                }
            descriptorERRWrite=system_call(4,STDERR,"\n",1);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
        }
        if(descriptorFi<0){
        system_call(4,STDOUT,"error while tring to open file to read from\n",44);
        system_call(1,0x55,dont_care,dont_care);
        }

    }
    if(isDebug){
    int descriptorWrite=system_call(4,STDOUT,"we read from ",13);
    descriptorWrite=system_call(4,STDOUT,filenametoReadFrom,strlen(filenametoReadFrom));
    descriptorWrite=system_call(4,STDOUT,"\n",1);
    if(descriptorWrite==-1 || descriptorWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
    descriptorWrite=system_call(4,STDOUT,"we write to ",12);
    descriptorWrite=system_call(4,STDOUT,filename,strlen(filename));
    descriptorWrite=system_call(4,STDOUT,"\n",1);
    if(descriptorWrite==-1 || descriptorWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
    }
    char intvalofChar;
    while(1){
        int descriptorRead=system_call(3,descriptorFi,&intvalofChar,1);
        char* str=itoa(descriptorRead);
         if(isDebug){
            int descriptorERRWrite=system_call(4,STDERR,"3\t",2);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
            descriptorERRWrite=system_call(4,STDERR,str,strlen(str));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
                }
            descriptorERRWrite=system_call(4,STDERR,"\n",1);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
        }
        
        if(descriptorRead==-1 || descriptorRead==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        
       
        if(intvalofChar>=97 && intvalofChar<=122){
            intvalofChar=intvalofChar-32;
            
        }
    
    int descriptorWrite=system_call(4,descriptorFp,&intvalofChar,1);
       str=itoa(descriptorWrite);
       if(isDebug){
            int descriptorERRWrite=system_call(4,STDERR,"\n4\t",3);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptorERRWrite=system_call(4,STDERR,str,strlen(str));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptorERRWrite=system_call(4,STDERR,"\n\n",2);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
       }
       
        if(descriptorWrite==-1 || descriptorWrite==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        
    
    }
    
    if(isOfile){
        int descriptorClose=system_call(6,descriptorFp);
        if(descriptorClose==-1){
                system_call(1,0x55,dont_care,dont_care);
            }
        char* str=itoa(descriptorClose);
         if(isDebug){
            int descriptorERRWrite=system_call(4,STDERR,"6\t",2);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
            descriptorERRWrite=system_call(4,STDERR,str,strlen(str));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
                }
            descriptorERRWrite=system_call(4,STDERR,"\n",1);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
        }
    
    }
    
    if(isMinusifile)
    {   
        int descriptorClose=system_call(6,descriptorFi);
        if(descriptorClose==-1){
                system_call(1,0x55,dont_care,dont_care);
            }
        char* str=itoa(descriptorClose);
         if(isDebug){
            int descriptorERRWrite=system_call(4,STDERR,"6\t",2);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
            descriptorERRWrite=system_call(4,STDERR,str,strlen(str));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
                }
            descriptorERRWrite=system_call(4,STDERR,"\n",1);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        
        }

    }
    
    return 0;
}
