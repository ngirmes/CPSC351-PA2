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
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>

//Begin main function
int main (int argc, char * argv[])
{   
    //Struct timeval for gettimeofday()
    struct timeval *ptr = (struct timeval *)malloc(sizeof(struct timeval));
    struct timeval start_time, end_time, elapsed_time;

    //Create shared memory
    const char *name = "Shared memory";
    int fd;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(struct timeval));
    ptr = (struct timeval *)mmap(0, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

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

        //Obtain start time
        gettimeofday(&start_time, NULL);

        //Write start time to shared memory
        memcpy(ptr, &start_time, sizeof(struct timeval));

        //Send command and arguments to execvp()
        int errno = execvp(argv[1], argv + 1);
        if(errno == -1)
        {
            //Print error if execvp() == -1
            printf("Error\n");
            return 1;
        }
        exit(0);
    }

    //Execute if parent process
    else
    {
        //Wait for child process
        if (wait(0) == -1)
        {
            perror("wait");
        }
        
        //Obtain end time
        gettimeofday(&end_time, NULL);

        //Obtain start_time from shared memory
        memcpy(&start_time, ptr, sizeof(struct timeval));

        //Calculate and print elapsed time
        timersub(&end_time, &start_time, &elapsed_time);
        
        //Print elapsed time
        printf("Elapsed time: %ld.%06ld\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
        return 0; 
    }
}