#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//You may also add more include directives as well.
#define NAME "Jinghan Cao"

int
main(int argc, char const *argv[])
{
	char * buffer =malloc(128);
	int bytes_formatted, bytes_written;
	if(buffer==NULL){
		perror("Error allocating memory...");
		return 1;
	}

	bytes_formatted=sprintf(buffer,"CSC 415, This program has been written by %s!\n",NAME);
	if(bytes_formatted<0){
		perror("Error formatting buffer...");
		return -1;
	}
	
	bytes_written=write(1,buffer,bytes_formatted);
	if(bytes_written<0){
		perror("Error writting to file...");
	}
	free(buffer);
	return 0;
}