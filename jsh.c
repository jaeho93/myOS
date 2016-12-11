#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define inode_array ia

typedef struct inode
{
	int ftype;
	int itime;
	int size;
	int direct;
	int single;
	int Double;
}inode;

typedef struct linked_list_dir
{
	char name[5];
	int inode_num;
	struct linked_list_dir * sib;
	struct linked_list_dir * next;
} ELEMENT;

typedef ELEMENT *LINK;

typedef struct linked_list_data
{
	char DB[128];
	struct linked_list_data * next;
}LINK_DB;
typedef LINK_DB* LINKDB;

int pos = 0;
int p_size = 0;

int num1=0,num2=0;
int show_cnt=0;//myshowfile하기 위해 mycat()처럼 file의 전체를 펴놓고, num1 과 num2 사이의 카운트가 되면 출력하도록 변경
int SERCH = 1;
int nowdir_inode = 1; // 루트 디렉터리의 경우!!!!!!!!!!!
LINK h	   = NULL;
LINK now_h = NULL;
LINK consth = NULL;
LINK inode_array[512] = {};
char now_name[5] = "/";
char fname[5];
char pstr[1000] = {};
int SWITCH = 0;
int iacnt = 0;

LINKDB next(LINKDB P)
{
	for(int i = 0; i < (p_size-1); i++)
	{
		(P+i)->next = P+i+1;
	}
	return P;
}
LINKDB string_to_list(FILE *ifp, LINKDB P)
{
	char c;
	int t=0;
	if(pos<=128)
	{
		for(; t<pos; t++){
			c = getc(ifp);
			P->DB[t] = c;
		}
	}

	else{
		if(P->next != NULL){			// next포인터가 NULL이 아닐 때 -> 0부터 127까지 다 문자받음
			for(t=0; t<128; t++){
				c = getc(ifp);
				P->DB[t] = c;
			}
			string_to_list(ifp, P->next); 
		}
		else								//next포인터가 NULL이면은 pos까지만 문자받음
			for(t=0; t<(pos-(128*(p_size-1))); t++){
				c = getc(ifp);
				P->DB[t] = c;
			}
	}

	return P;
}

void printf_list(LINKDB P){
	char c;
	int t=0;

	if(pos<=128){
		for(; t < pos; t++)
			printf("%c",P->DB[t]);
	}

	else{
		if(P->next != NULL){
			for(t=0; t<128; t++)
				printf("%c",P->DB[t]);
			printf_list(P->next);
		}

		else
			for(t=0; t<(pos-(128*(p_size-1))); t++)
				printf("%c",P->DB[t]);
	}

	return ;
}


LINK dir_to_list(FILE *); // 트리구조를 링크드리스트롤 구현
void print_time(int);
void bin_to_str(FILE *fp);
int binary_changer(FILE *fp, int size);
void print_list(LINK head);
void print_list_i(LINK);
void print_list_l(LINK, FILE*);
void get_inode(FILE *fp, inode * ip, int num);
void link_free(LINK);
void print_inode_info(int num, FILE *fp);
int find_my_inode(LINK head, char * my_name);

int 	f_sb_inode_empty(FILE *);		// 비어있는 superblock_inode 찾아서 숫자 리턴; 
void 	u_sb_inode(FILE *, int);		// superblock_inode 사용
void    r_sb_inode(FILE *, int);		// superblock_inode 제거
void	p_sb_inode_used(FILE *);		// 사용중인 superblock_inode 출력;


int 	f_sb_data_empty(FILE *);		// 비어있는 superblock_data 찾아서 숫자 리턴; 
void 	u_sb_data(FILE *, int);		// superblock_data 사용
void    r_sb_data(FILE *, int);		// superblock_data 제거
void	p_sb_data_used(FILE *);		// 사용중인 superblock_data 출력;

void mymkdir(LINK head, FILE *fp, char *);
void write_to_inode_dir(FILE *fp, int sb_inode_empty, int sb_data_empty, char ftype);
void write_to_data_dir(FILE *fp, int sb_inode_empty, int sb_data_empty);
void link_newdir_to_list(FILE * fp, LINK head, int sb_inode_empty, char newdir_str[]);
LINK dir_to_list_sib(FILE *fp, int sb_inode_empty, LINK mother_link);
void binary(int time, char*str, int num);
void write_name_to_dir(FILE * fp, LINK head, int sb_inode_empty, char *newdir_str);
void mycd(LINK head, FILE *fp, char * gostr);
void myprompt(FILE *);
void mypwd(FILE *fp);
void init(FILE*fp);
LINK find_next(LINK head, int *cnt);
LINK find_name(LINK head, int inode);
LINK find_inode(LINK head, char * str);
LINK ia_link(LINK head);
LINK rmtree(LINK head, int inode, int *cnt);
LINK find_dir(LINK head, char *str, int *inode, LINK tmp_next);
void rm_data(FILE *fp, int cnt);
void myrmdir(FILE *fp, char *ptr);
LINK mytreeshow1(LINK head, int cnt, char *tt, char *tt2);
void mytree(char * str, FILE *fp);
void touch(FILE *fp, int inode);
void touch_new(FILE *fp, char * str);
void link_newfile(LINK head, int inode, char *fname);
void mytouch(FILE *fp, char * str);
LINK find_file(LINK head, char *str, int *inode);
LINK rmtree_file(LINK head, int inode, int *cnt);
void myrm(FILE *fp, char * str);
void mycpfrom(FILE *fp, char * str1, char * str2);
void push_DB(FILE *fp, LINKDB database, int datablock, int *cnt, int byte);
LINKDB write_to_LINK_DB(FILE *out);
void write_to_inode_ind(FILE *fp, int inode, int size, int dir, int ind, int dou);
LINKDB db_link(LINKDB * ary, LINKDB head, int *cnt);
LINKDB find_db(LINKDB head, int size, int *cnt);

void direct_block_reader(FILE *fp,int direct_num);
void single_block_reader(FILE *fp,int single_num);
void Double_block_reader(FILE *fp,int Double_num);
void mycat(FILE *fp,LINK head,char*file_name);
void myshowfile(FILE *fp,LINK head, char *tmp_name);
int sb_inode_used_checker(FILE *fp,int inode_num);//myshowinode하기 전에 사용여부 확인해	서 없으면 애초에 다음 함수 안나오게 stop시키기
void show_inode(int inode_num,FILE *fp);
void data_block_num(FILE *fp);//show inode number 
void my_show_block(FILE *fp,int block_num);
void mystate(FILE*fp); 
void mymv(FILE *fp, char *str1, char *str2);
void mv_data(FILE *fp, int cnt, char * str);
LINK mvtree_file(LINK head, int inode, int *cnt);
LINK mvfind_file(LINK head, char *str, int *inode);
void mycat2(FILE *fp, char * str1, char * str2, char * str3);
void mycp(FILE *fp, char * str1, char * str2);
void mycpto(FILE *fp, char * str1, char * str2);
void size_1_cpto(FILE *fp, FILE *out, int inode_num);
void size_2_cpto(FILE *fp, FILE *out, int inode_num);
void size_104_cpto(FILE *fp, FILE *out, int inode_num);

int main(void)
{
	FILE *fp = fopen("myfs", "r+");
	if(fp == NULL)
	{
		printf("error : no myfs\n\n");
		return 0;
	}
	fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1), 0);
	h = dir_to_list(fp);
	ia[0] = h;
	ia_link(h);

	while(1){
		init(fp);
		while(getchar() !='\n');
	}
	link_free(h);
	fclose(fp);
	return 0;
}

void init(FILE*fp)
{
	char prompt[1000];
	char *word;
	char *ptr[5];
	int i=0;
	for(int j=0;j<5;j++)
	{
		ptr[j]=(char*)calloc(50, sizeof(char));
	}
	myprompt(fp);
	scanf("%[^\n]",prompt);
	word=strtok(prompt," ");
	while(word !=NULL){
		strcpy(ptr[i],word);
		i++;
		word=strtok(NULL," ");
	}

	if(strcmp("myls",ptr[0])==0){
		if(strcmp("-l",ptr[1])==0)
		{
			if(strlen(ptr[2]) != 0)
			{
				LINK tmp = find_inode(ia[nowdir_inode-1], ptr[2]);
				if(tmp != NULL)
				{	print_list_l(ia[tmp->inode_num-1], fp);}
				else
				{printf("잘못된 인자 입력.\n\n"); return;}
			}
			else print_list_l(ia[nowdir_inode-1], fp);
		}
		else if(strcmp("-i",ptr[1])==0)
		{
			if(strlen(ptr[2]) != 0)
			{
				LINK tmp = find_inode(ia[nowdir_inode-1], ptr[2]);
				if(tmp != NULL)
				{	print_list_i(ia[tmp->inode_num-1]);}
				else
				{printf("잘못된 인자 입력.\n\n"); return;}
			}
			else print_list_i(ia[nowdir_inode-1]);
		}
		else 
		{
			if(strlen(ptr[1]) != 0)
			{
				LINK tmp = find_inode(ia[nowdir_inode-1], ptr[1]);
				if(tmp != NULL)
				{	print_list(ia[tmp->inode_num-1]); }
				else
				{printf("잘못된 인자 입력.\n\n"); return;}
			}
			else print_list(ia[nowdir_inode-1]);
		}
		printf("\n\n");
	}
	else if(strcmp("mycat",ptr[0])==0){
		if(strcmp(">",ptr[2])==0){
			mycat2(fp, ptr[1], ptr[2], ptr[3]);
		}
		else
			mycat(fp,ia[nowdir_inode-1],ptr[1]);
		ia_link(h);
	}
	else if(strcmp("myshowfile",ptr[0])==0){
		{
			if(strlen(ptr[1]) == 0 || strlen(ptr[2]) == 0 || strlen(ptr[3]) == 0)
			{
				printf("잘못된 인자를 입력했습니다.\n\n");
				return;
			}
			num1=atoi(ptr[1]);
			num2=atoi(ptr[2]);
			myshowfile(fp,ia[nowdir_inode-1],ptr[3]);
		}
	}
	else if(strcmp("mypwd",ptr[0])==0){
		mypwd(fp); putchar('\n');
	}
	else if(strcmp("mycd",ptr[0])==0){
		mycd(now_h, fp, ptr[1]);
	}
	else if(strcmp("mycp",ptr[0])==0){
		mycp(fp, ptr[1], ptr[2]);
		ia_link(h);
	}
	else if(strcmp("mycpto",ptr[0])==0){
		mycpto(fp, ptr[1], ptr[2]);
	}
	else if(strcmp("mycpfrom",ptr[0])==0){
		mycpfrom(fp, ptr[1], ptr[2]);
		ia_link(h);
	}
	else if(strcmp("mymkdir",ptr[0])==0){
		mymkdir(now_h, fp, ptr[1]);
		ia_link(h);
	}
	else if(strcmp("myrm",ptr[0])==0){
		myrm(fp, ptr[1]);
		ia_link(h);
	}
	else if(strcmp("myrmdir",ptr[0])==0){
		myrmdir(fp, ptr[1]);
		ia_link(h);
	}
	else if(strcmp("mymv",ptr[0])==0){
		mymv(fp, ptr[1], ptr[2]);
		ia_link(h);
	}
	else if(strcmp("mytouch",ptr[0])==0){
		mytouch(fp, ptr[1]);
		ia_link(h);
	}
	else if(strcmp("myshowinode",ptr[0])==0){
		int inode_number=atoi(ptr[1]);
		if(inode_number==0){
			printf("inode number는 1부터 시작합니다 다시 입력하세요\n");
			return;
		}
		if(sb_inode_used_checker(fp,inode_number)==0)//해당함수는 아이노드가 할당되지 않았다고 판단하면 0을 리턴하고 할당되면 1을 리턴합니다
			return;
		show_inode(inode_number,fp);		
	}
	else if(strcmp("myshowblock",ptr[0])==0){
		int data_block_number=atoi(ptr[1]);
		if(data_block_number==0){
			printf("block_data_number는 1부터 시작합니다 다시 입력하세요\n");
			return;
		}
		my_show_block(fp,data_block_number);	
	}
	else if(strcmp("mystate",ptr[0])==0){
		mystate(fp);
	}
	else if(strcmp("mytree",ptr[0])==0){
		mytree(ptr[1], fp);
	}
	else if(strcmp("byebye", ptr[0])==0)exit(1);
	else{
		char order[100] = {};

		for(int k=0;k<5;k++){
			strcat(order,ptr[k]);
			strcat(order," ");
		}
		system(order);
		for(int l=0;l<100;l++)
			order[l]='\0';
	}
}
void mv_data(FILE *fp, int cnt, char * str)
{	
	char newdir_str2[5];
	strcpy(newdir_str2, str);
	fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1)+(8*4+10)*(cnt-1),0);
	for(int i = 0; i < 4; i++)
	{
		char tmp_str[9] = {};
		int j = 128;
		for(int k = 0; k < 8; k++)
		{
			tmp_str[k] = newdir_str2[i] / j + '0';
			newdir_str2[i] %= j; 
			j /= 2;
		}
		fprintf(fp, "%s", tmp_str);
	}
	fflush(fp);
}
LINK mvtree_file(LINK head, int inode, int *cnt)
{
	if(head -> next -> inode_num == inode)
	{
		return NULL;
	}
	else {(*cnt)++; return mvtree_file(head -> next, inode, cnt);}
}
LINK mvfind_file(LINK head, char *str, int *inode)
{
	if(strcmp(head -> name, str) == 0)
	{
		*inode = head -> inode_num;
		return head;
	}
	else if(head -> next == NULL)
		return NULL;
	else
		return mvfind_file(head -> next, str, inode);
}

void mymv(FILE *fp, char *str1, char *str2)
{
	int mvinode = 0;
	int cnt = 2;
	if(strlen(str1) == 0)
	{
		printf("파일 이름을 지정하시오.\n\n");
		return;
	}
	if(strlen(str2) == 0)
	{
		printf("변경할 이름 혹은 이동할 디렉터리를 지정하시오.\n\n");
		return;
	}
	LINK mvfile = mvfind_file(ia[nowdir_inode-1], str1, &mvinode);
	if(mvfile == NULL)
	{
		printf("%s is not a file.\n\n", str1);
		return;
	}
	LINK tmp = find_inode(ia[nowdir_inode-1], str2);
	if(tmp == NULL || tmp -> sib == NULL)
	{
		strcpy(mvfile -> name, str2);
		mvtree_file(ia[nowdir_inode-1], mvinode, &cnt);
		mv_data(fp, cnt, str2);
		return;
	}
	else
	{
		int newmother_inode = tmp -> inode_num;
		rmtree_file(ia[nowdir_inode-1], mvinode, &cnt);
		rm_data(fp, cnt);
		write_name_to_dir(fp, ia[newmother_inode-1], mvinode, str1);
		link_newfile(ia[newmother_inode-1], mvinode, str1);
	}
}
void mycp(FILE *fp, char * str1, char * str2)
{
	if(strlen(str1) == 0)
	{
		printf("복사할 파일 이름을 지정하시오.\n\n");
		return;
	}
	if(strlen(str2) == 0)
	{
		printf("복사될 파일 이름을 지정하시오.\n\n");
		return;
	}
	int inode1 = 0;
	LINK file = mvfind_file(ia[nowdir_inode-1], str1, &inode1);
	if(file == NULL || file -> sib != NULL)
	{
		printf("%s is not a file.\n\n", str1);
		return;
	}
	inode * ip = (inode*)malloc(sizeof(inode));
	get_inode(fp, ip, 1552+79*(inode1-1));
	int size = ip -> size;
	int dir  = ip -> direct;
	int single = ip -> single;
	int Double = ip -> Double;

	int sb_inode_empty = f_sb_inode_empty(fp);
	u_sb_inode(fp, sb_inode_empty);
	write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
	link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
	int data_block = size / 128;
	if((size % 128) != 0)
		data_block++;

	if(data_block <= 128)
	{
		int sb_data_empty = f_sb_data_empty(fp);
		u_sb_data(fp,sb_data_empty);
		write_to_inode_ind(fp, sb_inode_empty, size, sb_data_empty, 0, 0);
		fseek(fp, 16+512+1024+79*512+1024*(ip -> direct-1), 0);
		char str[1025] = {};
		for(int i = 0; i < 1024; i++)
			str[i] = getc(fp);
		fseek(fp, 16+512+1024+79*512+1024*(sb_data_empty-1), 0);
		fprintf(fp, "%s", str);
	}
	if(data_block > 128 && data_block < 128*103)
	{
		int sb_data_empty = f_sb_data_empty(fp);
		u_sb_data(fp,sb_data_empty);
		write_to_inode_ind(fp, sb_inode_empty, size, sb_data_empty, 0, 0);
		fseek(fp, 16+512+1024+79*512+1024*(ip -> direct-1), 0);
		char str[1025] = {};
		for(int i = 0; i < 1024; i++)
			str[i] = getc(fp);
		fseek(fp, 16+512+1024+79*512+1024*(sb_data_empty-1), 0);
		fprintf(fp, "%s", str);

		int sb_single_empty = f_sb_data_empty(fp);
		u_sb_data(fp,sb_single_empty);
	}

}

void mycpto(FILE *fp, char * File1, char * File2)
{
	if(strlen(File1) == 0)
	{
		printf("복사할 파일 이름을 지정하시오.\n\n");
		return;
	}
	if(strlen(File2) == 0)
	{
		printf("복사될 파일 이름을 지정하시오.\n\n");
		return;
	}
	int inode1 = 0;
	LINK file = mvfind_file(ia[nowdir_inode-1], File1, &inode1);
	if(file == NULL || file -> sib != NULL)
	{
		printf("%s is not a file.\n\n", File1);
		return;
	}
	//File1의 이름의 아이노드 번호 찾기 -> 그 번호를 inode_num1 이라 하겠음
	int inode_num1 = file -> inode_num;
	fseek(fp, 16+512+1024+79*(inode_num1-1)+32,SEEK_SET);
	pos = binary_changer(fp, 17);

	if(pos<=128)
		p_size = 1;

	else{
		if((pos%128)==0)
			p_size = pos/128;
		else
			p_size = pos/128 + 1;
	}

	FILE *out = NULL;
	out = fopen(File2,"w");

	if(p_size == 1){
		size_1_cpto(fp, out, inode_num1 );
	}

	else if((p_size>1)&&(p_size<=103)){
		size_1_cpto(fp, out, inode_num1 );
		size_2_cpto(fp, out, inode_num1 );
	}

	else{
		size_1_cpto(fp, out, inode_num1 );
		size_2_cpto(fp, out, inode_num1 );
		size_104_cpto(fp, out, inode_num1 );
	}
}

void size_1_cpto(FILE *fp, FILE *out, int inode_num)
{
	fseek(fp, 16+512+1024+79*(inode_num-1)+49,0);
	int direct_num = binary_changer(fp,10);		//direct번호 찾아 냈음

	fseek(fp, 16+512+1024+79*512+(128*8*(direct_num-1)),0);
	char c;
	for(int i=0; i < ((pos%128==0)? (128) : (pos%128)); i++){
		fprintf(out, "00");		// 내용쓰기
		for(int t=128; t>=1; t/=2)
			c += (getc(fp)-'0')*t;				//2진으로 되있는 8비트를 char형으로 바꾸기
		fprintf(out,"%c",c);					// out에 써주기
	}
}

	void size_2_cpto(FILE *fp, FILE *out, int inode_num)
	{

		fseek(fp, 16+512+1024+79*(inode_num-1)+59,0);
		int single_num = binary_changer(fp,10);	
		//single번호 알아냈음.

		for(int i=0; i < (p_size-1); i++){		// 데이터블록수만큼 for문 돌릴거임
			fseek(fp, 16+512+1024+79*512+(128*8*(single_num-1)+10*i), 0);
			int insingle = binary_changer(fp, 10);

			fseek(fp, 16+512+1024+79*512+128*8*(insingle-1), 0);
			char c;
			if( i == (p_size-2) )			// 마지막 데이터 블록일 때
				for(int k=0; k < ((pos%128==0)? (128):(pos%128)); k++){
					for(int t=128; t>=1; t/=2)
						c += (getc(fp)-'0')*t;
					fprintf(out, "%c", c);
				}

			else
				for(int k=0; k<128; k++){
					for(int t=128; t>=1; k/=2)
						c += (getc(fp)-'0')*t;
					fprintf(out, "%c", c);
				}
		}
	}

	void size_104_cpto(FILE *fp, FILE *out, int inode_num)
	{
		char c;

		fseek(fp, 16+512+1024+79*(inode_num-1)+69,0);
		int double_num = binary_changer(fp,10);	

		for(int i=0; i < ((p_size-103)/102+1); i++){
			fseek(fp, 16+512+1024+79*512+(128*8*(double_num-1)+10*i), 0);
			int indoublein = binary_changer(fp,10);	

			if(i == (p_size-103)/128){

				for(int j=0; j < (((p_size-103)%102==0)? (102) :((p_size-103)%102)); j++){ 
					fseek(fp, 16+512+1024+79*512+128*8*(indoublein-1)+10*j, 0);

					if( j == ((p_size-103)%102 - 1) )
						for(int k=0; k < ((pos%128==0)? (128) : (pos%128)); k++){
							for(int t=128; t>=1; k/=2)
								c += (getc(fp)-'0')*t;
							fprintf(out, "%c", c);
						}

					else
						for(int k=0; k<128; k++){
							for(int t=128; t>=1; k/=2)
								c += (getc(fp)-'0')*t;
							fprintf(out, "%c", c);
						}
				}
			}

			else{
				for(int j=0; j < 102; j++){ 
					fseek(fp, 16+512+1024+79*512+128*8*(indoublein-1)+10*j, 0);

					if( j == ((p_size-103)%102 - 1) )
						for(int k=0; k < ((pos%128==0)? (128) : (pos%128)); k++){
							for(int t=128; t>=1; t/=2)
								c += (getc(fp)-'0')*t;
							fprintf(out, "%c", c);
						}

					else
						for(int k=0; k<128; k++){
							for(int t=128; t>=1; t/=2)
								c += (getc(fp)-'0')*t;
							fprintf(out, "%c", c);
						}
				}
			}

		}
	}
	/*
	   inode * ip = (inode*)malloc(sizeof(inode));
	   get_inode(fp, ip, 1552+79*(inode1-1));
	   int size = ip -> size;
	   int dir  = ip -> direct;
	   int single = ip -> single;
	   int Double = ip -> Double;

	   int data_block = size / 128;
	   if((size % 128) != 0)
	   data_block++;

	   if(data_block <= 128)
	   {
	   fseek(fp, 16+512+1024+79*512+1024*(ip -> direct-1), 0);
	   FILE *out = fopen(str2, "w");

	   for(int i = 0; i < 128; i++)
	   {
	   char c = 0;
	   for(int j = 128; j >= 1; j/= 2)
	   {
	   c += (getc(fp) - '0') * j;
	   }
	   fprintf(out, "%c", c);
	   }
	   fclose(out);
	   }
	   }
	   */




void mycat2(FILE *fp, char * str1, char * str2, char * str3)
{
	if(strlen(str1) == 0)
	{
		printf("연결할 파일 이름을 지정하시오.\n\n");
		return;
	}
	if(strcmp(str2, ">") != 0)
	{
		printf("잘못된 옵션.\n\n");
		return;
	}
	if(strlen(str3) == 0)
	{
		printf("연결될 파일 이름을 지정하시오.\n\n");
		return;
	}

	int inode1 = 0, inode2 = 0;
	LINK file1 = mvfind_file(ia[nowdir_inode-1], str1, &inode1);
	if(file1 == NULL)
	{
		printf("%s is not a file.\n\n", str1);
		return;
	}
	LINK file2 = mvfind_file(ia[nowdir_inode-1], str3, &inode2);
	if(file2 == NULL)
	{
		printf("%s is not a file.\n\n", str3);
		return;
	}
	if(file1 -> sib != NULL || file2 -> sib != NULL)
	{
		printf("잘못된 대상을 선택했습니다.\n\n");
		return;
	}
	int dir1 = 0, ind1 = 0, dou1 = 0, dir2 = 0, ind2 = 0, dou2 = 0, size1 = 0, size2 = 0;
	inode * ip1 = (inode*)malloc(sizeof(inode));
	inode * ip2 = (inode*)malloc(sizeof(inode));
	get_inode(fp,ip1, 1552+79*(file1 -> inode_num-1));
	get_inode(fp,ip2, 1552+79*(file2 -> inode_num-1));
	size1 = ip1 -> size;
	dir1 = ip1 -> direct;
	ind1 = ip1 -> single;
	dou1 = ip1 -> Double;
	size2 = ip2 -> size;
	dir2 = ip2 -> direct;
	ind2 = ip2 -> single;
	dou2 = ip2 -> Double;
	printf("size1= %d\n", size1);
	printf("dir1= %d\n", dir1);
	printf("ind1= %d\n", ind1);
	printf("dou1= %d\n", dou1);
	printf("size2= %d\n", size2);
	printf("dir2= %d\n", dir2);
	printf("ind2= %d\n", ind2);
	printf("dou2= %d\n", dou2);

	if((size1 <= 128) && (size2 <= 128) && ((size1+size2) <= 128))
	{
		char str[1025] = {};
		fseek(fp, 16+512+1024+79*512+(128*8)*(dir1-1), 0);
		for(int i = 0; i < size1*8; i++)
			str[i] = getc(fp);//2진수 받아와서 저장하기
		fseek(fp, 16+512+1024+79*512+(128*8)*(dir2-1)+size2*8, 0);
		fprintf(fp, "%s", str);
		size2 += size1;
		fseek(fp, 16+512+1024+79*(file2 -> inode_num-1)+32, 0);
		char sizeary[18] = {};
		binary(size2, sizeary, 17);
		fprintf(fp, "%s", sizeary);
		fflush(fp);
	}
}

void myshowfile(FILE *fp,LINK head,char *tmp_name)
{
	int direct_db=0,single_db=0,Double_db=0;
	int c;
	LINK k=head;
	inode *ip=(inode*)malloc(sizeof(inode));
	if(strcmp(k->name,tmp_name)==0){
		get_inode(fp,ip,16+512+1024+79*(k->inode_num-1));
		direct_db=ip->direct;
		single_db=ip->single;
		Double_db=ip->Double;

		if(Double_db==0 && single_db==0)//direct만 쓴경우
			direct_block_reader(fp,direct_db);
		else if(Double_db==0){//single까지 쓴경우
			direct_block_reader(fp,direct_db);
			single_block_reader(fp,single_db);
		}
		else{//double까지 다쓴경우
			direct_block_reader(fp,direct_db);
			single_block_reader(fp,single_db);
			Double_block_reader(fp,Double_db);
		}
		show_cnt=0;
		num1=0;
		num2=0;
		printf("\n\n");
	}
	else
		myshowfile(fp,head->next,tmp_name);
}
void direct_block_reader(FILE *fp,int direct_num)
	//single내 direct중 128이 안되는 애를 접근해야하는 경우 rest에 그 애를 넣는다
{
	int letter;
	int basic=128;

	fseek(fp,16+512+1024+79*512+128*8*(direct_num-1),0);
	for(int i=0;i<basic;i++){
		letter=binary_changer(fp,8);
		show_cnt++;
		if(num1==0 && num2==0)
			printf("%c",letter);
		else if(num1<=show_cnt && show_cnt <=num2)
			printf("%c",letter);
	}
}
//single indirect의 주소로 접근해서 주소들을 10진수로 바꾼상태로 저장 하여 해당 >주소를 direct_block_reader로 보내기
void single_block_reader(FILE *fp,int single_num)
	//cnt는 single내에서 쓰이는 direct의 갯수입니다
{
	int basic=102;
	int direct[102];
	fseek(fp,16+512+1024+79*512+128*8*(single_num-1),0);
	for(int i=0;i<basic;i++){
		direct[i]=binary_changer(fp,10);
	}
	for(int j=0;j<basic;j++)
		direct_block_reader(fp,direct[j]);
}
void Double_block_reader(FILE *fp,int Double_num)
	//single_block_reader랑 같은 이유
{
	int basic=102;
	int single[102];
	fseek(fp,16+512+1024+79*512+128*8*(Double_num-1),0);
	for(int i=0;i<basic;i++){
		single[i]=binary_changer(fp,10);
	}
	for(int j=0;j<basic;j++)
		single_block_reader(fp,single[j]);
}
void mycat(FILE *fp,LINK head,char*file_name)
{
	int direct_db=0,single_db=0,Double_db=0;

	inode *ip=(inode*)malloc(sizeof(inode));
	LINK k=head;
	if(strcmp(k->name,file_name)==0){
		get_inode(fp,ip,16+512+1024+79*(k->inode_num-1));//inode의 시작위치까지만보내주면 아이노드 정보 10진화 해서 ip에 다 리턴해줌
		direct_db=ip->direct;//몇번째 db에 있는지 10진수 형태로주소가지고 있음
		single_db=ip->single;
		Double_db=ip->Double;

		if(Double_db==0 && single_db==0)//direct만 쓴경우
			direct_block_reader(fp,direct_db);
		else if(Double_db==0){//single까지 쓴경우
			direct_block_reader(fp,direct_db);
			single_block_reader(fp,single_db);
		}
		else{//double까지 다쓴경우
			direct_block_reader(fp,direct_db);
			single_block_reader(fp,single_db);
			Double_block_reader(fp,Double_db);
		}
		printf("\n\n");
	}
	else
		mycat(fp,head->next,file_name);

}

LINK find_file(LINK head, char *str, int *inode)
{
	if(strcmp(head -> name, str) == 0)
	{
		if(head -> sib == NULL)
		{
			*inode = head -> inode_num;
		}
		else return NULL;
	}
	else if(head -> next == NULL)
		return NULL;
	else
		return find_file(head -> next, str, inode);
}

LINK rmtree_file(LINK head, int inode, int *cnt)
{
	if(head -> next -> inode_num == inode)
	{
		if(head -> next -> next != NULL)
		{
			LINK tmp = head -> next -> next;
			LINK tmp2 = head -> next;
			head -> next -> next = NULL;
			head -> next = tmp;
			free(tmp2);
		}
		else head -> next = NULL;
		return NULL;
	}
	else {(*cnt)++; return rmtree_file(head -> next, inode, cnt);}
}

void myrm(FILE *fp, char * str)
{
	int rminode;
	int cnt = 2;
	if(strlen(str) == 0)
	{
		printf("파일 이름을 지정하시오.\n");
		return;
	}
	LINK rmfile = find_file(ia[nowdir_inode-1], str, &rminode);
	if(rmfile == NULL)
	{
		printf("%s is not a file.\n\n", str);
		return;
	}
	rmtree_file(ia[nowdir_inode-1], rminode, &cnt);
	r_sb_inode(fp, rminode);
	r_sb_data(fp, rminode);
	rm_data(fp, cnt);
}


void touch(FILE *fp, int inode)
{
	fseek(fp, 16+512+1024+79*(inode-1)+1, 0);
	time_t touch_new;
	touch_new = time(NULL);
	char touch_str[32] = {};
	binary(touch_new, touch_str, 31);
	fprintf(fp, "%s", touch_str);
	fflush(fp);
}

void touch_new(FILE *fp, char * str)
{
	int sb_inode_empty = f_sb_inode_empty(fp);
	int sb_data_empty  = f_sb_data_empty(fp);
	u_sb_inode(fp, sb_inode_empty);
	u_sb_data(fp, sb_data_empty);
	write_to_inode_dir(fp, sb_inode_empty, sb_data_empty, '1');
	write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str);
	link_newfile(ia[nowdir_inode-1], sb_inode_empty, str);
	fflush(fp);
}

void link_newfile(LINK head, int inode, char *fname)
{
	if(head -> next == NULL)
	{
		LINK newLINK = (ELEMENT*)malloc(sizeof(ELEMENT));
		strcpy(newLINK -> name, fname);
		newLINK -> inode_num = inode;
		newLINK -> next = NULL;
		newLINK -> sib = NULL;
		head -> next = newLINK;
	}
	else link_newfile(head -> next, inode, fname);
}



void mytouch(FILE *fp, char * str)
{
	if(strlen(str) == 0)
	{
		printf("파일 이름을 지정하시오.\n");
		return;
	}
	LINK tmp = find_inode(ia[nowdir_inode-1], str);
	if(tmp == NULL)
	{
		touch_new(fp, str);
	}
	else
	{
		touch(fp, tmp -> inode_num);
	}
}
void mycpfrom(FILE *fp, char * str1, char * str2)
{
	if(strlen(str1) == 0 || strlen(str2) == 0)
	{
		printf("파일 이름을 지정하시오.\n\n");
		return;
	}
	FILE *ifp = fopen(str1, "r");
	if(ifp == NULL)
	{
		printf("그런 파일이 없습니다.\n\n");
		return;
	}
	LINKDB P = NULL;

	fseek(ifp,SEEK_SET,SEEK_END);
	pos = ftell(ifp);
	fseek(ifp,SEEK_SET,SEEK_SET);
	printf("%d",pos); //NULL문자 포함한 txt의 길이 알려줌

	if(pos<=128){
		P = (LINK_DB *)calloc(1,sizeof(LINK_DB));
		p_size = 1;
	}

	else{
		if((pos%128)==0){
			P = (LINK_DB *)calloc((pos/128), sizeof(LINK_DB));
			p_size = (pos/128);
		}

		else{
			P = (LINK_DB *)calloc((pos/128)+1, sizeof(LINK_DB));
			p_size = (pos/128)+1;
		}
	}

	P = next(P);
	P = string_to_list(ifp, P);
	printf_list(P);
	if(p_size == 1)
	{
		int sb_inode_empty = f_sb_inode_empty(fp);
		u_sb_inode(fp, sb_inode_empty);
		int sb_data_empty = f_sb_data_empty(fp);
		u_sb_data(fp, sb_data_empty);
		write_to_inode_ind(fp, sb_inode_empty, pos, sb_data_empty, 0, 0);
		write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
		link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
		fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_empty-1), 0);
		int push_cnt = 0;
		push_DB(fp, P, sb_data_empty, &push_cnt, pos);
		fflush(fp);
		fclose(ifp);
	}	
	else if((p_size > 1) && (p_size < 104))
	{
		putchar('k');
		int sb_inode_empty = f_sb_inode_empty(fp);
		u_sb_inode(fp, sb_inode_empty);
		int sb_data_empty = f_sb_data_empty(fp);
		u_sb_data(fp, sb_data_empty);
		write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
		link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
		fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_empty-1), 0);
		int push_cnt = 0;
		push_DB(fp, P, sb_data_empty, &push_cnt, pos);
		int inode_array[102] = {}, sb_data, sb_indirect_empty;
		LINKDB LINKDB_array[102]; int db_cnt = 0;
		db_link(LINKDB_array, P -> next, &db_cnt);
		sb_indirect_empty = f_sb_data_empty(fp);
		u_sb_data(fp, sb_indirect_empty);
		printf("db_cnt:%d", db_cnt);
		for(int i = 0; i < db_cnt; i++)
		{
			sb_data = f_sb_data_empty(fp);
			u_sb_data(fp, sb_data);
			inode_array[i] = sb_data;
			fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data-1), 0);
			push_DB(fp, LINKDB_array[i], sb_data, &push_cnt, pos);
		}
		fseek(fp, 16+512+1024+79+512+(128*8)*(sb_indirect_empty-1), 0);
		for(int i = 0; i < db_cnt; i++)
		{
			char str[11] = {};
			binary(inode_array[i], str, 10);
			fprintf(fp, "%s", str);
		}
		write_to_inode_ind(fp, sb_inode_empty, pos, sb_data_empty, sb_indirect_empty, 0);
		fflush(fp);
		fclose(ifp);
	}
	else
	{
		int sb_inode_empty = f_sb_inode_empty(fp);
		u_sb_inode(fp, sb_inode_empty);
		int sb_data_empty = f_sb_data_empty(fp);
		u_sb_data(fp, sb_data_empty);
		write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
		link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
		fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_empty-1), 0);
		int push_cnt = 0;
		push_DB(fp, P, sb_data_empty, &push_cnt, pos);
		int inode_array[102] = {}, sb_data, sb_indirect_empty;
		LINKDB LINKDB_array[102] = {}; int db_cnt = 0;
		db_link(LINKDB_array, P -> next, &db_cnt);
		sb_indirect_empty = f_sb_data_empty(fp);
		u_sb_data(fp, sb_indirect_empty);
		for(int i = 0; 102; i++)
		{
			sb_data = f_sb_data_empty(fp);
			u_sb_data(fp, sb_data);
			inode_array[i] = sb_data;
			fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data-1), 0);
			push_DB(fp, LINKDB_array[i], sb_data, &push_cnt, pos);
		}
		fseek(fp, 16+512+1024+79*512+(128*8)*(sb_indirect_empty-1), 0);
		for(int i = 0; i < 102; i++)
		{
			char str[11] = {};
			binary(inode_array[i], str, 10);
			fprintf(fp, "%s", str);
		}
		int sb_data_double = f_sb_data_empty(fp);
		u_sb_data(fp, sb_data_double);
		write_to_inode_ind(fp, sb_inode_empty, pos, sb_data_empty, sb_indirect_empty, sb_data_double);
		int double_array[102][102] = {}; int ind_array[102]; LINKDB LINKDBarray2[102][102] = {}; int need_ind = (pos-103*128)/128/102;
		int cnt2 = 0; LINKDB start = find_db(P, 102, &cnt2);
		if((pos-103*128)%(128*102) != 0)
			need_ind++;
		int cnt3;
		for(int j = 0; j < 102; j++)
		{
			cnt3 = 0;
			start = db_link(LINKDBarray2[j], start, &cnt3);
		}
		int now = 0;
		for(int i = 0; i < need_ind; i++)
		{
			for(int j = 0; j < 102; j++)
				if(now < p_size)
				{
					now++;
					sb_data = f_sb_data_empty(fp);
					u_sb_data(fp, sb_data);
					double_array[i][j] = sb_data;
					fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data-1), 0);
					push_DB(fp, LINKDBarray2[i][j], sb_data, &push_cnt, pos);
				}
				else break;
		}
		for(int i = 0; i < need_ind; i++)
		{
			int sb_ind = f_sb_data_empty(fp);
			ind_array[i] = sb_ind;
			u_sb_data(fp, sb_ind);
			fseek(fp, 16+512+1024+79*512+(128*8)*(sb_ind-1), 0);
			for(int j = 0; j < 102; j++)
			{
				char str[11] = {};
				binary(double_array[i][j], str, 10);
				fprintf(fp, "%s", str);
			}
		}
		fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_double-1), 0);
		for(int i = 0; i < need_ind; i++)
		{
			char str[11] = {};
			binary(ind_array[i], str, 10);
			fprintf(fp, "%s", str);
		}
		fclose(ifp);
	}
}


/*void mycpfrom(FILE *fp, char * str1, char * str2)
  {
  if(strlen(str1) == 0 || strlen(str2) == 0)
  {
  printf("파일 이름을 지정하시오.\n\n");
  return;
  }
  FILE *out = fopen(str1, "r");
  if(out == NULL)
  {
  printf("그런 파일이 없습니다.\n\n");
  return;
  }
  int total_byte = 0, now = 0; char c;
  getchar();
  while((c = getc(out)) != EOF)
  total_byte++;
  fseek(out, 0, 0);
  printf("ii%d",total_byte);
  getchar();
  putchar('k');
  LINKDB database = write_to_LINK_DB(out);
  int data_block = total_byte / 128;
  putchar('k');
  if((total_byte % 128) != 0)
  data_block++;

  if(data_block == 1)
  {
  int sb_inode_empty = f_sb_inode_empty(fp);
  u_sb_inode(fp, sb_inode_empty);
  int sb_data_empty = f_sb_data_empty(fp);
  u_sb_data(fp, sb_data_empty);
  write_to_inode_ind(fp, sb_inode_empty, total_byte, sb_data_empty, 0, 0);
  write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
  link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
  fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_empty-1), 0);
  int push_cnt = 0;
  push_DB(fp, database, sb_data_empty, &push_cnt, total_byte);
  fflush(fp);
  fclose(out);
  }	
  else if((data_block > 1) && (data_block < 104))
  {
  putchar('k');
  int sb_inode_empty = f_sb_inode_empty(fp);
  u_sb_inode(fp, sb_inode_empty);
  int sb_data_empty = f_sb_data_empty(fp);
  u_sb_data(fp, sb_data_empty);
  write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
  link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
  fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_empty-1), 0);
  int push_cnt = 0;
  push_DB(fp, database, sb_data_empty, &push_cnt, total_byte);
  int inode_array[102] = {}, sb_data, sb_indirect_empty;
  LINKDB LINKDB_array[102]; int db_cnt = 0;
  db_link(LINKDB_array, database -> next, &db_cnt);
  sb_indirect_empty = f_sb_data_empty(fp);
  u_sb_data(fp, sb_indirect_empty);
  printf("db_cnt:%d", db_cnt);
  for(int i = 0; i < db_cnt; i++)
  {
  sb_data = f_sb_data_empty(fp);
  u_sb_data(fp, sb_data);
  inode_array[i] = sb_data;
  fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data-1), 0);
  push_DB(fp, LINKDB_array[i], sb_data, &push_cnt, total_byte);
  }
  fseek(fp, 16+512+1024+79+512+(128*8)*(sb_indirect_empty-1), 0);
  for(int i = 0; i < db_cnt; i++)
  {
char str[11] = {};
binary(inode_array[i], str, 10);
fprintf(fp, "%s", str);
}
write_to_inode_ind(fp, sb_inode_empty, total_byte, sb_data_empty, sb_indirect_empty, 0);
fflush(fp);
fclose(out);
}
else
{
	int sb_inode_empty = f_sb_inode_empty(fp);
	u_sb_inode(fp, sb_inode_empty);
	int sb_data_empty = f_sb_data_empty(fp);
	u_sb_data(fp, sb_data_empty);
	write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
	link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
	fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_empty-1), 0);
	int push_cnt = 0;
	push_DB(fp, database, sb_data_empty, &push_cnt, total_byte);
	int inode_array[102] = {}, sb_data, sb_indirect_empty;
	LINKDB LINKDB_array[102] = {}; int db_cnt = 0;
	db_link(LINKDB_array, database -> next, &db_cnt);
	sb_indirect_empty = f_sb_data_empty(fp);
	u_sb_data(fp, sb_indirect_empty);
	for(int i = 0; 102; i++)
	{
		sb_data = f_sb_data_empty(fp);
		u_sb_data(fp, sb_data);
		inode_array[i] = sb_data;
		fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data-1), 0);
		push_DB(fp, LINKDB_array[i], sb_data, &push_cnt, total_byte);
	}
	fseek(fp, 16+512+1024+79*512+(128*8)*(sb_indirect_empty-1), 0);
	for(int i = 0; i < 102; i++)
	{
		char str[11] = {};
		binary(inode_array[i], str, 10);
		fprintf(fp, "%s", str);
	}
	int sb_data_double = f_sb_data_empty(fp);
	u_sb_data(fp, sb_data_double);
	write_to_inode_ind(fp, sb_inode_empty, total_byte, sb_data_empty, sb_indirect_empty, sb_data_double);
	int double_array[102][102] = {}; int ind_array[102]; LINKDB LINKDBarray2[102][102] = {}; int need_ind = (total_byte-103*128)/128/102;
	int cnt2 = 0; LINKDB start = find_db(database, 102, &cnt2);
	if((total_byte-103*128)%(128*102) != 0)
		need_ind++;
	int cnt3;
	for(int j = 0; j < 102; j++)
	{
		cnt3 = 0;
		start = db_link(LINKDBarray2[j], start, &cnt3);
	}
	int now = 0;
	for(int i = 0; i < need_ind; i++)
	{
		for(int j = 0; j < 102; j++)
			if(now < data_block)
			{
				now++;
				sb_data = f_sb_data_empty(fp);
				u_sb_data(fp, sb_data);
				double_array[i][j] = sb_data;
				fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data-1), 0);
				push_DB(fp, LINKDBarray2[i][j], sb_data, &push_cnt, total_byte);
			}
			else break;
	}
	for(int i = 0; i < need_ind; i++)
	{
		int sb_ind = f_sb_data_empty(fp);
		ind_array[i] = sb_ind;
		u_sb_data(fp, sb_ind);
		fseek(fp, 16+512+1024+79*512+(128*8)*(sb_ind-1), 0);
		for(int j = 0; j < 102; j++)
		{
			char str[11] = {};
			binary(double_array[i][j], str, 10);
			fprintf(fp, "%s", str);
		}
	}
	fseek(fp, 16+512+1024+79*512+(128*8)*(sb_data_double-1), 0);
	for(int i = 0; i < need_ind; i++)
	{
		char str[11] = {};
		binary(ind_array[i], str, 10);
		fprintf(fp, "%s", str);
	}
	fclose(out);
}

}*/
LINKDB find_db(LINKDB head, int size, int *cnt)
{
	if((*cnt)++ < size)
		find_db(head -> next, size, cnt);
	else return head;
}




LINKDB db_link(LINKDB * ary, LINKDB head, int *cnt)
{
	ary[(*cnt)++] = head;
	if(*cnt < 102)
	{
		if(head -> next != NULL)
			db_link(ary, head -> next, cnt);
		else
			return NULL;
	}
	else return head -> next;
}
void push_DB(FILE *fp, LINKDB database, int datablock, int *cnt, int byte)
{
	char str[9] = {};
	for(int i = 0; i < 128; i++)
	{
		if((*cnt)++ < byte)
		{
			binary(database -> DB[i], str, 8);
			fprintf(fp, "%s", str);
			for(int j = 0; j < 9; j++)
				str[j] = 0;
		}
		else return;
	}
}



LINKDB write_to_LINK_DB(FILE *out)
{
	int Sw = 0; char c;
	LINKDB head = (LINK_DB*)malloc(sizeof(LINK_DB));
	for(int i = 0; i < 128; i++)
	{
		if((c = getc(out)) != EOF)
			head -> DB[i] = c;
		else {Sw = 1;break;}
	}
	if(Sw = 1)
	{
		head -> next = NULL;
		return head;
	}
	else
	{
		head -> next = write_to_LINK_DB(out);
		return head;
	}
}
/*
   int c, total_byte = 0, need_data = 0;
   while((c = getc(out)) != EOF)
   {
   total_byte++;
   }
   if(total byte <= 128)
   need_data = 1;
   else{
   need_data = total_byte / 128;
   if((total_byte % 128) != 0)
   need_data++;
   }
   int indirect = 0, Double = 0;
   if(need_data > 1 && need_data < 104)
   indirect = 1;
   else if (need_data > 103)
   { indirect = 1, Double = 1; }
   if(Double == 1)
   {
   indirect += (need_data - 103) / 102;
   if((need_data-103)%102 != 0)
   indirect++;
   }
   int nd = 0, id = 0;
   if(need_data > 1)
   nd = need_data-1;
   if(indirect > 1)
   id = indirect-1;
   int * direct_array = (int *)calloc(nd+1, sizeof(int));
   int * indirect_array = (int *)calloc(id+1, sizeof(int));
   int sb_inode_empty = f_sb_inode_empty(fp);
   u_sb_inode(fp, sb_inode_empty);
   int sb_data_empty = f_sb_data_empty(fp);
   u_sb_data(fp, sb_data_empty);
   for(int i = 0; i < nd; i++)
   {
   direct_array[i] = f_sb_data_empty(fp);
   u_sb_data(fp, direct_array[i]);
   }
   for(int i = 0; i < id; i++)
   {
   indirect_array[i] = f_sb_data_empty(fp);
   u_sb_data(fp, indirect_array[i]);
   }
   int sb_indirect = 0, sb_Double = 0;
   if(indirect >= 1)
   {
   sb_indirect = f_sb_data_empty(fp);
   u_sb_data(fp, sb_indirect);
   }
   if(Direct == 1)
   {
   sb_Double = f_sb_data_empty(fp);
   u_sb_data(fp, sb_Double);
   }
   write_to_inode_ind(fp, sb_inode_empty, total_byte, sb__data_empty, sb_indirect, sb_Double);
   write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str);
   link_new_file(ia[nowdir_inode-1], sb_inode_empty, str);
   datacopy(out, fp, total_byte, sb_data_empty, direct_array, indirect_array, sb_indirect, sb_Double, nd);
   }
   void datacopy(FILE *out, FILE *fp, int size, int data, int * dataa, int * inda, int ind, int Dou, int block)
   {
   char str[9] = {}; int j = 128;
   fseek(out, 0, 0); fseek(fp, 16+512+1024+79*512+(128*8)*(data-1), 0);
   int now = 0; char c;
   for(; now <= 127; now++)
   {
   if((c = getc(out)) == EOF){return;}
   j = 128;
   for(int k = 0; k < 8; k++)
{
	str[k] = c / j + '0';
	c %= j;
	j /= 2;
}
fprintf(fp, "%s", str);
for(int i = 0; i < 8; i++)
str[i] = 0;
}
for(int i = 0; i < 102; i++)
{
	fseek(fp, 16+512+1024+79*512+(128*8)(ind-1), 0);






	*/

		void write_to_inode_ind(FILE *fp, int inode, int size, int dir, int ind, int dou)
		{
			time_t now = time(NULL);
			char strTime[32] = {};
			char strAdr[11] = {};
			char strsize[18] = {};
			binary(size, strsize, 17);
			binary(now, strTime, 31);
			fseek(fp, 16+512+1024+79*(inode-1), 0);
			fprintf(fp, "1");
			fprintf(fp, "%s", strTime);
			fprintf(fp, "%s", strsize);
			binary(dir, strAdr, 10);
			fprintf(fp, "%s", strAdr);
			binary(ind, strAdr, 10);
			fprintf(fp, "%s", strAdr);
			binary(dou, strAdr, 10);
			fprintf(fp, "%s", strAdr);
		}



	LINK mytreeshow1(LINK head, int cnt, char *tt, char *tt2)
	{
		if((strcmp(head -> name, ".") != 0) && (strcmp(head -> name, "..") != 0))
		{
			for(int i = 0; i < cnt; i++)
				printf("%s", tt2);
			printf("%s%s\n", tt, head -> name);
			if(head -> sib != NULL)
			{
				mytreeshow1(head -> sib, cnt+1, tt, tt2);
			}
		}
		if(head -> next != NULL)
			return mytreeshow1(head -> next, cnt, tt, tt2);
		else return NULL;
	}

	void mytree(char * str, FILE *fp)
	{
		int cnt = 0;
		char tt[] = "--* "; char tt2[] = "---";
		if(strlen(str) == 0)
		{
			printf("%s\n", now_name);
			mytreeshow1(ia[nowdir_inode-1], cnt, tt, tt2);
			putchar('\n');
			return;
		}
		LINK tmp = find_inode(ia[nowdir_inode-1], str);
		fseek(fp, 16+512+1024+79*(tmp -> inode_num - 1), 0);
		char ftype = getc(fp);
		if(ftype == '1')
		{
			printf("%s is not a directory.\n\n", str);
			return;
		}
		printf("%s\n", tmp -> name);
		mytreeshow1(ia[tmp->inode_num-1], cnt, tt, tt2);
		putchar('\n');
		return;

	}


	LINK find_name(LINK head, int inode)
	{
		if(head -> inode_num == inode)
			return head;
		else 
		{
			if(head -> next != NULL)
			{
				return find_name(head -> next, inode);
			}
			else return NULL;
		}
	}
	LINK find_inode(LINK head, char * str)
	{
		if(strcmp(head -> name, str) == 0)
			return head;
		else
		{
			if(head -> next != NULL)
			{
				return find_inode(head -> next, str);
			}
			else return NULL;
		}
	}

	void mycd(LINK head, FILE *fp, char * gostr)
	{
		if(strlen(gostr) == 0)
		{
			strcpy(now_name, "/");
			nowdir_inode = 1;
			return ;
		}
		int tmp = 0;
		LINK tmpLINK = NULL;
		if(strcmp(gostr, ".") == 0)
			return;
		if(strcmp(gostr, "..") == 0)
		{
			if(ia[nowdir_inode-1] -> next -> inode_num == 1)
			{
				strcpy(now_name, "/");
				nowdir_inode = 1;
				return ;
			}
			nowdir_inode = ia[nowdir_inode-1] -> next -> inode_num;
			tmp = ia[nowdir_inode-1] -> next -> inode_num;
			tmpLINK = find_name(ia[tmp-1], nowdir_inode);
			strcpy(now_name, tmpLINK -> name);
			return ;
		}
		int rminode = 0;
		LINK tmp_next;
		LINK isdir = find_dir(ia[nowdir_inode-1], gostr, &rminode, tmp_next);
		if(isdir == NULL)
		{
			printf("%s is not directory.\n\n", gostr);
			return ;
		}
		if(ia[nowdir_inode-1] -> next -> next == NULL)
		{
			printf("그런 디렉터리는 없습니다.\n\n");
			return ;
		}
		tmp = ia[nowdir_inode-1] -> next -> inode_num;
		tmpLINK = find_inode(ia[nowdir_inode-1], gostr);
		if(tmpLINK != NULL)
		{
			nowdir_inode = tmpLINK -> inode_num;
			strcpy(now_name, tmpLINK -> name);
			return;
		}
		else
		{
			printf("mycd 실패!\n\n");
			return ;
		}


	}
	LINK prompt_name(LINK head, int inode)
	{
		if(head -> inode_num == inode)
			return head;
		else
		{
			return prompt_name(head -> next, inode);
		}
	}


	void myprompt(FILE *fp)
	{
		LINK tmpLINK = NULL;
		if(nowdir_inode == 1)
		{
			printf("[ / ]$ "); return;
		}
		char pp[512][5] = {}; int cnt = 0; strcpy(pp[0], now_name);
		int mother = ia[nowdir_inode-1] -> next -> inode_num;
		int gmother = ia[mother-1] -> next -> inode_num;
		if(mother == 1)
		{printf("[ /%s ]$ ", now_name);return;}
		cnt++;
		while(mother != 1)
		{
			tmpLINK = prompt_name(ia[gmother-1], mother);
			strcpy(pp[cnt], tmpLINK -> name);
			mother = gmother;
			gmother = ia[mother-1] -> next -> inode_num;
			cnt++;
		}
		printf("[ ");
		for(int i = cnt-1; i >= 0; i--)
		{
			putchar('/');
			printf("%s", pp[i]);
		}
		printf(" ]$ ");
	}

	void mypwd(FILE *fp)
	{
		LINK tmpLINK = NULL;
		if(nowdir_inode == 1)
		{
			printf("/\n"); return;
		}
		char pp[512][5] = {}; int cnt = 0; strcpy(pp[0], now_name);
		int mother = ia[nowdir_inode-1] -> next -> inode_num;
		int gmother = ia[mother-1] -> next -> inode_num;
		if(mother == 1)
		{printf("/%s\n", now_name);return;}
		cnt++;
		while(mother != 1)
		{
			tmpLINK = prompt_name(ia[gmother-1], mother);
			strcpy(pp[cnt], tmpLINK -> name);
			mother = gmother;
			gmother = ia[mother-1] -> next -> inode_num;
			cnt++;
		}
		for(int i = cnt-1; i >= 0; i--)
		{
			putchar('/');
			printf("%s", pp[i]);
		}
		printf("\n");
	}
	LINK find_dir(LINK head, char *str, int *inode, LINK tmp_next)
	{
		if(strcmp(head -> name, str) == 0)
		{
			if(head -> sib != NULL)
			{
				*inode = head -> inode_num;
				if(head -> next != NULL)
				{
					tmp_next = head -> next;
				}
				return head -> sib;
			}
			else return NULL;
		}
		else if(head -> next == NULL)
			return NULL;
		else
			return find_dir(head -> next, str, inode, tmp_next);
	}

	LINK rmtree(LINK head, int inode, int *cnt)
	{
		if(head -> next -> inode_num == inode)
		{
			LINK tmp3 = head -> next -> sib;
			head -> next -> sib = NULL;
			link_free(tmp3);
			if(head -> next -> next != NULL)
			{
				LINK tmp = head -> next -> next;
				LINK tmp2 = head -> next;
				head -> next -> next = NULL;
				head -> next = tmp;
				free(tmp2);
			}
			else head -> next = NULL;
			return NULL;
		}
		else {(*cnt)++; return rmtree(head -> next, inode, cnt);}
	}
	void rm_data(FILE *fp, int cnt)
	{	int size = 128*8-(8*4+10)*cnt + 1;
		char * str = (char *)malloc(size);
		str[size-1] = '\0';
		fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1)+(8*4+10)*(cnt), 0);
		for(int i = 0; i < size-1; i++)
			str[i] = getc(fp);
		fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1)+(8*4+10)*(cnt-1), 0);
		fprintf(fp, "%s", str);
		for(int i = 0; i < 128*8-((8*4+10)*(cnt-1)+size-1); i++)
			fprintf(fp, "0");
		fflush(fp);
	}



	void myrmdir(FILE *fp, char *ptr)
	{
		int rminode;
		int cnt = 2;
		LINK tmp_next;
		if(strlen(ptr) == 0)
		{
			printf("파일 이름을 지정하시오.\n");
			return;
		}
		LINK rmdir = find_dir(ia[nowdir_inode-1], ptr, &rminode, tmp_next);
		if(rmdir == NULL)
		{
			printf("%s is not directory.\n\n", ptr);
			return ;
		}
		if(rmdir -> next -> next != NULL)
		{
			printf("%s is not empty.\n\n", ptr);
			return ;
		}
		rmtree(ia[nowdir_inode-1], rminode, &cnt);
		r_sb_inode(fp, rminode);
		r_sb_data(fp, rminode);
		rm_data(fp, cnt);
		ia[rminode-1] = NULL;
	}

	void mymkdir(LINK head, FILE *fp, char *ptr)
	{
		char newdir_str[5] = {};
		int sb_inode_empty = f_sb_inode_empty(fp);
		int sb_data_empty  = f_sb_data_empty(fp);
		u_sb_inode(fp, sb_inode_empty);
		u_sb_data(fp, sb_data_empty);
		write_to_inode_dir(fp, sb_inode_empty, sb_data_empty, '0');
		write_to_data_dir(fp, sb_inode_empty, sb_data_empty);
		for(int i = 0; i < 4; i++)
			newdir_str[i] = ptr[i];
		write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, newdir_str);
		link_newdir_to_list(fp, ia[nowdir_inode-1], sb_inode_empty, newdir_str);
		SWITCH = 0;
	}

	void write_to_inode_dir(FILE * fp, int sb_inode_empty, int sb_data_empty, char ftype)
	{
		time_t now = time(NULL);
		char strTime[32] = {};
		char strAdr[11] = {};
		binary(now, strTime, 31);
		fseek(fp, 16+512+1024+79*(sb_inode_empty-1), 0);
		fprintf(fp, "%c", ftype);
		fprintf(fp, "%s", strTime);
		fprintf(fp, "00000000000000000");
		binary(sb_data_empty, strAdr, 10);
		fprintf(fp, "%s", strAdr);
		fprintf(fp, "0000000000");
		fprintf(fp, "0000000000");
	}

	void write_to_data_dir(FILE *fp, int sb_inode_empty, int sb_data_empty)
	{
		char strInode[11] = {};
		char strInode_now[11] = {};
		binary(sb_inode_empty, strInode, 10);
		binary(nowdir_inode, strInode_now, 10);
		fseek(fp, 16+512+1024+79*512+128*8*(sb_data_empty-1), 0);
		fprintf(fp, "00101110");	// .
		fprintf(fp, "00000000");
		fprintf(fp, "00000000");
		fprintf(fp, "00000000");
		fprintf(fp, "%s", strInode);
		fprintf(fp, "00101110");
		fprintf(fp, "00101110");	// ..
		fprintf(fp, "00000000");
		fprintf(fp, "00000000");
		fprintf(fp, "%s", strInode_now); 	// 해당 아이노드번호
	}

	LINK find_next(LINK head, int *cnt)
	{

		if(head -> next == NULL)
		{
			return head;
		}
		else {(*cnt)++; return find_next(head -> next, cnt);}
	}

	void write_name_to_dir(FILE * fp, LINK head, int sb_inode_empty, char *newdir_str)
	{
		char newdir_str2[5];
		strcpy(newdir_str2, newdir_str);
		int inode_cnt = 2;
		LINK tmp = find_next(ia[nowdir_inode-1], &inode_cnt);
		fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1)+(8*4+10)*(inode_cnt-1),0);
		for(int i = 0; i < 4; i++)
		{
			char tmp_str[9] = {};
			int j = 128;
			for(int k = 0; k < 8; k++)
			{
				tmp_str[k] = newdir_str2[i] / j + '0';
				newdir_str2[i] %= j; 
				j /= 2;
			}
			fprintf(fp, "%s", tmp_str);
		}
		char tmp_str[11] = {};
		int j = 512;
		for(int k = 0; k < 10; k++)
		{
			tmp_str[k] = sb_inode_empty / j + '0';
			sb_inode_empty %= j;
			j /= 2;
		}
		fprintf(fp, "%s", tmp_str);
		fflush(fp);
	}



	void link_newdir_to_list(FILE *fp, LINK head, int sb_inode_empty, char newdir_str[55555])
	{
		if(head == NULL)
			return;
		else
		{
			if(head-> next == NULL)
			{
				LINK new_dir = (ELEMENT*)malloc(sizeof(ELEMENT));
				head-> next = new_dir;
				strcpy(head-> next -> name, newdir_str);
				head-> next -> inode_num = sb_inode_empty;
				head-> next -> next = NULL;
				head-> next -> sib = dir_to_list_sib(fp, sb_inode_empty, head);
				SWITCH = 1;
			}
			else if(SWITCH == 0)
				link_newdir_to_list(fp, head-> next, sb_inode_empty, newdir_str);
		}
	}

	LINK dir_to_list_sib(FILE *fp, int sb_inode_empty, LINK mother_link)
	{
		LINK head = (ELEMENT*)malloc(sizeof(ELEMENT));
		strcpy(head -> name, ".");
		head -> inode_num = sb_inode_empty;
		head -> next = (ELEMENT*)malloc(sizeof(ELEMENT));
		head -> sib = NULL;
		strcpy(head -> next -> name, "..");
		head -> next -> inode_num = nowdir_inode;
		head -> next -> next = NULL;
		head -> next -> sib = NULL;
		//head -> next -> sib = ia[nowdir_inode-1];
		ia[sb_inode_empty-1] = head;
		return head;
	}

	void bin_to_str(FILE *fp)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 128; j >= 1; j/= 2)
			{
				fname[i] += (getc(fp) - '0') * j;
			}
		}
	}

	LINK dir_to_list(FILE *fp)
	{                                                                                                                                                                         
		char ftype;
		LINK head = NULL;
		bin_to_str(fp);
		if(fname[0] == '\0')
		{
			return NULL;
		}
		else
		{
			head = (ELEMENT*)malloc(sizeof(ELEMENT));
			strcpy(head -> name, fname);
			if(strcmp(head -> name, ".") == 0)
			{ia[iacnt] = head; iacnt++;}
			for(int i = 0; i < 5; i++)
				fname[i] = 0;
			head -> inode_num = binary_changer(fp, 10);
			head -> next = dir_to_list(fp);
			if(strcmp(head -> name, ".") != 0)
			{
				fseek(fp, 1552 + 79 * (head -> inode_num-1), 0);
				if((ftype = getc(fp)) == '0')
				{
					if(strcmp(head -> name, "..") != 0)
					{
						fseek(fp, 16+512+1024+79*512+128*8*(head -> inode_num-1), 0);
						head -> sib = dir_to_list(fp);
					}
				}
				else head -> sib = NULL;
			}	
			else head -> sib = NULL;

			return head;
		}
	}

	int binary_changer(FILE *fp, int size)
	{
		char *cp;
		int num = 0, cnt = 1;
		cp = (char*)malloc(sizeof(size));

		for(int i = 0; i < size; i++)
		{
			cp[i] = getc(fp);
		}

		for(int i = size-1; i>= 0; i--)
		{
			num += (cp[i] - '0') * cnt;
			cnt *= 2;
		}

		//free(cp);
		return num;
	}

	void print_list(LINK head)
	{
		if(head == NULL)
			return;
		else
		{
			printf("%s\n", head -> name);
			print_list(head -> next);
		}
	}

	void print_list_i(LINK head)
	{
		if(head == NULL)
			return;
		else
		{
			printf("%d ", head -> inode_num);
			printf("%s\n", head -> name);
			print_list_i(head -> next);
		}
	}

	void print_list_l(LINK head, FILE *fp)
	{
		if(head == NULL)
			return;
		else
		{
			print_inode_info(head -> inode_num, fp);
			printf("%s\n", head -> name);
			print_list_l(head -> next, fp);
		}
	}

	void print_inode_info(int num, FILE *fp)
	{
		inode * ip = (inode*)malloc(sizeof(inode));
		get_inode(fp, ip, 1552 + 79 * (num-1));
		printf("%c ", (ip -> ftype) ? '-' : 'd');
		printf("%17d ", ip -> size);
		print_time(ip -> itime);
		free(ip);
	}


	void print_time(int itime)
	{
		struct tm *t;
		time_t got = itime;
		t = localtime(&got);

		printf("%d/",t -> tm_year + 1900);
		printf("%02d/",t -> tm_mon + 1);
		printf("%02d ",t -> tm_mday);
		printf("%02d:",t -> tm_hour);
		printf("%02d:",t -> tm_min);
		printf("%02d ",t -> tm_sec);
	}

	void get_inode(FILE *fp, inode * ip, int num)
	{
		fseek(fp, num, 0);

		ip -> ftype 	= binary_changer(fp, 1);
		ip -> itime 	= binary_changer(fp, 31);
		ip -> size 		= binary_changer(fp, 17);
		ip -> direct 	= binary_changer(fp, 10);
		ip -> single 	= binary_changer(fp, 10);
		ip -> Double 	= binary_changer(fp, 10);
	}

	void link_free(LINK head)
	{
		if (head -> next == NULL)
		{
			free(head);
			return;
		}
		else
		{
			link_free(head -> next);
			free(head);
			return;
		}
	}
	int f_sb_inode_empty(FILE *fp)
	{
		int cnt;

		fseek(fp, 16, 0);

		for(int i = 0; i < 512; i++)
		{
			if(getc(fp) == '0')
			{
				cnt = i+1;
				break;
			}
		}
		return cnt;
	}

	void u_sb_inode(FILE *fp, int cnt)
	{
		fseek(fp, 15+cnt, 0);  	// go superblock inode
		putc('1', fp);
		fflush(fp);
	}

	void r_sb_inode(FILE *fp, int cnt)
	{
		fseek(fp, 15+cnt, 0);  	// go superblock inode
		putc('0', fp);
		fflush(fp);
	}

	void p_sb_inode_used(FILE *fp)
	{
		//gsi(0);
		fseek(fp, 16, 0);

		printf("사용중인 sb_inode 번호 : ");
		for(int i = 0; i < 512; i++)
		{
			if(getc(fp) == '1')
			{
				printf("%d  ", ftell(fp) - 16);
			}
		}
	}

	int 	f_sb_data_empty(FILE *fp)		// 비어있는 superblock_data 찾아서 숫자 리턴; 
	{
		int cnt;

		fseek(fp, 528, 0);

		for(int i = 0; i < 1024; i++)
		{
			if(getc(fp) == '0')
			{
				cnt = i+1;
				break;
			}
		}
		return cnt;
	}
	void 	u_sb_data(FILE *fp, int cnt)		// superblock_data 사용
	{
		fseek(fp, 527+cnt, 0);  	// go superblock inode
		putc('1', fp);
		fflush(fp);
	}
	void    r_sb_data(FILE *fp, int cnt)		// superblock_data 제거
	{
		fseek(fp, 527+cnt, 0);  	// go superblock inode
		putc('0', fp);
		fflush(fp);
	}
	void	p_sb_data_used(FILE *fp)		// 사용중인 superblock_data 출력;
	{
		//gsi(0);
		fseek(fp, 528, 0);

		printf("사용중인 sb_inode 번호 : ");
		for(int i = 0; i < 1024; i++)
		{
			if(getc(fp) == '1')
			{
				printf("%d  ", ftell(fp) - 528);
			}
		}
	}

	void binary(int time, char*str, int num)
	{
		for(int i = num-1; i >= 0; i--)
		{
			str[i] = time % 2 + '0';
			time /= 2;
		}
		return;
	}
	LINK ia_link(LINK head)
	{
		if(head -> sib != NULL)
		{	ia[head -> sib -> inode_num-1] = head -> sib;
		}if(head -> next != NULL)
		return ia_link(head -> next);
		else return NULL;
	}

	int sb_inode_used_checker(FILE *fp,int inode_num)//myshowinode하기 전에 사용여부 확인해	서 없으면 애초에 다음 함수 안나오게 stop시키기
	{
		fseek(fp,16+inode_num-1,0);
		if(getc(fp)=='0')
			printf("해당 아이노드는 할당되지 않았습니다\n");
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
	void my_show_block(FILE *fp,int block_num)
	{
		int ascii;

		fseek(fp,16+512+1024+79*512+128*8*(block_num-1),0);
		for(int i=0;i<128;i++){
			ascii=binary_changer(fp,8);
			printf("%c",ascii);
			printf("\n\n");
		}
	}
	void mystate(FILE *fp)
	{
		int inode_cnt=0;
		int db_cnt=0;
		int i,j;
		int c;

		fseek(fp,16-1,0);
		for(i=0;i<512;i++){
			c=getc(fp);
			if(c=='0')
				inode_cnt++;
		}
		for(j=0;j<1024;j++){
			c=getc(fp);
			if(c=='0')
				db_cnt++;
		}
		printf("free inode : %d\n",inode_cnt);
		printf("free data block: %d\n",db_cnt);
	}
