//
//  main.c
//  OS - HW6
//
//  Created by Henri Thomas on 12/8/17.
//  Copyright © 2017 Henri Thomas. All rights reserved.
//
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#define BUFFER_TOKENS 32
#define DELIM_WHITESPACE " \t\r\n\a"
int *PageTable, vm = 0, phys = 0, bus = 0;
bool is_read_only = false;
int physical_page_frame = 0, occupied_pages = 0;
int rw = 0, vm_request = 0, vm_request_copy = 0, read_write = 0,
offset = 0, page_table_index = 0, status = 1, physical_page = 0;
char *lineIn;
char **tokens;
/*
Bit mask function from "The C Programming Language", 2E, Kernighan and Ritchie
getbits: get n bits from position p
*/
unsigned getbits(unsigned x, int p, int n)
{
    return (x >> (p + 1 - n)) & ~(~0 << n);
}
/* Function to convert a decinal number to binary number
 http://www.techcrashcourse.com/2015/08/c-program-to-convert-decimal-number-binary.html
 */
long decimalToBinary(long n) {
    int remainder;
    long binary = 0, i = 1;
    
    while(n != 0) {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
    }
    return binary;
}

long binaryToDecimal(char *n) 
{
    long decimal = 0, k = 0;
    while(k < vm) 
    {
	//printf("k: %ld decimal: %ld n[k]: %d pow: %d\n",k,decimal,*(n + k) - 48, (int)pow(2, vm - (k + 1)));
        decimal = decimal + (*(n + k) - 48) * (int)pow(2, vm - (k + 1));
	k++;
    }
    return decimal;
}

int getPhysicalPageframe(int pt_idx, int rw)
{
    physical_page_frame = 0;
    is_read_only = false;
    int i = 0, j = 0;
    if(occupied_pages < phys)
    {
        if(*(PageTable + 3 * pt_idx + 1) != 1)
        {
            //set the page frame
            *(PageTable + 3 * pt_idx) = occupied_pages;
            //set the present/absent bit
            *(PageTable + 3 * pt_idx + 1) = 1;
            //set the rw bit
            *(PageTable + 3 * pt_idx + 2) = rw;
            physical_page_frame = occupied_pages;
            occupied_pages++;
        }
        else
        {
            //reset rw bit
            *(PageTable + 3 * pt_idx + 2) = rw;
            //grab physical address since it's already present
            physical_page_frame = *(PageTable + 3 * pt_idx);
        }
    }
    else
    {
        if(*(PageTable + 3 * pt_idx + 1) != 1)
        {
            //loop through page table, looking at present/absent = 1
            for(i = 0; i < vm; i++)
            {   //check for rw = 0 and pa = 1;
                if(*(PageTable + 3 * i + 1) == 1 && *(PageTable + 3 * i + 2) == 0 && i != pt_idx)
                {
                    is_read_only = true;
                    physical_page_frame = *(PageTable + 3 * i);
                    *(PageTable + 3 * i) = 0;
                    *(PageTable + 3 * i + 1) = 0;
                    *(PageTable + 3 * i + 2) = 0;
                    printf("Virtual page %d evicted from physical page %d\n",i,physical_page_frame);
	            break;
                }
            }
            //if no read only pages
            if(!is_read_only)
            {
                for(j = 0; j < vm; j++)
                {   //check for rw = 1 and pa = 1;
                    if(*(PageTable + 3 * j + 1) == 1 && *(PageTable + 3 * j + 2) == 1 && j != pt_idx)
                    {
                        physical_page_frame = *(PageTable + 3 * j);
                        *(PageTable + 3 * j) = 0;
                        *(PageTable + 3 * j + 1) = 0;
                        *(PageTable + 3 * j + 2) = 0;
                        printf("Virtual page %d evicted from physical page %d\n",j,physical_page_frame);
                        break;
                    }
                }
            }
        }
        else
        {
            *(PageTable + 3 * pt_idx + 2) = rw;
            physical_page_frame = *(PageTable + 3 * pt_idx);
        }
    }
    if(*(PageTable + 3 * pt_idx + 1) == 0)
    {
        *(PageTable + 3 * pt_idx + 1) = 1;
    }
    *(PageTable + 3 * pt_idx) = physical_page_frame;
    *(PageTable + 3 * pt_idx + 2) = rw;
    return physical_page_frame;
}

void completePhysicalAddress(int page, int offset)
{
    printf("Physiscal page: %d  Offset: %d\n",page, offset);
    page = page << 12;
    printf("Physical address: %d\n", page | offset);
    printf("Physical address (binary): %ld\n\n", decimalToBinary(page | offset));
}

char **parse_WhiteSpace(char *line)
{
    int buff = BUFFER_TOKENS, numTokes = 0;
    char **tokens_WhiteSpace = malloc(buff * sizeof(char*));
    char *toke;
    int tokePos = 0;
    
    if(tokens_WhiteSpace == NULL)
    {
        printf("vmsim: parse-ws: init - memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    toke = strtok(line, DELIM_WHITESPACE);
    while(toke != NULL)
    {
        numTokes++;
        tokens_WhiteSpace[tokePos] = toke;
        tokePos++;
        if(tokePos >= buff)
        {
            buff += BUFFER_TOKENS;
            tokens_WhiteSpace = realloc(tokens_WhiteSpace, buff * sizeof(char*));
            if(tokens_WhiteSpace == NULL)
            {
                printf("vmsim: parse-ws: realloc - memory allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        toke = strtok(NULL, DELIM_WHITESPACE);
    }
    tokens_WhiteSpace[tokePos] = NULL;
    return tokens_WhiteSpace;
}

char *read_LineInput(void)
{
    char *line = NULL;
    size_t buffer = 0;
    getline(&line, &buffer, stdin);
    return line;
}

void loop_VMSim(void)
{
    while(status)
    {
        printf("vm-sim$ ");
        lineIn = read_LineInput();
        tokens = parse_WhiteSpace(lineIn);
        read_write = atoi(tokens[0]);
        vm_request = binaryToDecimal(tokens[1]);
       	vm_request_copy = vm_request;
       	offset = getbits(vm_request, 11, 12);
       	page_table_index = getbits(vm_request_copy, 15, 4);
       	//printf("PT index: %d offset: %d\n",page_table_index,offset);
      	physical_page = getPhysicalPageframe(page_table_index, read_write);
       	completePhysicalAddress(physical_page, offset);
	free(lineIn);
	free(tokens);
    }
}

int main(int argc, char *argv[]) {
    //set size of page table from arguments passed in
    bus = pow(2,atoi(argv[1])); vm = pow(2,atoi(argv[2])); phys = pow(2,atoi(argv[3]));
    PageTable = malloc(vm *  3 * sizeof(int));
    printf("- VM Simulator -\n");
    printf("bus: %d  vm: %d phys: %d\n",bus,vm,phys);
    loop_VMSim();
    free(PageTable);
    return 0;
}
