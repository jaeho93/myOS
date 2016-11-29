#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

int fileCheck();

int main(void)
{

	if(fileCheck == FALSE)
	{
		printf("error : no myfs\n");
		exit(TRUE);
	}
