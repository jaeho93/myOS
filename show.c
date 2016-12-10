#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void data_block_num(FILE*);
int sb_inode_used_checker(FILE*,int);
void my_show_block(FILE *fp,int block_num);
void show_inode(int,FILE*);

void init(FILE *fp)
{
	int inode_number;
	int data_block_number;

	if(strcmp("myshowinode",ptr[0])==0){
		inode_number=atoi(ptr[1]);
		if(inode_number==0){
			printf("inode number는 1부터 시작합니다 다시 입력하세요\n");
			return;
		}
		if(sb_inode_used_checker(fp,inode_number)==0)
//checker함수는 아이노드가 할당 안됐으면 0 리턴, 할당되면 1리턴 
			return;
		show_inode(inode_number,fp);
	}
	else if(strcmp("myshowblock",ptr[0])==0){
		data_block_number=atoi(ptr[1]);
		if(data_block_number==0){
			printf("block_data_number는 1부터 시작합니다 다시 입력하세요\n");
			return;
		}
		my_show_block(fp,data_block_number);    
	}
	return 0;
}
void my_show_block(FILE *fp,int block_num)
{   
	int ascii;

	fseek(fp,16+512+1024+79*512+128*8*(block_num-1),0);
	for(int i=0;i<128;i++){
		ascii=binary_changer(fp,8);
		printf("%c",ascii);
	}
}

int sb_inode_used_checker(FILE *fp,int inode_num)//myshowinode하기 전에 사용여부 확인해	서 없으면 애초에 다음 함수 안나오게 stop시키기
{   
	fseek(fp,16+inode_num-1,0);
	if(getc(fp)=='0'){
		printf("해당 아이노드는 할당되지 않았습니다\n");
		return 0;
	}
	else
		return 1;
}
void show_inode(int inode_num,FILE *fp)
{   
	inode * ip=(inode*)malloc(sizeof(inode));
	get_inode(fp,ip,1552 + 79 *(inode_num-1));
	printf("file type:");
	printf("%s\n",(ip->ftype)? "regular_file":"directory");
	printf("file size:");
	printf("%d\n",ip->size);
	printf("modified time:");
	print_time(ip->itime);
	printf("\n");
	fseek(fp,16+512+1024+79*(inode_num-1),0);
	printf("data block list:");
	data_block_num(fp);
	data_block_num(fp);
	data_block_num(fp);
	printf("\n");
	//free(ip);
}
void data_block_num(FILE *fp)//show inode number 
{
	int block_number;
	block_number=binary_changer(fp,10);
	printf("%d  ",block_number);
}

