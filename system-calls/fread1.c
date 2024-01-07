//
//  myhead.c
//
//  Created by Henri Thomas on 9/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define READ_AMT 1
int main(int argc, char* argv[])
{
    int long fSize;
    char *buffer;
    FILE *fp;
    
    if (argc > 2)
    {
        printf("Error. Too many arguments.\n");
    }
    else
    {   
        fp = fopen(argv[1], "r");
        fseek(fp, 0, SEEK_END);
        fSize = ftell(fp);
        rewind(fp);
        buffer = (char*)malloc(sizeof(char)*fSize);
        
        size_t read = fread(buffer, READ_AMT, fSize, fp);
        printf("Number of bytes: %lu\n", read);
        fclose(fp);
        free(buffer);
    }
    return 0;
}
