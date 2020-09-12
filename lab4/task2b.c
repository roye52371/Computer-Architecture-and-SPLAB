#include "util.h"


#define SYS_WRITE 4
#define STDOUT 1
#define STDIN 0
#define STDERR 2
#define dont_care 0 


#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WHT 14
  

typedef struct ent{
    int inode;
    int offset;
    short len;
    char buffer[1];
}ent;

extern int system_call();
void debugwhileLoop(char* str, ent* PointerToent);

int main (int argc , char* argv[], char* envp[])
{ 
    int isprefix=0;
    int indexofprefix=0;
    int isDebug=0;
	int pd=0;
    int i=1;
    for(i=1; i<argc; i=i+1){

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
            if(strncmp(argv[i],"-p",2)==0){
            isprefix=1;
            indexofprefix=i;
            int descriptorWrite=system_call(4,STDOUT,"-p\n",3);
            if(descriptorWrite==-1 || descriptorWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
        }
        

    }
     char* prefix;
    if(isprefix){
    prefix=argv[indexofprefix]+2;
}


char Maxbuf[8192];
    ent* PointerToent;
    int fd=system_call(5,".",0,0);
    if(isDebug){
        char* str=itoa(fd);
        
        int descriptorWrite=system_call(4,STDOUT,"5\t",2);
        if(descriptorWrite==-1 || descriptorWrite==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        descriptorWrite=system_call(4,STDOUT,str,strlen(str));
        if(descriptorWrite==-1 || descriptorWrite==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        descriptorWrite=system_call(4,STDOUT,"\n",1);
        if(descriptorWrite==-1 || descriptorWrite==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        
    }
    if(fd<0){
            system_call(1,0x55,dont_care,dont_care);
    }
    int count_size=system_call(141,fd,Maxbuf,8192);
    if(isDebug){
        char* str=itoa(count_size);
       int descriptorWrite=system_call(4,STDOUT,"141\t",4);
        if(descriptorWrite==-1 || descriptorWrite==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        descriptorWrite=system_call(4,STDOUT,str,strlen(str));
        if(descriptorWrite==-1 || descriptorWrite==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        descriptorWrite=system_call(4,STDOUT,"\n",1);
        if(descriptorWrite==-1 || descriptorWrite==0){
            system_call(1,0x55,dont_care,dont_care);
        }
        
    }
    if(count_size<0){
                system_call(1,0x55,dont_care,dont_care);
            }
    PointerToent=(struct ent*)Maxbuf;
    int j=0;
    while(j<count_size){
    if(isprefix){
        if(strncmp(prefix,PointerToent->buffer,strlen(prefix))==0){
            char type= *(Maxbuf+j+PointerToent->len-1);
            char* typename;
            if(type == DT_REG){typename="regular";}
            else if(type == DT_DIR){typename="directory";}                     
            else if(type == DT_FIFO){typename="FIFO";}                       
            else if(type == DT_SOCK) { typename="socket";}
            else if(type == DT_LNK){typename="symbolic link";}
            else if(type == DT_BLK){typename="block device"; }
            else if(type == DT_CHR){typename="char device";} 
            else if(type == DT_WHT){typename="dt wht-without inode";}
            else if(type == DT_UNKNOWN){typename="unknown type";}
            int descriptorWrite=system_call(4,STDOUT,PointerToent->buffer,strlen(PointerToent->buffer));
            if(descriptorWrite==-1 || descriptorWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            int descriptortonewLine=system_call(4,STDOUT,"\t",1);
            if(descriptortonewLine==-1 || descriptortonewLine==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptortonewLine=system_call(4,STDOUT,"typeName:",9);
            if(descriptortonewLine==-1 || descriptortonewLine==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptortonewLine=system_call(4,STDOUT,typename,strlen(typename));
            if(descriptortonewLine==-1 || descriptortonewLine==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptortonewLine=system_call(4,STDOUT,"\n",1);
            if(descriptortonewLine==-1 || descriptortonewLine==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            char* str=itoa(descriptorWrite);
        if(isDebug){
            debugwhileLoop(str,PointerToent);
        }

        }
    }
    else{
    int descriptorWrite=system_call(4,STDOUT,PointerToent->buffer,strlen(PointerToent->buffer));
     if(descriptorWrite==-1 || descriptorWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            int descriptortonewLine=system_call(4,STDOUT,"\n",1);
            if(descriptortonewLine==-1 || descriptortonewLine==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            char* str=itoa(descriptorWrite);
       if(isDebug){
           debugwhileLoop(str,PointerToent);
        }
            
       }
       
        j=j+PointerToent->len;
        PointerToent=(struct ent*)(Maxbuf+j);
    
}
    
    int descriptorClose=system_call(6,fd);
         if(isDebug){
            char* str=itoa(descriptorClose);
            
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
        if(descriptorClose==-1){
                system_call(1,0x55,dont_care,dont_care);
            }


    return 0;
}

void debugwhileLoop(char* str, ent* PointerToent){
     int descriptorERRWrite=system_call(4,STDERR,"\n4\t",3);
            if(descriptorERRWrite<0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptorERRWrite=system_call(4,STDERR,str,strlen(str));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptorERRWrite=system_call(4,STDERR,"\nfile name:",11);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            
            descriptorERRWrite=system_call(4,STDERR,PointerToent->buffer,strlen(PointerToent->buffer));
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptorERRWrite=system_call(4,STDERR,"\tlength:",8);
            if(descriptorERRWrite<0){
                system_call(1,0x55,dont_care,dont_care);
            }
            char *str2=itoa(PointerToent->len);
            descriptorERRWrite=system_call(4,STDERR,str2,strlen(str2));
            if(descriptorERRWrite<0){
                system_call(1,0x55,dont_care,dont_care);
            }
            descriptorERRWrite=system_call(4,STDERR,"\n\n",2);
            if(descriptorERRWrite==-1 || descriptorERRWrite==0){
                system_call(1,0x55,dont_care,dont_care);
            }
    
}

