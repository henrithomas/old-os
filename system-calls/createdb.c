//
//  createdb.c
//
//  Created by Henri Thomas on 9/16/17.
//
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1

struct rec
{
    unsigned int id;
    unsigned int name;
};
int main(int argc, char *argv[])
{
    int out_fd = 0;
    ssize_t werror = 0;
    struct rec r;
    if((out_fd = open(argv[1], O_CREAT | O_WRONLY, 0644)) < 1)
    {
        perror(argv[1]);
        exit(1);
    }
    if (argc > 2)
    {
        printf("Error. Too many arguments.");
        exit(1);
    }
    else
    {
        if(argc == 2)
        {
            for(int i = 0; i < 100; i++)
            {
                r.id = i;
                r.name = i * 2;
                if ((werror = write(out_fd, &r, sizeof(struct rec))) < 0){perror(argv[1]);}
            }
        }
    }
    close(out_fd);
    return(0);
}
