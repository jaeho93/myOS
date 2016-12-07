#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int SWITCH;
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

int nowdir_inode = 1; // 루트 디렉터리의 경우!!!!!!!!!!!
char fname[5];
int root = 0;
char pstr[1000] = {};


LINK dir_to_list(FILE *);
void print_time(int);
void bin_to_str(FILE *fp);
int binary_changer(FILE *fp, int size);
void print_list(LINK head);
void print_list_i(LINK);
void print_list_l(LINK, FILE*);
void get_inode(FILE *fp, inode * ip, int num);
void link_free(LINK);
void print_inode_info(int num, FILE *fp);
void prompt(LINK head, FILE * fp);
char * find_my_name(LINK head, int myinode);
void mypwd(LINK head, FILE * fp);
void myrmdir(LINK head,FILE *fp);
void rm_target_dir_from_list(FILE *fp,LINK head,char*rmdir_str);

int 	f_sb_inode_empty(FILE *);		// 비어있는 superblock_inode 찾아서 숫자 리턴; 
void 	u_sb_inode(FILE *, int);		// superblock_inode 사용
void    r_sb_inode(FILE *, int);		// superblock_inode 제거
void	p_sb_inode_used(FILE *);		// 사용중인 superblock_inode 출력;


int 	f_sb_data_empty(FILE *);		// 비어있는 superblock_data 찾아서 숫자 리턴; 
void 	u_sb_data(FILE *, int);		// superblock_data 사용
void    r_sb_data(FILE *, int);		// superblock_data 제거
void	p_sb_data_used(FILE *);		// 사용중인 superblock_data 출력;

void mymkdir(LINK head, FILE *fp);
void write_to_inode_dir(FILE *fp, int sb_inode_empty, int sb_data_empty);
void write_to_data_dir(FILE *fp, int sb_inode_empty, int sb_data_empty);
void link_newdir_to_list(FILE * fp, LINK head, int sb_inode_empty, char newdir_str[]);
LINK dir_to_list_sib(FILE *fp, int sb_inode_empty);
void binary(int time, char*str, int num);
void write_name_to_dir(FILE * fp, LINK head, int sb_inode_empty, char *newdir_str);
void name_to_bin(char * fromname, char res[][9]);

int main(void)
{
	FILE *fp = fopen("myfs", "r+");

	LINK h = NULL;

	fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1), 0);
	h = dir_to_list(fp);

	prompt(h, fp);
	getchar();
	mypwd(h, fp);
	getchar();

	mymkdir(h, fp);
	//h = dir_to_list(fp);



	printf("%s\n", h -> next -> next -> name);

	putchar('\n');
	putchar('\n');
	printf("ls\n");
	print_list(h);
	printf("\nls -i\n");
	print_list_i(h);
	printf("\nls -l\n");
	print_list_l(h, fp);

	link_free(h);
	fclose(fp);
	return 0;
}
void mymkdir(LINK head, FILE *fp)
{
	int sb_inode_empty = f_sb_inode_empty(fp);
	int sb_data_empty  = f_sb_data_empty(fp);
	u_sb_inode(fp, sb_inode_empty);
	printf("빈아이노드:%d", sb_inode_empty);
	printf("빈아이노드:%d", sb_data_empty);
	u_sb_data(fp, sb_data_empty);
	write_to_inode_dir(fp, sb_inode_empty, sb_data_empty);
	write_to_data_dir(fp, sb_inode_empty, sb_data_empty);
	char newdir_str[5]= "test";
	write_name_to_dir(fp, head, sb_inode_empty, newdir_str);
	link_newdir_to_list(fp, head, sb_inode_empty, newdir_str);
	SWITCH = 0;
}

void myrmdir(LINK head,FILE *fp)
{
	char rmdir_str[5]="test";//지우고자 하는 디렉토리의 이름 저장
	rm_target_dir_from_list(fp,head,rmdir_str);
}/////////////modify is needed

void rm_target_dir_from_list(FILE *fp,LINK head,char* rmdir_str)
{
	int sb_db_num;//슈퍼블록에서 데이터블록 번호저장변수
	int target_inode_num;
	if(strcmp(head->next->name,rmdir_str)==0){
		target_inode_num=head->inode_num;
		fseek(fp,16+512+1024+79*(target_inode_num)+1+31,0);//to make sb_db's bin to 10진법
		sb_db_num=binary_changer(fp,10);
		fseek(fp,16+target_inode_num-1,0);
		fprintf(fp,"0");//해당 슈퍼블록의 아이노드 바꾸기
		fseek(fp,16+512+sb_db_num-1,0);
		fprintf(fp,"0");//해당 슈퍼블록의 데이터블록 0으로 바꾸기
		fseek(fp,16+512+1024+(79*(target_inode_num))-1,0);
		for(int i=0;i<10;i++)
			fprintf(fp,"0");//아이노드주소부분 0으로 바꾸기
		fseek(fp,16+512+1024+79*512+(128*(sb_db_num))-1,0);
		for(int i=0;i<128;i++)
			fprintf(fp,"0");//db부분 0으로 바꾸기
		head->next=NULL;//노드 끊기
	}   
	else
		printf("해당 이름을 가진 디렉이 존재하지 않음\n");

}

void write_to_inode_dir(FILE * fp, int sb_inode_empty, int sb_data_empty)
{
	time_t now = time(NULL);
	char strTime[32] = {};
	char strAdr[11] = {};
	binary(now, strTime, 31);
	fseek(fp, 16+512+1024+79*(sb_inode_empty-1), 0);
	fprintf(fp, "0");
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
	binary(sb_inode_empty, strInode, 10);
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
	fprintf(fp, "%s", strInode); 	// 해당 아이노드번호
}

void write_name_to_dir(FILE * fp, LINK head, int sb_inode_empty, char *newdir_str)
{
	char newdir_str2[5];
	strcpy(newdir_str2, newdir_str);
	LINK tmp = head -> next;
	int inode_cnt = 2;
	while(1)
	{
		if(tmp -> next != NULL)
		{
			inode_cnt++;
			tmp = tmp -> next;
		}
		else {inode_cnt++;break;}

	}
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
		printf("이름:%s/n", tmp_str);
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
}








void name_to_bin(char * fromname, char res[][9])
{
	int k = 128;
	for(int i = 0; i < 4; i++)
	{
		k = 128;
		for(int j = 0; j < 8; j++)
		{
			res[i][j] = fromname[i] / k + '0';
			fromname[i] %= k;
			k /= 2;
		}
		printf("gg\n%s\n", res[i]);
	}
}




void link_newdir_to_list(FILE *fp, LINK head, int sb_inode_empty, char newdir_str[55555])
{
	if(head == NULL)
		return;
	else
	{
		if(head -> next == NULL)
		{
			LINK new_dir = (ELEMENT*)malloc(sizeof(ELEMENT));
			head -> next = new_dir;
			strcpy(head -> next -> name, newdir_str);
			head -> next -> inode_num = sb_inode_empty;
			head -> next -> next = NULL;
			head -> next -> sib = dir_to_list_sib(fp, sb_inode_empty);
			SWITCH = 1;
		}
		else if(SWITCH == 0)
			link_newdir_to_list(fp, head -> next, sb_inode_empty, newdir_str);
	}
}

LINK dir_to_list_sib(FILE *fp, int sb_inode_empty)
{
	fseek(fp, 16+512+1024+79*512+128*8*(sb_inode_empty-1), 0);
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
		for(int i = 0; i < 5; i++)
			fname[i] = 0;
		head -> inode_num = binary_changer(fp, 10);
		head -> next = dir_to_list(fp);
		if(!strcmp(head -> name, ".") && !strcmp(head -> name, ".."))
		{
			fseek(fp, 1552 + 79 * (head -> inode_num-1), 0);
			if((ftype = getc(fp)) == '0')
			{
				fseek(fp, 16+512+1024+79*512+128*8*(head -> inode_num-1), 0);
				head -> sib = dir_to_list(fp);
			}
			else head -> sib = NULL;
		}	

		return head;
	}
}




void mypwd(LINK head, FILE * fp)
{
	int mother_inode;
	int temp = nowdir_inode;
	int pstrlen;
	mother_inode = head -> next -> inode_num;
	while(1)
	{
		if(mother_inode != 1)
		{
			root = 1;
			fseek(fp, 16+512+1024+79*512+128*8*(mother_inode-1), 0);
			LINK h2 = dir_to_list(fp);
			strcat(pstr, find_my_name(h2, temp));
			strcat(pstr," ");
			temp = mother_inode;
			mother_inode = h2 -> next -> inode_num;
			free(h2);
		}
		else
			break;
	}
	if(root == 1)
	{
		pstrlen = strlen(pstr);
		for(int i = 0; pstrlen - 1; i ++)
		{
			if(pstr[i] == ' ')
				pstr[i] = 0;
		}
	}

	printf("/");
	if(root == 1)
	{
		for(int i = pstrlen - 1 ; i >= 0; i--)
		{
			if(pstr[i] == '/')
				break;
			else if(pstr[i] == '\0')
			{
				printf("%s", pstr[i+1]);
				printf("/");
			}	
		}
	}
	printf("\n");
}

void prompt(LINK head, FILE * fp)
{
	int mother_inode;
	int temp = nowdir_inode;
	int pstrlen;
	mother_inode = head -> next -> inode_num;
	while(1)
	{
		if(mother_inode != 1)
		{
			root = 1;
			fseek(fp, 16+512+1024+79*512+128*8*(mother_inode-1), 0);
			LINK h2 = dir_to_list(fp);
			strcat(pstr, find_my_name(h2, temp));
			strcat(pstr," ");
			temp = mother_inode;
			mother_inode = h2 -> next -> inode_num;
			free(h2);
		}
		else
			break;
	}
	if(root == 1)
	{
		pstrlen = strlen(pstr);
		for(int i = 0; pstrlen - 1; i ++)
		{
			if(pstr[i] == ' ')
				pstr[i] = 0;
		}
	}

	printf("[ /");
	if(root == 1)
	{
		for(int i = pstrlen - 1 ; i >= 0; i--)
		{
			if(pstr[i] == '/')
				break;
			else if(pstr[i] == '\0')
			{
				printf("%s", pstr[i+1]);
				printf("/");
			}	
		}
	}
	printf(" ]$ ");
}

char * find_my_name(LINK head, int myinode)
{
	char * ans;
	if(head -> inode_num == myinode)
		return head -> name;
	else
	{
		ans = find_my_name(head -> next, myinode);
		if(ans != NULL)
			return ans;
		else
			return NULL;
	}
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
		for(int i = 0; i < 5; i++)
			fname[i] = 0;
		head -> inode_num = binary_changer(fp, 10);
		head -> next = dir_to_list(fp);
		if(!strcmp(head -> name, ".") && !strcmp(head -> name, ".."))
		{
			fseek(fp, 1552 + 79 * (head -> inode_num-1), 0);
			if((ftype = getc(fp)) == '0')
			{
				fseek(fp, 16+512+1024+79*512+128*8*(head -> inode_num-1), 0);
				head -> sib = dir_to_list(fp);
			}
			else head -> sib = NULL;
		}	

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
