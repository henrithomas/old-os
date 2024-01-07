//
//  accessdb.c
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
    int in_fd = 0;
    ssize_t rerror = 0;
    struct rec input;
    int offset = sizeof(struct rec);
    int idx = 0;
    int next = 0;
    if((in_fd = open(argv[1], O_RDONLY)) < 1)
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
                idx = rand() % 100;
                next = offset * idx;
                
                lseek(in_fd, next, SEEK_SET);
                if((rerror = read(in_fd, &input, offset)) < 0) {perror(argv[1]);}
                
                printf("ID = %d Name = %d\n", input.id, input.name);
            }
        }
    }
    close(in_fd);
    return(0);
}









