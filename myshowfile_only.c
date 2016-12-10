#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void myshowfile(FILE *fp,LINK head,int num1,int num2,char *tmp_name);

void init(void)
{
	int num1,num2;

	else if(strcmp("myshowfile",ptr[0])==0){
		num1=atoi(ptr[1]);
		num2=atoi(ptr[2]);
		myshowfile(fp,ia[nowdir_inode-1],num1,num2,ptr[3]);//filename
	}
}

void myshowfile(FILE *fp,LINK head,int num1,int num2,char *tmp_name)
{
	int letter;
	LINK k=head;

	if(strcmp(k->name,tmp_name)==0){
		fseek(fp,16+512+1024+79*512+(128*8*(k->inode_num))-1,0);
		fseek(fp,(8*num1)-1,SEEK_CUR);
		for(int i=0;i<num2;i++){
			letter=binary_changer(fp,8);
			printf("%c",letter);
		}
	}
	else
		myshowfile(fp,head->next,num1,num2,tmp_name);
}


