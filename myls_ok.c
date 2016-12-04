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
typedef ELEMENT *LINK;

int nowdir_inode = 1; // 루트 디렉터리의 경우!!!!!!!!!!!
char fname[5];
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

int main(void)
{
	FILE *fp = fopen("myfs", "r+");

	LINK h = NULL;

	fseek(fp, 16+512+1024+79*512+128*8*(nowdir_inode-1), 0);
	h = dir_to_list(fp);

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
	printf("%d/",t -> tm_mon + 1);
	printf("%d ",t -> tm_mday);
	printf("%d:",t -> tm_hour);
	printf("%d:",t -> tm_min);
	printf("%d ",t -> tm_sec);
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
