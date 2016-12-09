#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void init(void);
int main(void)
{
	while(1){
		init();
		while(getchar() !='\n');
	}
	return 0;
}

void init()
{
	int i=0;
	char *word;
	char *ptr[100];
	char prompt[100];

	for(int j=0;j<100;j++)
		ptr[j]=(char*)malloc(sizeof(char)*30);
	printf("input sth:");
	scanf("%[^\n]",prompt);

	word=strtok(prompt,"/");
	while(word!=NULL){
		strcpy(ptr[i],word);
		i++;
		word=strtok(NULL,"/");
	}
	for(i=0;i<4;i++)
		printf("%s\n",ptr[i]);
}

