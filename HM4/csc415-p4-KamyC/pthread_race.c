#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
/**
 * THESE DEFINE VALUES CANNOT BE CHANGED.
 * DOING SO WILL CAUSE POINTS TO BE DEDUCTED
 * FROM YOUR GRADE (15 points)
 */
 /** BEGIN VALUES THAT CANNOT BE CHANGED */
#define MAX_THREADS 16
#define MAX_ITERATIONS 40
/** END VALUES THAT CANNOT BE CHANGED */

int global=0;
/**
 * use this struct as a parameter for the function
    nanospleep(&ts)
 * nanosleep. 
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {0, 999999};

struct Thread{
    pthread_t tid;
    int id;
};

void *adder(void *arg){
    // nanosleep(&ts,NULL);
    int temp;
    struct Thread* args=(struct Thread*)arg;
    for(int i=0;i!=MAX_ITERATIONS;i++){
        temp=global;//Read global value and store it into temp
        temp+=3;//add 3 to the temp value
        global=temp;//store temp value into global variable
        // nanosleep(&ts, NULL);//set the nanosleep before the global value is changed
        printf("Current Value written to Global Variables by ADDER thread id: %d is %d\n", args->id,temp);  
    }
    pthread_exit(0);
}

void *subtractor(void *arg){
    // nanosleep(&ts,NULL);
    int temp;
    struct Thread* args=(struct Thread*)arg;
    for(int i=0;i!=MAX_ITERATIONS;i++){
        temp=global;//Read global value and store it into temp
        temp-=3;//subtract 3 to the temp value
        nanosleep(&ts, NULL);//set the nanosleep before the global value is changed
        global=temp;//store temp value into global variable
        printf("Current Value written to Global Variables by SUBTRACTOR thread id: %d is %d\n", args->id,temp);
    }
    pthread_exit(0);
}

int
main(int argc, char** argv)
{
    struct Thread threads[MAX_THREADS];
    int ret;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for(int t=0;t!=MAX_THREADS;t++){
     
        if(t%2==0){//even 
            threads[t].id=t+1;
            ret=pthread_create(&threads[t].tid,&attr,adder,(void *) &threads[t]);
            if(ret<0){
                perror("Error creating threads");
                return -1;
            }
        }
        else{//odd 
            threads[t].id=t+1;
            ret=pthread_create(&threads[t].tid,&attr,subtractor,(void *) &threads[t]);
            if(ret<0){
                perror("Error creating threads");
                return -1;
            }
        }
    }

    for(int t=0;t!=MAX_THREADS;t++){
        ret = pthread_join(threads[t].tid,NULL);
        if(ret<0){
            perror("Error joining threads");
        }
    }
    
    printf("Final Value of Shared Variable : %d\n",global);
    return 0;
}
