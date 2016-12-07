#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

typedef struct linked_list_prompt
{
	char name[5];
	struct linked_list_prompt * next;
} ELEMENT_prompt;

typedef ELEMENT_prompt *LINK2;



typedef ELEMENT *LINK;

int nowdir_inode = 1; // 루트 디렉터리의 경우!!!!!!!!!!!
LINK now_h = NULL;
char now_name[5] = "/";
char fname[5];
int root = 0;
char pstr[1000] = {};
int SWITCH = 0;


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
char * find_my_name(LINK head, int myinode);
int find_my_inode(LINK head, char * my_name);

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
void mycd(LINK head, FILE *fp, char * gostr);
void prompt(FILE *);
void mypwd(FILE *fp);

int main(void)
{
	FILE *fp = fopen("myfs", "r+");

	LINK h = NULL;

	fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1), 0);
	h = dir_to_list(fp);
	now_h = h;

	mymkdir(now_h, fp);
	mycd(now_h, fp, "test");
	prompt(fp);
	mypwd(fp);
	//mypwd(now_h, fp);
	



	putchar('\n');
	putchar('\n');
	printf("ls\n");
	print_list(now_h);
	printf("\nls -i\n");
	print_list_i(now_h);
	printf("\nls -l\n");
	print_list_l(now_h, fp);

	link_free(h);
	fclose(fp);
	return 0;
}
void mycd(LINK head, FILE *fp, char * gostr)
{
	LINK tmp = head;
	while(1)
	{
		if(strcmp(tmp -> name, gostr) == 0)
		{
			now_h = tmp -> sib;
			nowdir_inode = tmp -> inode_num;
			strcpy(now_name, tmp -> name);
			return;
		}
		if(tmp -> next != NULL)
			tmp = tmp -> next;
		else {printf("mycd 실패\n"); return;}
	}
}

void prompt(FILE *fp)
{
	if(nowdir_inode == 1)
	{
		printf("[ / ]$"); return;
	}
	char *pp[100]; int cnt = 1; pp[0] = now_name;
	int mother_inode = now_h -> next -> inode_num;
	fseek(fp, 16+512+1024+79*512+128*8*(mother_inode-1), 0);
	LINK mother_list = dir_to_list(fp);
	if(mother_inode != 1)
	{
		cnt++;
		pp[1] = now_h -> next -> name;
	}
	else {printf("[ /%s ]$ ", now_name); return;}
	for(; cnt <= 100; cnt++)
	{
		mother_inode = mother_list -> next -> inode_num;
		if(mother_inode != 1)
		{
			pp[cnt] = mother_list -> next -> name;
			cnt++;
			fseek(fp, 16+512+1024+79*512+128*8*(mother_inode-1), 0);
			mother_list = dir_to_list(fp);
			
		}
		else {pp[cnt] = "/"; break;}
	}
	printf("[ /");
	for(int i = cnt; cnt >= 0; cnt--)
	{
		printf("%s", pp[i]);
		putchar('/');
	}
	printf(" ]$");
}

void mypwd(FILE *fp)
{
	if(nowdir_inode == 1)
	{
		printf("/\n"); return;
	}
	char *pp[100]; int cnt = 1; pp[0] = now_name;
	int mother_inode = now_h -> next -> inode_num;
	fseek(fp, 16+512+1024+79*512+128*8*(mother_inode-1), 0);
	LINK mother_list = dir_to_list(fp);
	if(mother_inode != 1)
	{
		cnt++;
		pp[1] = now_h -> next -> name;
	}
	else {printf("/%s\n", now_name); return;}
	for(; cnt <= 100; cnt++)
	{
		mother_inode = mother_list -> next -> inode_num;
		if(mother_inode != 1)
		{
			pp[cnt] = mother_list -> next -> name;
			cnt++;
			fseek(fp, 16+512+1024+79*512+128*8*(mother_inode-1), 0);
			mother_list = dir_to_list(fp);
			
		}
		else {pp[cnt] = "/"; break;}
	}
	printf("/");
	for(int i = cnt; cnt >= 0; cnt--)
	{
		printf("%s", pp[i]);
		putchar('/');
	}
	printf("\n");
}




void mymkdir(LINK head, FILE *fp)
{
	int sb_inode_empty = f_sb_inode_empty(fp);
	int sb_data_empty  = f_sb_data_empty(fp);
	u_sb_inode(fp, sb_inode_empty);
	u_sb_data(fp, sb_data_empty);
	write_to_inode_dir(fp, sb_inode_empty, sb_data_empty);
	write_to_data_dir(fp, sb_inode_empty, sb_data_empty);
	char newdir_str[5]= "test";
	write_name_to_dir(fp, head, sb_inode_empty, newdir_str);
	link_newdir_to_list(fp, head, sb_inode_empty, newdir_str);
	SWITCH = 0;
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

	
	


/*int find_my_inode(LINK head, char * my_name)
{
	int my_inode = NULL;
	if(strcmp(head -> name, my_name) == '0')
		return head -> inode_num;
	else
	{
		my_inode = find_my_inode(head -> next, my_name);
		if(my_inode != NULL)
			return my_inode;
		else
			return NULL;
	}
}*/
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
