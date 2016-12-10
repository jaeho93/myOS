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
void push_DB(FILE *fp, LINKDB database, int datablock);
LINKDB write_to_LINK_DB(FILE *out);
void write_to_inode_ind(FILE *fp, int inode, int size, int dir, int ind, int dou);

void direct_block_reader(FILE *fp,int direct_num,int);
void single_block_reader(FILE *fp,int single_num,int cnt);
void Double_block_reader(FILE *fp,int Double_num,int cnt);
void mycat(FILE *fp,LINK head,char*file_name);
void myshowfile(FILE *fp,LINK head,int num1,int num2,char *tmp_name);

int main(void)
{
	FILE *fp = fopen("myfs", "r+");
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
			print_list_l(ia[nowdir_inode-1], fp);
		else if(strcmp("-i",ptr[1])==0)
		{
			print_list_i(ia[nowdir_inode-1]);}
		else 
			print_list(ia[nowdir_inode-1]);
		putchar('\n');
	}
	else if(strcmp("mycat",ptr[0])==0){
		if(strcmp(">",ptr[1])==0){
			printf("i'm mycat > ptr[2]\n");
			//ptr[2]에 파일명 집어 넣고 함수의 인자로 ptr[2]집어 넣으면 됩니다
		}
		else
			mycat(fp,ia[nowdir_inode-1],ptr[1]);
	}
	else if(strcmp("myshowfile",ptr[0])==0){
		printf("i'm myshowfile ptr[1] ptr[2] ptr[3]\n");
	}
	else if(strcmp("mypwd",ptr[0])==0){
		mypwd(fp); putchar('\n');
	}
	else if(strcmp("mycd",ptr[0])==0){
		mycd(now_h, fp, ptr[1]);
	}
	else if(strcmp("mycp",ptr[0])==0){
		printf("i'm mycp ptr[1] ptr[2]\n");
	}
	else if(strcmp("mycpto",ptr[0])==0){
		printf("i'm mycpto ptr[1] ptr[2]\n");
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
		printf("i'm mymv ptr[1] ptr[2]\n");
	}
	else if(strcmp("mytouch",ptr[0])==0){
		mytouch(fp, ptr[1]);
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
		mytree(ptr[1], fp);
	}
	else if(strcmp("command",ptr[0])==0){
		printf("i'm command ptr[1]\n");
	}
	else
		printf("wrong command\n");
}
void myshowfile(FILE *fp,LINK head,int num1,int num2,char *tmp_name)
{
	int direct_db=0,single_db=0,Double_db=0;
	int c;
	int single_direct_cnt=0,Double_single_cnt=0;//single안에 있는 direct 갯수,double안에 있는 single의 갯수
	int single_rest=0;//single내에 있는 direct중 128을 못채우는 것 저장
		LINK k=head;
	inode *ip=(inode*)malloc(sizeof(inode));
	if(strcmp(k->name,tmp_name)==0){
		get_inode(fp,ip,16+512+1024+79*(k->inode_num-1));
		direct_db=ip->direct;
		single_db=ip->single;
		Double_db=ip->Double;

		if(num2<=128){//direct
			fseek(fp,16+512+1024+79*512+128*8*(direct_db-1),0);
			fseek(fp,num1-1,SEEK_CUR);
			for(int i=num1;i<=num2;i++){
				c=getc(fp);
				printf("%c",c);
			}
		}
		else if(num1>=1 && num2<=128*102){//direct,single
			fseek(fp,16+512+1024+79*512+128*8*(direct_db-1),0);
			fseek(fp,num1-1,SEEK_CUR);
			for(int j=num1;j<=128;j++){
				c=getc(fp);
				printf("%c",c);
			}
			single_direct_cnt=(num2-128)/128;
			single_rest=(num2-128)%128;
			single_block_reader(fp, single_db, single_direct_cnt);
			if(single_rest !=0)//single내 128넘지 않는 direct를 출력
				direct_block_reader(fp, num2/128, single_rest);
		}
		/*      else if(num1>=1 && num2>128*102){//direct,single,double
				fseek(fp,16+512+1024+79*512+128*8*(direct_db-1),0);
				fseek(fp,num1-1,SEEK_CUR);
				for(int i=num1-1;i<128;i++){
				c=getc(fp);
				printf("%c",c);
				}
				single_block_reader(fp, single_db,0);
				Double_single_cnt=(num2-128-(102*128));
				Double_block_reader(fp, Double_db, Double_single_cnt);


				}
				else if(num1>128 && num2<=128*102){//single

				}
				else if(num1>128 && num2>128*102){//single,double

				}
				else if(num1>128*102)//double
		 */
	}

}
void direct_block_reader(FILE *fp,int direct_num,int rest)
	//single내 direct중 128이 안되는 애를 접근해야하는 경우 rest에 그 애를 넣는다
{
	int letter;
	int basic=128;
	if(rest !=0)
		basic=rest;

	fseek(fp,16+512+1024+79*512+128*8*(direct_num-1),0);
	for(int i=0;i<basic;i++){
		letter=binary_changer(fp,8);
		printf("%c",letter);
	}
}
//single indirect의 주소로 접근해서 주소들을 10진수로 바꾼상태로 저장 하여 해당 >주소를 direct_block_reader로 보내기
void single_block_reader(FILE *fp,int single_num,int cnt)
	//cnt는 single내에서 쓰이는 direct의 갯수입니다
{
	int basic=102;
	if(cnt != 0)
		basic=cnt;//mycat할때는 102개 다쓰지만 myshowfile할때 경우에 따라 써야하>는 갯수가 다르기 때문에 쓰는 것임
	int direct[102];
	fseek(fp,16+512+1024+79*512+128*8*(single_num-1),0);
	for(int i=0;i<basic;i++){
		direct[i]=binary_changer(fp,10);
	}
	for(int j=0;j<basic;j++)
		direct_block_reader(fp,direct[j],0);
}
void Double_block_reader(FILE *fp,int Double_num,int cnt)
	//single_block_reader랑 같은 이유
{
	int basic=102;
	if(cnt !=0)
		basic=cnt;
	int single[102];
	fseek(fp,16+512+1024+79*512+128*8*(Double_num-1),0);
	for(int i=0;i<basic;i++){
		single[i]=binary_changer(fp,10);
	}
	for(int j=0;j<basic;j++)
		single_block_reader(fp,single[j],0);
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
			direct_block_reader(fp,direct_db,0);
		else if(Double_db==0){//single까지 쓴경우
			direct_block_reader(fp,direct_db,0);
			single_block_reader(fp,single_db,0);
		}
		else{//double까지 다쓴경우
			direct_block_reader(fp,direct_db,0);
			single_block_reader(fp,single_db,0);
			Double_block_reader(fp,Double_db,0);
		}
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
	FILE *out = fopen(str1, "r");
	if(out == NULL)
	{
		printf("그런 파일이 없습니다.\n\n");
		return;
	}
	int total_byte = 0, now = 0; char c;
	while((c = getc(out)) != EOF)
		total_byte++;
	fseek(out, 0, 0);
	LINKDB database = write_to_LINK_DB(out);
	int data_block = total_byte / 128;
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
		push_DB(fp, database, sb_data_empty);
		fflush(fp);
	}	

}
void push_DB(FILE *fp, LINKDB database, int datablock)
{
	char str[9] = {};
	for(int i = 0; i < 128; i++)
	{
		binary(database -> DB[i], str, 8);
		fprintf(fp, "%s", str);
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

