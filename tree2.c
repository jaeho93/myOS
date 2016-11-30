#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct tree{
	char name[5];
	//struct inode *linker;
	struct tree * bro;
	struct tree * child;
}tree;

tree * head=NULL;
tree * tail=NULL;

void init(char*,char*,char*,char*,tree*);
void mymkdir(char*);
void node_swap(tree *prev1,tree **node1,tree **prev2,tree**node2);
void myls(tree **head);
void print_list(tree*head,char *option);


int main(void)
{
	while(1){
		char prompt[100];
		char *word;
		char *ptr[4];
		int i=0;

		tree * root;//root directory 만들기
		root=(tree*)malloc(sizeof(tree));
		strcpy(root->name,"/");
		root->bro=NULL;
		root->child=NULL;
		head=root;
		tail=root;
		//inode struct 만들어지면 추가하기
//////////////////////////prompt문자열 스페이스 단위로 쪼개서 각각의 단어로 저장/////////////////////////////////
		for(int j=0;j<4;j++)//
			ptr[j]=(char*)malloc(sizeof(char)*30);//동적으로 안하면 쓰레기 값 생겨서 안됨
		printf("input prompt:");
		scanf("%[^\n]",prompt);
		word=strtok(prompt," ");
		while(word !=NULL){
			strcpy(ptr[i],word);	
			i++;
			word=strtok(NULL," ");
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			init(ptr[0],ptr[1],ptr[2],ptr[3],root);
		for(int j=0;j<4;j++)
			free(ptr[j]);
		for(int j=0;j<200;j++)
			prompt[j]='\0';//프롬프트 비워주기
		while(getchar() !='\n');//엔터를 getchar()안해주면 버퍼에 개행문자 남아 있어서 무한루프 걸림...
	}
	return 0;
}

void init(char * ptr1,char *ptr2,char *ptr3,char * ptr4,tree*root)
{
	if(strcmp(ptr1,"mymkdir")==0){
		if(strlen(ptr2)>=5){
			printf("길어\n");
			return ;
		}
		mymkdir(ptr2);
	}
	else if(strcmp(ptr1,"myls")==0){
		myls(&head);
		print_list(head,ptr2);
	}
}
void mymkdir(char * name)
{
	tree * new=(tree*)malloc(sizeof(tree));
	if(head->child==NULL){//하위 디렉이 없는경우
		head->child=new;
		strcpy(new->name,name);
		new->child=NULL;
		new->bro=NULL;
		
		head=new;
		tail=new;
	}
	else{
		tail->bro=new;
		strcpy(new->name,name);
		new->child=NULL;
		new->bro=NULL;
		tail=new;
	}
}
void myls(tree **head)
{
	tree *tmp_head=NULL;
	tree *prev1=NULL;
	tree *prev2=NULL;
	tree *node1=NULL;
	tree *node2=NULL;

	if(*head==NULL)//디렉이 없는경우
		return ;
	if((*head)->bro==NULL)//하위 디렉이 1개인 경우
		return ;
	tmp_head=(tree*)malloc(sizeof(tree));
	tmp_head->bro=*head;

	prev1=tmp_head;

	while(prev1 !=NULL && prev1->bro !=NULL){
		node1=prev1->bro;
		prev2=node1;

		while(prev2 !=NULL && prev2->bro !=NULL){
			node2=prev2->bro;

			if(strcmp(node1->name,node2->name) >0)
				node_swap(prev1,&node1,&prev2,&node2);
		}
		*head=tmp_head->bro;
	}
	free(tmp_head);
}
void node_swap(tree *prev1,tree **node1,tree **prev2,tree**node2)
{
	tree *tmp=NULL;

	if(*node1 == *prev2){
		prev1->bro=*node2;
		*prev2=*node2;

		tmp=*node1;
		*node1=*node2;
		*node2=tmp;

		(*node2)->bro=(*node1)->bro;
		(*node1)->bro=(*node2);
	}
	else{
		prev1->bro=*node2;
		(*prev2)->bro=*node1;

		tmp=*node1;
		*node1=*node2;
		*node2=tmp;

		tmp=(*node1)->bro;
		(*node1)->bro=(*node2)->bro;
		(*node2)->bro=tmp;
	}
}
void print_list(tree* head,char *option)//향후 옵션 추가 예정
{
	while(head !=NULL){
		printf("%s\n",head->name);
		head=head->bro;
	}
}
