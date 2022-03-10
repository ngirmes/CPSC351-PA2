#include<sys/wait.h>
#include<sys/time.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc,char *argv[]){
    struct timeval start,end;
    int num[2];
    pipe(num);
    int pid = fork();

    if(pid < 0){
        exit(1);
    } else if(pid == 0) {
        close(num[0]);
        gettimeofday(&start, NULL);
        char buffer[100];
        sprintf(buffer,"%ld %ld", start.tv_sec,start.tv_usec);
        write(num[1],buffer,strlen(buffer)+1);
        execvp(argv[1],argv + 1);
        close(num[1]); 
    } else {
        double timer;
        long int startsec,startusec;
        close(num[1]);
        dup2(num[0],0);
        scanf("%ld",&startsec);
        scanf("%ld",&startusec);
        gettimeofday(&end,NULL);
        timer = (end.tv_sec - startsec) * 1e6;
        timer = (timer + (end.tv_usec - startusec)) * 1e-6;
        printf("Elapsed time: %0.6lf sec\n",timer);
        close(num[0]);
        wait(NULL);
    }
    
    return 0;
}