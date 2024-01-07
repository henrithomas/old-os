//
//  read1.c
//  OS - HW2
//
//  Created by Henri Thomas on 9/13/17.
//  Copyright © 2017 Henri Thomas. All rights reserved.
//
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1

char* itoa(int val, int base)					//http://www.strudel.org.uk/itoa/
{								//implementation of itoa
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
}

int main(int argc, char *argv[])
{
    int in_fd, out_fd,rd_count;
    char terminal[] = "/dev/tty";
    char buffer[BUFFER_SIZE];
    char stmnt[] = "Number of bytes: ";
    char newln[] = "\n";
    char *buffer2;
    int readAmt = 0;
    in_fd = open(argv[1], O_RDONLY);
    out_fd = open(terminal,O_WRONLY);
    
    while((rd_count = read(in_fd, buffer, BUFFER_SIZE)) > 0)
    {
        readAmt++;
    }
    
    buffer2 = itoa(readAmt,10);
    write(out_fd, stmnt, sizeof(stmnt));
    write(out_fd, buffer2, sizeof(buffer2));
    write(out_fd, newln, sizeof(newln));
    close(in_fd);
    close(out_fd);
    return(0);
}
