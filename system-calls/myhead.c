//
//  myhead.c
//  OS - HW2
//
//  Created by Henri Thomas on 9/12/17.
//  Copyright © 2017 Henri Thomas. All rights reserved.
//
//  myhead.c
//
//  Created by Henri Thomas on 9/11/17.
//

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1
#define DEFAULT_LINES 10
int main(int argc, char *argv[])
{
    int in_fd, out_fd, numLines;
    ssize_t rd_count;
    int lines = 0;
    char buff[BUFFER_SIZE];
    char tempStr[10];
    char terminal[] = "/dev/tty";
    if (argc > 3)
    {
        printf("Error. Too many arguments.\n");
        exit(1);
    }
    else
    {
        if(argc == 2)
        {
            numLines = DEFAULT_LINES;
            
            in_fd = open(argv[1], O_RDONLY);
            out_fd = open(terminal,O_WRONLY);
            
            while(((rd_count = read(in_fd, buff, BUFFER_SIZE)) > 0) && (lines < numLines))
            {
                if (*buff == '\n') {lines++;}
                write(out_fd, buff, BUFFER_SIZE);
            }
        }
        else
        {
            for(int i = 0; argv[1][i] != '\0' && i < 9; i++)
            {
                tempStr[i] = argv[1][i+1];
            }
            numLines = atoi(tempStr);
            
            in_fd = open(argv[2], O_RDONLY);
            out_fd = open(terminal,O_WRONLY);
            
            while(((rd_count = read(in_fd, buff, BUFFER_SIZE)) > 0) && (lines < numLines))
            {
                if (*buff == '\n') {lines++;}
                
                write(out_fd, buff, BUFFER_SIZE);
            }
        }
    }
    close(in_fd);
    close(out_fd);
    return(0);
}
