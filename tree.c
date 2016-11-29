//recent modified line:125 , mypwd()
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//주소록 보고 링크드 원리 되세김질 + 가장 중요한것은 pwd 함수를 안정적으로 구현해야 뭐든게 안정적이다
typedef struct tree{
	char name[5];
	//struct inode * linker;
	struct tree * child;
	struct tree * bro;
}tree;

tree * pwd;//node pointer

tree* mymkdir(char name[100]);
void mycd(char*address[1]);
char mypwd(tree*);
void init(tree*);

static char position[200]=" ";

//char address_analyzer(char address[100]);
int depth_analyzer(char *address[1]);

int main(void)
{
	tree * root;
	root=(tree*)malloc(sizeof(tree));
	strcpy(root->name,"/");
	root->child=NULL;
	root->bro=NULL;
	pwd=root;
	while(1)
		init(root);
return 0;
}

void init(tree*root)
{

	char tmp[100];
	char *word;
	char *ptr[4];
	int i=0;
		ptr[0]=(char*)malloc(sizeof(char)*40);
		ptr[1]=(char*)malloc(sizeof(char)*40);
		ptr[2]=(char*)malloc(sizeof(char)*40);
		ptr[3]=(char*)malloc(sizeof(char)*40);
			printf("명령처럼 쓰시오:");
			scanf("%[^\n]",tmp);
			word=strtok(tmp," ");
			while(word!=NULL){
				ptr[i]=word;
				i++;
				word=strtok(NULL," ");
			}
			if(strcmp("mycd",ptr[0])==0){
				mycd(&ptr[1]);
			}
		/*	else if(strcmp("mymkfs",ptr[0])==0){
				make
			}*/
			else if(strcmp("myls",ptr[0])==0){
				if(strcmp(ptr[1],"-l")==0)
					;
				else if(strcmp(ptr[1],"-i")==0)
					;
				else//옵션이 업는 경우
					;
			}
			else if(strcmp("mycat",ptr[0])==0){
				//connect_datablock(ptr[1]);
			}
			else if(strcmp("myshowfile",ptr[0])==0){//파일의 지정된 부분 출력 명령어
					//connect_datablock(ptr[1],ptr[2],ptr[3]);//from ptr[1] byte to ptr[2] byte, file name is ptr[3]
			}
			else if(strcmp("mypwd",ptr[0])==0){
				printf("%s\n",position);//1순위임.현재위치를 문자열 형태로 알려주는 형태의 함수를 구현하고 문자열을 리턴해주는 함수를 만들어야 한다.활용범위가 다양하다
			}
			else if(strcmp("mycp",ptr[0])==0){
				;
			}
			else if(strcmp("mycpto",ptr[0])==0){//my_file_system에 있는 파일을 일반 파일로 복사하는 명령어
				;
			}
			else if(strcmp("mycpfrom",ptr[0])==0){//일반 파일을 my_file_system의 파일로 복사하는 명령어
				;
			}
			else if(strcmp("mymkdir",ptr[0])==0){
				;
			}
			else if(strcmp("myrmdir",ptr[0])==0){
				;
			}
			else if(strcmp("myrm",ptr[0])==0){
				;
			}
			else if(strcmp("mymv",ptr[0])==0){
				;
			}
			else if(strcmp("mytouch",ptr[0])==0){//file 수정 시간 변경
				;
			}
			else if(strcmp("myshowinode",ptr[0])==0){
				;
			}
			else if(strcmp("myshowblock",ptr[0])==0){
				;
			}
			else if(strcmp("mystate",ptr[0])==0){
				;
			}
			else if(strcmp("mytree",ptr[0])==0){
				;
			}
			else if(strcmp("command",ptr[0])==0){
				;
			}
			else if(strcmp("\n",ptr[0])==0){
				;
			}//진짜 ; 처리만 한다
			else
				printf("-myfs_shell:%s:command not found\n",ptr[0]);
}				
char mypwd(tree*pwd)
{
	int length;//position배열의 길이 저장용
	int file_length;//파일명의 기링 저장용
	int i;
	length=strlen(position+1);//position의 전체길이를 저장하고 +1은 /를 고려한 것이다
	file_length=strlen(pwd->name);
	for(i=0;i<file_length;i++){
		*(position+length+i)=*((pwd->name)+1);
		
	}
	*(position+length+i+1)='/';
	*(position+length+i+2)='\0';

	return *position;
}

tree* mymkdir(char name[100])
{
	tree * new=(tree*)malloc(sizeof(tree));
	strcpy(new->name,name);
	new->child=NULL;
	new->bro=NULL;
	pwd->child=new;
	return new;
}
void mycd(char*address[1])
{
	char *word;
	char *ptr[8];
	int i=0;
	int j;
	int depth;
	depth=depth_analyzer(&address[1]);
	for(i=0;i<8;i++)
		ptr[i]=(char*)malloc(sizeof(char)*6);
	word=strtok(address[1],"/");
	while(word!=NULL){
		ptr[i]=word;
		i++;
		word=strtok(NULL," ");
	}//ptr[1]에 들어온 이동 주소를 단어별로 분석한 후 주소를 따라 이동한다
	for(i=0;i<depth;i++){
		for(j=0;strcmp(pwd->name,ptr[i]);j++)
			pwd->bro=pwd;
		pwd->child=pwd;
	}

}


int depth_analyzer(char* address[1])
{
	
	int depth=1;

	while(1){
		if(*(address[1])=='/')
			depth++;
		if(*(address[1])=='\n')
			return depth;
		address[1]++;
	}
}



