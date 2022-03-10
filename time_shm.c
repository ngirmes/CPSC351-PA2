//Nicholas Girmes, Michael Babinec, Quinn Curry, John Bither
//CPSC351
//Project 2

//Includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

//Begin main function
int main (int argc, char * argv[])
{   
    //Struct timeval for gettimeofday()
    struct timeval start_time, end_time, elapsed_time;

    //Get fork id
    pid_t pid = fork();

    //Print error if fork() == -1
    if (pid == -1)
    {
        printf("Error");
    }
    //Execute if child process
    else if (pid == 0)
    {
        gettimeofday(&start_time, NULL);
        int errno = execvp(argv[1], argv + 1);
        if(errno == -1)
        {
            //Print error if execvp() == -1
            printf("Terminated Incorrectly\n");
            return 1;
        }
    }
    //Execute if parent process
    else
    {
        //Wait for child process
        if (wait(0) == -1)
        {
            perror("wait");
        }
        gettimeofday(&end_time, NULL);

        //Calculate and print elapsed time
        timersub(&end_time, &start_time, &elapsed_time);
        printf("Elapsed time: %ld.%06ld", elapsed_time.tv_sec, elapsed_time.tv_usec);
    }
    return 0; 
}