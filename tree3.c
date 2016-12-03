#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree{
	char name[5];
	//struct inode * linker;
	struct tree * bro;
	struct tree * child;
}tree;

	tree * head=NULL;
	tree * tail=NULL;
void mymkdir(char*,tree**,tree**);
void myls(tree**);
void node_swap(tree*,tree**,tree**,tree**);
void print_list(tree*,char*);
void init(void);
int main(void)
{


	tree * root;
	root=(tree*)malloc(sizeof(tree));
	strcpy(root->name,"/");
	root->bro=NULL;
	root->child=NULL;
	
	head=root;
	tail=root;

	while(1){
		init();
		while(getchar() !='\n');
	}
	return 0;
}
void init()
{
	char prompt[100];
	char *word;
	char *ptr[4];
	int i=0;
	tree* upper_head=NULL;//mkdir하면서 head의 위치가 내려가기 때문에 ls쓸떄 필요함
		for(int j=0;j<4;j++)
			ptr[j]=(char*)malloc(sizeof(char)*30);
		printf("input prompt:");
		scanf("%[^\n]",prompt);
		word=strtok(prompt," ");
		while(word !=NULL){
			strcpy(ptr[i],word);
			i++;
			word=strtok(NULL," ");
		}
////////////////////////////////prompt 분석//////////////
		if(strcmp(ptr[0],"mymkdir")==0){
			if(strlen(ptr[1])>=5){
				printf("파일명은 4글자로 제한\n");
				return;
			}
			upper_head=head;
			mymkdir(ptr[1],&head,&tail);
		}
		else if(strcmp(ptr[0],"myls")==0){
			if(upper_head->child==NULL){
				printf("there is no 하위 디렉\n");
				return;
			}
			else{
			myls(&head);
			print_list(head,ptr[1]);
			}
		}
		else{
			printf("wrong commend\n");
			return;
		}

/////////////////////////////////////////////////////////////
		for(int i=0;i<4;i++)
			free(ptr[i]);
	
}
void mymkdir(char * name,tree**head,tree**tail)
{
	tree*new=(tree*)malloc(sizeof(tree));
	strcpy(new->name,name);
	new->child=NULL;
	new->bro=NULL;

	if(((*head)->child)==NULL && ((*head)->bro)==NULL){
		(*head)->child=new;
		*head=new;
		*tail=new;
	}
	else{
		(*tail)->bro=new;
		*tail=new;
	}
}
void myls(tree**head)
{
	tree *tmp_head=NULL;
	tree *prev1=NULL;
	tree *prev2=NULL;
	tree *node1=NULL;
	tree *node2=NULL;


	tmp_head=(tree*)malloc(sizeof(tree));
	tmp_head->bro=*head;

	prev1=tmp_head;

	while(prev1 !=NULL && prev1->bro !=NULL){
		node1=prev1->bro;
		prev2=node1;

		while(prev2 !=NULL && prev2->bro !=NULL){
			node2=prev2->bro;
			if(strcmp(node1->name,node2->name)>0)
				node_swap(prev1,&node1,&prev2,&node2);
		}
		*head=tmp_head->bro;
	}
	free(tmp_head);
}
void node_swap(tree *prev1,tree **node1,tree**prev2,tree**node2)
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
		(*prev2)->bro=(*node1);

		tmp=*node1;
		*node1=*node2;
		*node2=tmp;

		tmp=(*node1)->bro;
		(*node1)->bro=(*node2)->bro;
		(*node2)->bro=tmp;
	}
}
void print_list(tree * head,char *option)
{
	while(head !=NULL){
		printf("%s\n",head->name);
		head=head->bro;
	}
}



