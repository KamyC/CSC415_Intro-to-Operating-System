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
        p=strtok(NULL," \t\n");
    }
    myargv[myargc]==NULL;
    
    int i=0;
    int index=0;// index of | 
    while(myargv[i]!=NULL){
        if(strcmp(myargv[i],"|")==0){
            index=i;
        }
        i++;
    }
    int len=myargc;
    for(int n=0;n!=index;n++){
        argv_1[n]=myargv[n];//store arguments before | in argv_1
    }
    // printf("index : %d\n",index);
    argv_1[index]=NULL;
    int k=0;
    for(int m=index+1;m!=len;m++){
        argv_2[k]=myargv[m];//store arguments after | in argv_2
        k++;
    }
    // printf("k : %d\n",k);
    argv_2[k]=NULL;
    myargc=0;//reset the input index
}

void loadDirectIn(char* input,char **myargv,int myargc){
    char *p;
    p=strtok(input," \t\n");
    char*temp[BUFFERSIZE];
    while(p!=NULL){
        temp[myargc++]=p;
        p=strtok(NULL," \t\n");
    }
    int j=0;
    for(int i=0;i!=myargc;i++){
        if(strcmp(temp[i],"<")==0){
            i++;
        }
        myargv[j]=temp[i];
        j++;
    }   
    myargv[myargc-1]=NULL;
    myargc=0;//reset the input index
}

void loadDirectOut(char* input,char **myargv,int myargc){
    char *p;
    p=strtok(input," \t\n");
    int index;
    while(p!=NULL){
        myargv[myargc++]=p;
        if(strcmp(p,">")==0||strcmp(p,">>")==0){
            index=myargc;
        }
        p=strtok(NULL," \t\n");
    }
    myargv[--index]=NULL;
    myargc=0;//reset the input index
}

void loadBak(char* input,char **myargv,int myargc){
    char *p;
    p=strtok(input," \t\n");
    while(strcmp(p,"&")!=0){
        myargv[myargc++]=p;
        p=strtok(NULL," \t\n");
    }
    myargv[myargc]=NULL;
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
int buildIn(char* input){
    int b=0;
    if(strstr(input,"cd")){
        char *p;
        p=strtok(input," \t\n");
        int c=0;
        char* arg[BUFFERSIZE];
        while(p!=NULL){
            arg[c++]=p;
            p=strtok(NULL," \t\n");
        }
        chdir(arg[1]);
        b++;
    }

    //get the current working directory
    if(strcmp(input,"pwd\n")==0){
        char *buf;
        char *cwd;
        if((buf=(char*)malloc(BUFFERSIZE))!=NULL){
            cwd=getcwd(buf,BUFFERSIZE);
        }
        printf("%s\n",buf);
        b++;
        free(buf);
    }

    //if input "exit", quit the shell
    if(strcmp(input,"exit\n")==0){
        exit(0);
        b++;
    }
    return b>0?1:0;
}

int main(int* argc, char** argv){  
    char *myargv[BUFFERSIZE];//input segment stores here
    int myargc=0;//my argument count
    char input[BUFFERSIZE];
    
    int fd[2];

    while(1){
        //read user input into the input[BUFFERSIZE]
        do{ 
            printf(PROMPT);
            if(fgets(input,BUFFERSIZE,stdin)==NULL){
                printf("\n");
                exit(0);
            };
        }while(strcmp(input,"\n")==0);

        int b=buildIn(input);//cd ,pwd ,exit

        //redirect to a file pid_1
        if(strstr(input," > ")||strstr(input," >> ")){
            char* filename;
            if(strstr(input," > ")){
                char* pos_1=strstr(input," > ");
                filename=pos_1+3;
            }
            else if(strstr(input," >> ")){
                char* pos_2=strstr(input," >> ");
                filename=pos_2+4;
            } 
           
            loadDirectOut(input,myargv,myargc);
            pid_t pid_1=fork();
            if(pid_1<0){
                printf("fork error");
                return 0;
            }
            else if(pid_1==0){
                int fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd<0){
                    perror("file open error");
                }
                dup2(fd,fileno(stdout));
                close(fd);
                execvp(myargv[0],myargv);
                exit(1);
            }else{
                wait(NULL);
            }
        }
      
        //redirect the input from file pid_2
        else if(strstr(input,"<")){
            wait(NULL);// just wait for the background to finish
            char* pos=strstr(input," < ");
            char* filename=pos+3;
            loadDirectIn(input,myargv,myargc);
            pid_t pid_2=fork();
                int fd_2=open(filename,O_RDONLY,0644);
            if(pid_2<0){
                printf("fork error");
                return 0;
            }
            else if(pid_2==0){
                if(fd_2<0){
                    perror("file open error");
                }
                dup2(fd_2,fileno(stdin));
                close(fd_2);
                execvp(myargv[0],myargv);
                perror("execvp of ls failed");
                exit(1);
            }
            // else{
            //     waitpid(pid_2,0,0);
            // } 
            close(fd_2);
            wait(NULL);
        }
        //pipe pid_3
        else if(strstr(input,"|")){
            char* argv1[BUFFERSIZE];
            char* argv2[BUFFERSIZE];
            loadPipe(input,myargv,myargc,argv1,argv2);

            pipe(fd);
           
            if(fork()==0){
                dup2(fd[1],fileno(stdout));      
                close(fd[0]);close(fd[1]);      
                execvp(argv1[0], argv1);
                perror("execvp of ls failed");
                exit(1);
            }

            if(fork() == 0) 
            {
                dup2(fd[0],fileno(stdin));         
                close(fd[1]);close(fd[0]);
                execvp(argv2[0], argv2);
                perror("execvp of wc failed");
                exit(1);
            }
            close(fd[0]);close(fd[1]);
            wait(NULL);
            wait(NULL);
        }

        //run in background pid_4
        else if(strstr(input,"&")){
            loadBak(input,myargv,myargc);
            pid_t pid_4=fork();
            if(pid_4<0){
                printf("fork error");
                return 0;
            }
            else if(pid_4==0){
                if(execvp(myargv[0],myargv)<0){
                    perror("execvp error");
                    return 0;
                }
                exit(1);
            }
            else {
                printf("%d\n",pid_4);
            }
        }

        //simple command pid_5
        else if(!b){ 
            loadCmd(input,myargv,myargc);
            pid_t pid_5=fork();
            if(pid_5<0){
                printf("fork error");
                return 0;
            }
            else if(pid_5==0){
                execvp(myargv[0],myargv);
                wait(NULL);//just in case the background is not finished
                exit(1);
            }else{
                waitpid(pid_5,0,0);
            }
        }
    }
return 0;
}