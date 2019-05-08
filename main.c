//workload
/***********/
// 1. create empty
// 2. open files
// 3. write files
// 4. read files
// 5. delete files
/**************/
// Create data block with size of 4 kb

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#define B 4096
int  N; //file num
struct timespec started, finish;
struct timespec monitor,check;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
double elapsed;
int cond = 0;
int counter = 0;
unsigned int seed;
int ran_file_num;

pthread_t  cid;


void write_safe(int ret){
    if (ret < B ){
        fprintf(stderr, "Write num is not 4096 ");
        exit(1);
    }
}


void read_safe(int ret){
    if (ret < B ){
        fprintf(stderr, "Read num is not 4096 ");
        exit(1);
    }
}

void open_safe(int ret){
    if (ret < 0 ){
        fprintf(stderr, "File descriptor is negative. ");
        exit(1);
    }
}

void close_safe(int ret){
    if (ret < 0 ){
        fprintf(stderr, "File close is negative. ");
        exit(1);
    }
}

void remove_safe(int ret){
    if (ret != 0 ){
        fprintf(stderr, "File remove is negative. ");
        //exit(1);
    }
}


void create_file(){
    char word[11];
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        sprintf(word, "%d", file_num+1);
        int file = open(word, O_RDWR | O_CREAT | O_TRUNC, 0700);
        open_safe(file);
        close_safe(close(file));
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);

}



void open_file(){
    char word[11];
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        sprintf(word, "%d", file_num+1);
        int file = open(word, O_RDWR | O_TRUNC, 0700);
        open_safe(file);
        close_safe(close(file));
        //fsync(file);
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}


void write_file(){
    char *out = (char *) malloc(B);
    memset (out,'1',B);
    char word[11];
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        sprintf(word, "%d", file_num+1);
        int file = open(word, O_RDWR | O_TRUNC, 0700);
        open_safe(file);
        write_safe((int)write(file, out, B));
        close_safe(close(file));
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}



void read_file(){
    char *in = (char *) malloc(B);
    memset (in,'0',B);
    char word[11];
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        sprintf(word, "%d", file_num+1);
        int file = open(word, O_RDWR | O_TRUNC, 0700);
        open_safe(file);
        read_safe((int)write(file, in, B));
        close_safe(close(file));
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}

void remove_file(){
    /**** Remove ******/
    char word[11];
    int file_num;
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (file_num = 0; file_num < N; file_num++) {
        sprintf(word, "%d", file_num+1);
        remove_safe(remove(word));
        counter--;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}


void create_file_rand(){
    char word[11];
    srand(seed);
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        ran_file_num = rand();
        sprintf(word, "%d", ran_file_num);
        int file = open(word, O_RDWR | O_CREAT | O_TRUNC, 0700);
        open_safe(file);
        close_safe(close(file));
        //fprintf(stderr, "File num: %d \n", ran_file_num);
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}

void open_file_rand(){
    char word[11];
    srand(seed);
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        ran_file_num = rand();
        sprintf(word, "%d", ran_file_num);
        int file = open(word, O_RDWR | O_TRUNC, 0700);
        open_safe(file);
        close_safe(close(file));
        //fprintf(stderr, "File num: %d \n", ran_file_num);
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}

void write_file_rand(){
    char *out = (char *) malloc(B);
    memset (out,'1',B);
    char word[11];
    srand(seed);
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        ran_file_num = rand();
        sprintf(word, "%d", ran_file_num);
        int file = open(word, O_RDWR | O_TRUNC, 0700);
        open_safe(file);
        write_safe((int)write(file, out, B));
        close_safe(close(file));
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
    free(out);
}

void read_file_rand(){
    char *in = (char *) malloc(B);
    memset (in,'0',B);
    char word[11];
    srand(seed);
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (int file_num = 0; file_num < N; file_num++) {
        ran_file_num = rand();
        sprintf(word, "%d", ran_file_num);
        int file = open(word, O_RDWR | O_TRUNC, 0700);
        open_safe(file);
        read_safe((int)write(file, in, B));
        close_safe(close(file));
        //fprintf(stderr, "File num: %d \n", ran_file_num);
        counter++;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}

void remove_file_rand(){
    /**** Remove ******/
    srand(seed);
    char word[11];
    int file_num;
    clock_gettime(CLOCK_MONOTONIC, &started);
    for (file_num = 0; file_num < N; file_num++) {
        ran_file_num= rand();
        sprintf(word, "%d", ran_file_num);
        remove(word);
        counter--;
    }
    sync();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    pthread_mutex_lock(&m);
    cond++;
    pthread_mutex_unlock(&m);
}




void *timestamp( void * args){
    double clock,passed;
    clock_gettime(CLOCK_MONOTONIC, &monitor);
    while(1){
        clock_gettime(CLOCK_MONOTONIC, &check);
        clock = (check.tv_sec - monitor.tv_sec) + (check.tv_nsec - monitor.tv_nsec) / 1000000000.0;
        if(clock >= 5){
            passed = (check.tv_sec - started.tv_sec) + (check.tv_nsec - started.tv_nsec) / 1000000000.0;
            printf(" Partial exec time: %.2f sec\n",passed);
            printf(" Current file num: %d \n", counter);
            clock_gettime(CLOCK_MONOTONIC, &monitor);
        }
        pthread_mutex_lock(&m);
        if(cond != 0){
            pthread_mutex_unlock(&m);
            break;
        }
        pthread_mutex_unlock(&m);
    }
}



void print_measurement(){
    elapsed = (finish.tv_sec - started.tv_sec) + (finish.tv_nsec - started.tv_nsec) / 1000000000.0;
    printf("\n    DONE! Total execution time: %.2f sec\n",elapsed);
}


int main(int argc, char *argv[]) {
    if( argc != 2 ){
        fprintf(stderr, "Not enough or too much arguments");
        exit(1);
    }else{
        N = atoi(argv[1]);
        printf("File input num is: %d \n", N);
    }

    //seed = (unsigned)time(NULL);
    seed = 2;
/************ create **************/
    cond = 0;
    counter = 0;

    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start creating \n");
    create_file();
    printf("\n    Finish creating \n");
    pthread_join(cid, NULL);
    print_measurement();
/************ open **************/
    cond = 0;
    counter = 0;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start opening \n");
    open_file();
    printf("\n    Finish opening \n");
    pthread_join(cid, NULL);
    print_measurement();

/************ Write **************/
    cond = 0;
    counter = 0;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start writing \n");
    write_file();
    printf("\n    Finish writing \n");
    pthread_join(cid, NULL);
    print_measurement();

/************ read **************/
    cond = 0;
    counter = 0;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start reading \n");
    read_file();
    printf("\n    Finish reading \n");
    pthread_join(cid, NULL);
    print_measurement();
/************ remove **************/
    cond = 0;
    counter = N;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start removing \n");
    remove_file();
    printf("\n    Finish removing \n");
    pthread_join(cid, NULL);
    print_measurement();

/************ randomly create **************/
    cond = 0;
    counter = 0;

    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start randomly creating \n");
    create_file_rand();
    printf("\n    Finish randomly creating \n");
    pthread_join(cid, NULL);
    print_measurement();
/************ rand open **************/
    cond = 0;
    counter = 0;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start randomly opening \n");
    open_file_rand();
    printf("\n    Finish randomly opening \n");
    pthread_join(cid, NULL);
    print_measurement();

/************ Write **************/
    cond = 0;
    counter = 0;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start randomly writing \n");
    write_file_rand();
    printf("\n    Finish randomly writing \n");
    pthread_join(cid, NULL);
    print_measurement();

/************ read **************/
    cond = 0;
    counter = 0;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start randomly reading \n");
    read_file_rand();
    printf("\n    Finish randomly reading \n");
    pthread_join(cid, NULL);
    print_measurement();
/************ remove **************/
    cond = 0;
    counter = N;
    pthread_create(&cid, NULL, timestamp, NULL);
    printf("\n    Start randomly removing \n");
    remove_file_rand();
    printf("\n    Finish randomly removing \n");
    pthread_join(cid, NULL);
    print_measurement();

    return 0;
}
