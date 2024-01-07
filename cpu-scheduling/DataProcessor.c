#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1
int main(int argc, char *argv[])
{
    int in_fd, out_fd;
    ssize_t rd_count;
    char buff[BUFFER_SIZE];
    if (argc > 3)
    {
        printf("Error. Too many arguments.\n");
        exit(1);
    }
    else
    {
        if(argc == 3)
        {   
            in_fd = open(argv[1], O_RDONLY);
            out_fd = open(argv[2],O_WRONLY,0644);
            
            while((rd_count = read(in_fd, buff, BUFFER_SIZE)) > 0)
            {
                write(out_fd, buff, BUFFER_SIZE);
            }
        }
    }
    close(in_fd);
    close(out_fd);
    return(0);
}

