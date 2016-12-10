#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void init(void)
{
	char order[100];

	for(int k=1;k<5;k++){
		strcat(order,ptr[k]);
		strcat(order," ");
	}
	system(order);
	for(int l=0;j<100;l++)
		order[l]='\0';
}

//init() 안에 있는 내용을 else if(strcmp("command",ptr[0])==0) 안아 추가만 하면 됩니다
