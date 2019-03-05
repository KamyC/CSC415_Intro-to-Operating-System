#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

int bufferSize;//N is the number of buffers to maintain.
int pNumber;//P is the number of producer threads.
int cNumber;//C is the number of consumer threads.
int xNumber;//X is the number of items each producer thread will produce.
int cTime;//Ctime is the how long (in seconds) each consumer thread will sleep after consuming an item.
int pTime;//Ptime is the how long (in seconds) each producer thread will sleep after producing an item

int* buffer;// 1-D array
int* checkP;//the array contains 
int* checkC;
int p=0;
int c=0;

int forEachConsumer;//number of items for each consumer
int rem;//show the result of mod%, if over consume is on, the rem shows extra items number.
int sub=0;// shows how many left after the consumption threads
int productions;//=pNUmber*xNumber

// int totalProduce=0;
// int totalConsume=0;

int item=1;//this is the item(int type) put into buffer

int rear=-1;//for circule queue, end of the queue
int front=-1;//for circule queue,front of the queue

sem_t empty;
sem_t full;

pthread_mutex_t lock;

struct Thread{
    pthread_t tid;
    int id;
    int consume_more;
};

void print_current_time(){
    time_t myTime=time(NULL);
    char *time_str=ctime(&myTime);
    time_str[strlen(time_str)-1]='\0';
    printf("Current time: %s\n",time_str);
}

void print_parameter(int bufferSize,int pNumber,int cNumber,int xNumber,int forEachConsumer,int overconsume,int ocAmout,int cTime,int pTime){
    printf("\n");
    printf("Number of Buffers: %d\n",bufferSize);
    printf("Number of Producers: %d\n",pNumber);
    printf("Number of Consumers: %d\n",cNumber);
    printf("Number of items produced by each producer: %d\n",xNumber);
    printf("Number of items consumed by each consumer: %d\n",forEachConsumer);
    printf("Over consume on? : %d\n",overconsume);
    printf("Over consume amount: %d\n",ocAmout);
    printf("Time each Producer Sleeps (seconds): %d\n",cTime);
    printf("Time each Producer Sleeps (seconds): %d\n",pTime);
    printf("\n");
}

/* 
 * Function to remove item.
 * Item removed is returned
 */
int dequeue_item()
{
    if(front==-1){
        return 0;
    }
    int temp=buffer[front];
    buffer[front]=-1;
    if(front==rear){
        front=-1;
        rear=-1;
    }
    else if(front==bufferSize-1){
        front=0;
    }
    else{
        front++;
    }
    return temp;
}

/* 
 * Function to add item.
 * Item added is returned.
 * It is up to you to determine
 * how to use the ruturn value.
 * If you decide to not use it, then ignore
 * the return value, do not change the
 * return type to void. 
 */
int enqueue_item(int item)
{
    if((front==0 && rear==bufferSize-1)||(rear==(front-1))){
        return 0;
    }
    else if(front==-1){
        front=0;
        rear=0;
        buffer[rear]=item;
    }
    else if(rear==bufferSize-1 && front!=0){
        rear=0;
        buffer[rear]=item;
    }
    else{
        rear++;
        buffer[rear]=item;
    }
    return 0;
}

void *produce_item(void *arg){
    struct Thread* args=(struct Thread*) arg;

    for(int i=0;i!=xNumber;i++){
        sem_wait(&empty);
        pthread_mutex_lock(&lock);

        enqueue_item(item);
        checkP[p++]=item;//for checking;
        printf("%d was produced by producer -> %d\n", item ,args->id);
        item++;

        pthread_mutex_unlock(&lock);
        sem_post(&full);
        sleep(pTime);
    }
}
/* 
 * Function to check elements in buffer
 */
// void printBuffer(){
//     printf("Buffer : ");
//     for(int i=0;i!=bufferSize;i++){
//         printf("%d,",buffer[i]);
//     }
//     printf("\n");
//     printf("rem: %d",rem);
// }

void *consume_item(void *arg){
    struct Thread* args=(struct Thread*) arg;
    int iteration=forEachConsumer;

    if (args->consume_more==1){
        iteration=forEachConsumer+rem;
    }
    
    for(int j=0;j!=iteration;j++){
        sem_wait(&full);
        pthread_mutex_lock(&lock);
        int temp=dequeue_item();
        checkC[c++]=temp;//for checking
        printf("%d was consumed by consumer -> %d\n",temp,args->id);

        pthread_mutex_unlock(&lock);
        sem_post(&empty);
        sleep(cTime);        
        }
}

void print(){
    printf("\nProducer Array | Consumer Array\n");
    for(int i=0;i!=productions;i++){
        printf("%d              | %d\n",checkP[i],checkC[i]);
    }
}

int validCheck(){
    for(int i=0;i!=productions;i++){
        if(checkP[i]!=checkC[i]){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    bufferSize=atoi(argv[1]);
    pNumber=atoi(argv[2]);
    cNumber=atoi(argv[3]);
    xNumber=atoi(argv[4]);
    cTime=atoi(argv[5]);
    pTime=atoi(argv[6]);
    rem=(pNumber*xNumber)%cNumber;
    forEachConsumer=(pNumber*xNumber)/cNumber;
    int overconsume=(rem==0)?0:1;
    int ocAmount=forEachConsumer+rem;//over consumer amount;
    print_current_time();

    productions=pNumber*xNumber;

    if(argc<7){
        perror("6 command like arguments N,P,C,X,Ptime,Ctime are required\n");
    }
    print_parameter(bufferSize,pNumber,cNumber,xNumber,forEachConsumer,overconsume,ocAmount,cTime,pTime);
    
    buffer=(int *)malloc(sizeof(int)*bufferSize);
    checkP=(int *)malloc(sizeof(int)*productions);
    checkC=(int *)malloc(sizeof(int)*productions);
    
    if(!buffer){
        perror("Fails to allocate memory for buffer\n");
        return -1;
    }
    if(!checkP){
        perror("Fails to allocate memory for checkP\n");
        return -1;
    }
    if(!checkC){
        perror("Fails to allocate memory for checkC\n");
        return -1;
    }
    if(pthread_mutex_init(&lock,NULL)<0){
        perror("Mutex lock init fails\n");
        return -1;
    }
    if(sem_init(&empty,0,bufferSize)<0){
        perror("Sem_init empty fails\n");
        return -1;
    }
    if(sem_init(&full,0,0)<0){
        perror("Sem_init full fails\n");
        return -1;
    }

    struct Thread producers[pNumber];//producer threads
    struct Thread consumers[cNumber];//consumer threads
    consumers[cNumber-1].consume_more=1;
    int ret;

    clock_t start,end;//for runtime
    double runtime;
    time(&start);

    //Thread of producer
    for(int i=0;i!=pNumber;i++){
        producers[i].id=i+1;
        ret=pthread_create(&producers[i].tid,NULL,produce_item,(void *) &producers[i]);
       
        if(ret<0){
            perror("Error producer thread\n");
            return -1;
        }
    }

    //Thread of consumer
    for(int i=0;i!=cNumber;i++){
        consumers[i].id=i+1;
        
        ret=pthread_create(&consumers[i].tid,NULL,consume_item,(void *) &consumers[i]);
        
        if(ret<0){
            perror("Erroor consumer thread");
            return -1;
        }
    }
    
    //Join threads
    for(int i=0;i!=pNumber;i++){
        ret=pthread_join(producers[i].tid,NULL);
        printf("Producer thread joined: %d\n",i+1);
        if(ret<0){
            perror("Thread join producers fail\n");
            return -1;
        }
    }
    for(int i=0;i!=cNumber;i++){
        ret=pthread_join(consumers[i].tid,NULL);
        printf("Consumer thread joined: %d\n",i+1);
        if(ret<0){
            perror("Thread join consumers fail\n");
            return -1;
        }
    }
   
    print_current_time();
    // printBuffer();//Use this function to check the buffer elements and rem.
    print();//print the checking result

    if(validCheck()){
        printf("\nConsume and Produce Arrays Match !!\n");
    }

    time(&end);
    runtime=difftime(end,start);//calculate the runtime
    printf("\nTotal Runtime: %.f secs\n",runtime);

    sem_destroy(&full);
    sem_destroy(&empty);
    free(buffer);
    free(checkC);
    free(checkP);

    return 0;
}
