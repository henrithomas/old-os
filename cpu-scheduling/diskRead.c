//
//  diskRead.c
//  OS - HW 5
//
//  Created by Henri Thomas on 11/18/17.
//  Copyright © 2017 Henri Thomas. All rights reserved.
//
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, const char * argv[])
{
    int buffSize = 200, in_fd0,in_fd1,in_fd2;
    char buffer[buffSize];
    
    for(int i = 0; i < 10000000; i++)
    {
        in_fd0 = open(argv[1], O_RDONLY);
        read(in_fd0,buffer,buffSize);
        close(in_fd0);
        in_fd1 = open(argv[2], O_RDONLY);
        read(in_fd1,buffer,buffSize);
        close(in_fd1);
        in_fd2 = open(argv[3], O_RDONLY);
        read(in_fd2,buffer,buffSize);
        close(in_fd2);
    }
}
