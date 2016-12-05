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
	char *ptr[5];
	int i=0;
		for(int j=0;j<5;j++)
			ptr[j]=(char*)malloc(sizeof(char)*50);
		printf("input prompt:");
		scanf("%[^\n]",prompt);
		word=strtok(prompt," ");
		while(word !=NULL){
			strcpy(ptr[i],word);
			i++;
			word=strtok(NULL," ");
		}

		if(strcmp("myls",ptr[0])==0){
			if(strcmp("-l",ptr[1])==0)
				printf("i am myls -l\n");
			else if(strcmp("-i",ptr[1])==0)
				printf("i'm myls -i\n");
			else 
				printf("i'm myls \n");
		}
		else if(strcmp("mycat",ptr[0])==0){
			if(strcmp(">",ptr[1])==0){
				printf("i'm mycat > ptr[2]\n");
				//ptr[2]에 파일명 집어 넣고 함수의 인자로 ptr[2]집어 넣으면 됩니다
			}
			else
				printf("i'm mycat ptr[1]\n");
		}
		else if(strcmp("myshowfile",ptr[0])==0){
			printf("i'm myshowfile ptr[1] ptr[2] ptr[3]\n");
		}
		else if(strcmp("mypwd",ptr[0])==0){
			printf("i'm mypwd\n");
		}
		else if(strcmp("mycd",ptr[0])==0){
			printf("i'm mycd ptr[1]\n");
		}
		else if(strcmp("mycp",ptr[0])==0){
			printf("i'm mycp ptr[1] ptr[2]\n");
		}
		else if(strcmp("mycpto",ptr[0])==0){
			printf("i'm mycpto ptr[1] ptr[2]\n");
		}
		else if(strcmp("mycpfrom",ptr[0])==0){
			printf("i'm mycpfrom ptr[1] ptr[2]\n");
		}
		else if(strcmp("mymkdir",ptr[0])==0){
			printf("i'm mymkdir ptr[1]\n");
		}
		else if(strcmp("myrmdir",ptr[0])==0){
			printf("i'm myrmdir ptr[1]\n");
		}
		else if(strcmp("mymv",ptr[0])==0){
			printf("i'm mymv ptr[1] ptr[2]\n");
		}
		else if(strcmp("mytouch",ptr[0])==0){
			printf("i'm mytouch ptr[1]\n");
		}
		else if(strcmp("myshowinode",ptr[0])==0){
			printf("i'm myshowinode ptr[1]\n");
		}
		else if(strcmp("myshowblock",ptr[0])==0){
			printf("i'm myshowblock ptr[1]\n");
		}
		else if(strcmp("mystate",ptr[0])==0){
			printf("i'm mystate\n");
		}
		else if(strcmp("mytree",ptr[0])==0){
			printf("i'm mytree\n");
		}
		else if(strcmp("command",ptr[0])==0){
			printf("i'm command ptr[1]\n");
		}
		else
			printf("wrong command\n");
}
