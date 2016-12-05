#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init(void);

int main(void)
{
	while(1){
		init();
		while(getchar() !='\n');
	}
	return 0;
}

void init(void)
{
	char prompt[1000];
	char *word;
	char *ptr[4];
	int i=0;
		for(int j=0;j<4;j++)
			ptr[j]=(char*)malloc(sizeof(char)*50);
		printf("input prompt:");
		scanf("%[^\n]",prompt);
		word=strtok(prompt," ");
		while(word !=NULL){
			strcpy(ptr[i],word);
			i++;
			word=strtok(NULL," ");
		}
		for(int j=0;j<4;j++)
			printf("%s\n",ptr[j]);

}
