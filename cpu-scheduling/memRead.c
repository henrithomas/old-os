//
//  memRead.c
//  OS - HW 5
//
//  Created by Henri Thomas on 11/18/17.
//  Copyright © 2017 Henri Thomas. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
int main(int argc, const char * argv[])
{
    int rows = 1000, cols = 1000;
    double sum = 0.0;
    double mat[rows][cols];
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            mat[i][j] = 1.61803399;
        }
    }
    for(int a = 0; a < 10000; a++)
    { 		
    	for(int i = 0; i < rows; i++)
    	{
        	for(int j = 0; j < cols; j++)
        	{
           		sum = sum + mat[i][j];
        	}
    	}	
    }
}
