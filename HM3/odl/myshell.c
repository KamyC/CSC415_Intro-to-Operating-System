/****************************************************************
 * Name        : Jinghan Cao                                    *
 * Class       : CSC 415                                        *
 * Date        :  2018-10-17                                    *
 * Description :  Writting a simple bash shell program          *
 *                that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFERSIZE 256
#define PROMPT "Jinghan-C >> "
#define PROMPTSIZE sizeof(PROMPT)

void loadPipe(char* input,char **myargv,int myargc,char **argv_1,char **argv_2){
    char *p;
    p=strtok(input," \t\n");
    while(p!=NULL){
        myargv[myargc++]=p;
        // printf("%s\n",p);
        p=strtok(NULL," \t\n");
    }
    myargv[myargc]==NULL;
    
    int i=0;
    int index=0;// index of | 
    while(myargv[i]!=NULL){
        // printf("%s ",myargv[i]);
        if(strcmp(myargv[i],"|")==0){
            index=i;
        }
        i++;
    }

    int len=myargc;
    for(int n=0;n!=index;n++){
        argv_1[n]=myargv[n];//store arguments before | in argv_1
        // printf("%s\n",argv_1[n]);
    }
    // argv_1[n]=NULL;
    int k=0;
    for(int m=index+1;m!=len;m++){
        argv_2[k]=myargv[m];//store arguments after | in argv_2
        k++;
    }
    // argv_2[k]=NULL;
    myargc=0;
}

void loadDirectIn(char* input,char **myargv,int myargc){
    char *p;
    p=strtok(input," \t\n");
    while(p!=NULL){
        if(strcmp(p,"<")!=0){
            myargv[myargc++]=p;
        }
        p=strtok(NULL," \t\n");
    }
    myargv[myargc]==NULL;
    myargc=0;//reset the input index
}

void loadDirectOut(char* input,char **myargv,int myargc){
    char *p;
    p=strtok(input," \t\n");
    while(p!=NULL){
        myargv[myargc++]=p;
        p=strtok(NULL," \t\n");
    }
    myargv[myargc-2]=NULL;
    myargc=0;//reset the input index
}

void loadBak(char* input,char **myargv,int myargc){
    char *p;
    p=strtok(input," \t\n");
    while(p!=NULL){
        myargv[myargc++]=p;
        p=strtok(NULL," \t\n");
    }
    myargv[myargc-1]=NULL;
    myargc=0;//reset the input index
}

void loadCmd(char* input,char **myargv,int myargc){
    char *p;
    p=strtok(input," \t\n");
    while(p!=NULL){
        myargv[myargc++]=p;
        p=strtok(NULL," \t\n");
    }
    myargv[myargc]=NULL;
    myargc=0;//reset the input index
}
void buildIn(char* input){
    if(strstr(input,"cd ")){
        char *p;
        p=strtok(input," \t\n");
        int c=0;
        char* argv[BUFFERSIZE];
        while(p!=NULL){
            argv[c++]=p;
            p=strtok(NULL," \t\n");
        }
        chdir(argv[1]);
    }
    //get the current working directory
    if(strcmp(input,"pwd")==0){
        char *buf;
        char *cwd;
        if((buf=(char*)malloc(BUFFERSIZE))!=NULL){
            cwd=getcwd(buf,BUFFERSIZE);
        }
        printf("%s\n",buf);
    }

    //if input "exit", quit the shell
    if(strcmp(input,"exit\n")==0){
        exit(0);
    }
}

int main(int argc, char* argv[]){   
    char *myargv[BUFFERSIZE];//input segment stores here
    int myargc=0;//my argument count
    char input[BUFFERSIZE];

    char *buf;
    char *cwd;

    int fd[2];

    int status = 1;
    while(status){
        //read user input into the input[BUFFERSIZE]
        do{ 
            printf(PROMPT);
            if(fgets(input,BUFFERSIZE,stdin)==NULL){
                printf("\n");
                exit(0);
            };
            
        }while(strcmp(input,"\n")==0);
       
        //change the working directory
        if(strstr(input,"cd ")){
            char *p;
            p=strtok(input," \t\n");
            while(p!=NULL){
                myargv[myargc++]=p;
                p=strtok(NULL," \t\n");
            }
            chdir(myargv[1]);
            myargc=0;//reset the input index
        }
        //get the current working directory
        if(strcmp(input,"pwd")==0){
            if((buf=(char*)malloc(BUFFERSIZE))!=NULL){
                cwd=getcwd(buf,BUFFERSIZE);
            }
            printf("%s\n",buf);
        }

        //if input "exit", quit the shell
        if(strcmp(input,"exit\n")==0){
            status=0;
            exit(1);
        }

        // if(buildIn(input)==0){}

        //run in background
        if(strstr(input,"&")){
            loadBak(input,myargv,myargc);
            int pid_1=fork();
            if(pid_1<0){
                printf("fork error");
                return 0;
            }
            else if(pid_1==0){
                execvp(myargv[0],myargv);
                exit(1);
            }
            else {
                printf("[1] %d\n",pid_1);//[1] refers to the parent process
            }
        }
        //redirect to a file
        else if(strstr(input," > ")){ //pid_2
            char* pos=strstr(input," > ");
            char* filename=pos+3;
            loadDirectOut(input,myargv,myargc);
            int pid_2=fork();
            if(pid_2<0){
                printf("fork error");
                return 0;
            }
            else if(pid_2==0){
                int fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd<0){
                    perror("file open error");
                }
                dup2(fd,1);
                close(fd);
                execvp(myargv[0],myargv);
                exit(1);
            }else{
                wait(NULL);
            }
        }
        //redirect to a file
        else if(strstr(input," >> ")){ //pid_3
            char* pos=strstr(input," >> ");
            char* filename=pos+4;

            loadDirectOut(input,myargv,myargc);
            int pid_3=fork();
            if(pid_3<0){
                printf("fork error");
                return 0;
            }
            else if(pid_3==0){
                int fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd<0){
                    perror("file open error");
                }
                dup2(fd,1);
                close(fd);
                execvp(myargv[0],myargv);
                // exit(1);
            }else{
                wait(NULL);
            } 
        }
        //redire the input from file
        else if(strstr(input,"<")){//pid_4
            char* pos=strstr(input," < ");
            char* filename=pos+3;
            
            loadDirectIn(input,myargv,myargc);
            int pid_3=fork();
            if(pid_3<0){
                printf("fork error");
                return 0;
            }
            else if(pid_3==0){
                int fd_2=open(filename,O_RDONLY,0644);
                if(fd_2<0){
                    perror("file open error");
                }
                dup2(fd_2,fileno(stdin));
                close(fd_2);
                execvp(myargv[0],myargv);
                // exit(1);
            }else{
                wait(NULL);
            } 
        }
        //pipe
        else if(strstr(input,"|")){
            char* argv1[BUFFERSIZE];
            char* argv2[BUFFERSIZE];
            loadPipe(input,myargv,myargc,argv1,argv2);

            pipe(fd);

            pid_t pid_5=fork();//pid_5
            if(pid_5<0){
                perror("pipe fork error");
            }
            else if(pid_5==0){//child process
                dup2(fd[1],fileno(stdout));
                execvp(argv1[0],argv1);
                close(fd[1]);
            }
            else{
                pid_t pid_A=fork();
                if(pid_A==0){
                    dup2(fd[0],fileno(stdin));
                    execvp(argv2[0],argv2);
                }                    
                else{
                    close(fd[0]);
                    wait(NULL);
                }
            }
        }
        //simple command
        else{ //pid_6
            // printf("simple command\n");
            loadCmd(input,myargv,myargc);
            int pid_6=fork();
            if(pid_6<0){
                printf("fork error");
                return 0;
            }
            else if(pid_6==0){
                execvp(myargv[0],myargv);
                exit(1);
            }else{
                wait(NULL);
            }
        }
    }
    return 0;
}
