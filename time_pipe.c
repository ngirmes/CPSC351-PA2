//Nicholas Girmes, Michael Babinec, Quinn Curry, John Bither
//CPSC351
//Project 2

//Includes
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

//Begin main function
int main(int argc,char *argv[]){

    //Struct timeval for gettimeofday
    struct timeval start,end;

    //Variables
    double timer;
    long int startsec,startusec;

    //Pipe declaration
    int num[2];
    pipe(num);

    //Get fork id
    int pid = fork();

    //Print error if fork() == -1
    if(pid == -1)
    {
        printf("Error");
    } 

    //Execute if child process
    else if(pid == 0) 
    {
        close(num[0]);

        //Obtain start time
        gettimeofday(&start, NULL);

        //Pipe transfer
        char buffer[100];
        sprintf(buffer,"%ld %ld", start.tv_sec,start.tv_usec);
        write(num[1],buffer,strlen(buffer)+1);

        //Send command and arguments to execvp()
        int errno = execvp(argv[1],argv + 1);
        if(errno == -1)
        {
            //Print error if execvp() == -1
            printf("Error\n");
            return 1;
        }
        close(num[1]); 
    }

    //Execute if parent process 
    else 
    {

        //Wait for child process
        if( wait(0) == -1)
        {
            perror("wait");
        }

        //Pipe transfer
        close(num[1]);
        dup2(num[0],0);
        scanf("%ld",&startsec);
        scanf("%ld",&startusec);

        //Obtain end time
        gettimeofday(&end,NULL);

        //Calculate and print elapsed time
        timer = (end.tv_sec - startsec) * 1e6;
        timer = (timer + (end.tv_usec - startusec)) * 1e-6;
        close(num[0]);

        //Print elapsed time
        printf("Elapsed time: %0.6lf sec\n",timer);
        return 0;
    }
}