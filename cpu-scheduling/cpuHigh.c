#include <limits.h>
#include <stdio.h>
int main(int argc, const char * argv[])
{
    int count = 0;
    for(int i = 0; i < 10; i++)
    {
    	for(int j = 0; j < INT_MAX; j++)
    	{
     	  	count++;
    	}
    }
}
