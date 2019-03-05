#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//You may also add more include directives as well.

// THIS VALUE CANNOT BE CHANGED.
// You should use this value when creating your buffer.
// And also to limit the amount of bytes each read CAN do.
#define BUFF_MAX 13
// DO NOT CHANGE ABOVE
#define NAME "Jinghan Cao"

int
main(int argc, char const *argv[])
{   
    char buffer[BUFF_MAX];
    char* filename=(char*)malloc(sizeof(char));
    char* destname=(char*)malloc(sizeof(char));
    
    if(filename==NULL||destname==NULL){
        perror("Memory allocation problem...");
        return 0;
    }
    printf("Welcome to the File Copy Program by %s!\n",NAME);
    printf("Enter the name of the file to copy from:\n");
    scanf("%s",filename);
    int source_des = open(filename,O_RDONLY);
    if(source_des<0){
        perror("Source File Open Error...");
        return 0;
    }
    printf("Enter the name of the file to copy to:\n");
    scanf("%s,",destname);
    int dest_des=open(destname,O_WRONLY|O_CREAT|O_EXCL,0644);
    if(dest_des<0){
        perror("Input File Error...");
        return 0;
    }
    
    int copied_bytes=0;
    int read_formatted;
    int write_formatted;
    //keep copying the file using read() and write()
    do{
        read_formatted=read(source_des,buffer,BUFF_MAX);
        if(read_formatted<0){
            perror("Source File Open Error...");
            return 0;
        }
        write_formatted=write(dest_des,buffer,read_formatted);
        if(write_formatted<0){
            perror("Input File Write Error...");
            return 0;
        }
        copied_bytes+=write_formatted;
        
    }while(read_formatted!=0);
    printf("File Copy Successful, %d bytes copied\n",copied_bytes);

    close(source_des);
    close(dest_des);
    return 0;
}
