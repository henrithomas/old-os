//
//  popen.c
//
//  Created by Henri Thomas on 9/16/17.
//

#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#define BUFFER_SIZE 1

int main(int argc, char *argv[])
{
    int pipefd[2];
    int pid;
    char buffer[BUFFER_SIZE];
    
    if(pipe(pipefd) == -1) {perror("pipe");}
    if((pid = fork()) == -1) {perror("unable to fork");}
    else if(pid == 0) //CHILD
    {
        if(dup2(pipefd[1], STDOUT_FILENO) == -1) {perror("dup2 error");}
        close(pipefd[0]);
        if (argc == 2)
        {
            write(STDOUT_FILENO,argv[1],strlen(argv[1]));
            char *args[2];
            args[0] = strdup(argv[1]);
            args[1] = NULL;
            execvp(args[0],args);
            perror("Could not execvp.");
            free(args[0]);
            free(args[1]);
        }
        else if (argc == 3)
        {
            char *args[3];
            args[0] = strdup(argv[1]);
            args[1] = strdup(argv[2]);
            args[2] = NULL;
            execvp(args[0], args);
            perror("Could not execvp.");
            for(int i = 0; i < 3; i++)
            {
                free(args[i]);
            }
        }
        else
        {
            char *args[argc];
            for (int i = 0; i < argc; i++)
            {
                if(i == (argc - 1))
                {
                    args[i] = NULL;
                }
                else
                {
                    args[i] = strdup(argv[i+1]);
                    printf("string %s",args[i]);
                }
            }
            execvp(args[0], args);
            perror("Could not execvp");
            for(int i = 0; i < argc; i++)
            {
                free(args[i]);
            }
        }
        close(pipefd[1]);
    }
    else             //PARENT
    {
        wait(NULL);
        if(dup2(pipefd[0],STDIN_FILENO) == -1) {perror("dup2 error");}
        close(pipefd[1]);
        while(read (STDIN_FILENO,buffer,BUFFER_SIZE) > 0)
        {
            write(STDOUT_FILENO,buffer,BUFFER_SIZE);
        }
        close(pipefd[0]);
    }
    return 0;
}
