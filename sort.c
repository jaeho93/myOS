#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct inode
{
	int ftype;
	int time;
	int size;
	int direct;
	int single;
	int Double;
} inode;
typedef struct dir_link
{
	char name[5];
	int	 data_block;
	struct dir_link * next;
} dir_link;

char str[5] = {};
int inode_num = 4;
dir_link * dir_to_list(FILE *);
void binary(int time, char*str, int num);
int binary_changer(FILE* fp,int size);
void print_list(dir_link*);
void print_list_i(dir_link*);
void print_list_l(dir_link*, FILE*);
void print_inode_info(int num, FILE *fp);
void get_inode(FILE * fp, inode * ip, int num);

int main(void)
{
	time_t  now;

	char com1[11];			// myls에서만 되니까 수정이 필요하다.
	char com2[3];
	int now_dir = 1;
	FILE *fp;
	fp = fopen("test", "w+");



	fprintf(fp, "000");
	fprintf(fp, "0");				  // 아이노드리스트_파일종류 > 루트 디렉터리 0 : 디렉터리 1 : 일반파일
	now = time(NULL);
	char strTime[31];
	binary(now, strTime, 30);
	fprintf(fp, "%s", strTime);	  // 아이노드리스트_파일생성날짜 > 루트디렉터리 30개
	fprintf(fp, "00000000000000000");// 아이노드리스트_파일크기 > 최대로 가질수있는 크기 : 129,280바이트 1010 데이타블록 17개
	fprintf(fp, "0000000001");		  // 아이노드리스트_다이렉트블록 > 루트디렉터리의 데이터블록 주소
	fprintf(fp, "0000000000");		  // 아이노드리스트_싱글다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값 10개
	fprintf(fp, "0000000000");		  // 아이노드리스트_더블다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값

	
	fprintf(fp, "1");				  // 아이노드리스트_파일종류 > 루트 디렉터리 0 : 디렉터리 1 : 일반파일
	now = time(NULL);
	binary(now, strTime, 30);
	fprintf(fp, "%s", strTime);	  // 아이노드리스트_파일생성날짜 > 루트디렉터리 30개
	fprintf(fp, "00000000000000001");// 아이노드리스트_파일크기 > 최대로 가질수있는 크기 : 129,280바이트 1010 데이타블록 17개
	fprintf(fp, "0000000010");		  // 아이노드리스트_다이렉트블록 > 루트디렉터리의 데이터블록 주소
	fprintf(fp, "0000000000");		  // 아이노드리스트_싱글다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값 10개
	fprintf(fp, "0000000000");		  // 아이노드리스트_더블다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값

	fprintf(fp, "1");				  // 아이노드리스트_파일종류 > 루트 디렉터리 0 : 디렉터리 1 : 일반파일
	now = time(NULL);
	binary(now, strTime, 30);
	fprintf(fp, "%s", strTime);	  // 아이노드리스트_파일생성날짜 > 루트디렉터리 30개
	fprintf(fp, "00000000000000010");// 아이노드리스트_파일크기 > 최대로 가질수있는 크기 : 129,280바이트 1010 데이타블록 17개
	fprintf(fp, "0000000011");		  // 아이노드리스트_다이렉트블록 > 루트디렉터리의 데이터블록 주소
	fprintf(fp, "0000000000");		  // 아이노드리스트_싱글다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값 10개
	fprintf(fp, "0000000000");		  // 아이노드리스트_더블다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값

	fprintf(fp, "0");				  // 아이노드리스트_파일종류 > 루트 디렉터리 0 : 디렉터리 1 : 일반파일
	now = time(NULL);
	binary(now, strTime, 30);
	fprintf(fp, "%s", strTime);	  // 아이노드리스트_파일생성날짜 > 루트디렉터리 30개
	fprintf(fp, "00000000000000000");// 아이노드리스트_파일크기 > 최대로 가질수있는 크기 : 129,280바이트 1010 데이타블록 17개
	fprintf(fp, "0000000100");		  // 아이노드리스트_다이렉트블록 > 루트디렉터리의 데이터블록 주소
	fprintf(fp, "0000000000");		  // 아이노드리스트_싱글다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값 10개
	fprintf(fp, "0000000000");		  // 아이노드리스트_더블다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값


	fprintf(fp, "00101110");									/// .           << 디렉터리 내용은 이런모양을 구현
	fprintf(fp, "00000000");		// 한바이트                 /// 1
	fprintf(fp, "00000000");		// 한바이트			        /// ..
	fprintf(fp, "00000000");		// 한바이트			    	/// 1		  루트디렉터리의 ..은 자기 자신을 가리킨다.
	fprintf(fp, "0000000001");									/// a
																/// 2			
	fprintf(fp, "00101110");									/// c					
	fprintf(fp, "00101110");									/// 3
	fprintf(fp, "00000000");		// 한바이트					/// b
	fprintf(fp, "00000000");		// 한바이트					/// 4
	fprintf(fp, "0000000001");
	
	fprintf(fp, "01100001");									/// c					
	fprintf(fp, "00000000");									/// 3
	fprintf(fp, "00000000");		// 한바이트					/// b
	fprintf(fp, "00000000");		// 한바이트					/// 4
	fprintf(fp, "0000000010");

	fprintf(fp, "01100011");									/// c					
	fprintf(fp, "00000000");									/// 3
	fprintf(fp, "00000000");		// 한바이트					/// b
	fprintf(fp, "00000000");		// 한바이트					/// 4
	fprintf(fp, "0000000011");

	fprintf(fp, "01100010");									/// c					
	fprintf(fp, "00000000");									/// 3
	fprintf(fp, "00000000");		// 한바이트					/// b
	fprintf(fp, "00000000");		// 한바이트					/// 4
	fprintf(fp, "0000000100");
	for(int i = 0; i < 101; i++)
	{
		fprintf(fp, "00000000");
	}

dir_link * h = NULL;
fseek(fp, 3+78*inode_num+128*8*(now_dir-1), 0);
h = dir_to_list(fp);

print_list(h);


print_list_i(h);


print_list_l(h, fp);





fclose(fp);
return 0;
}
void bin_to_str(FILE * fp)
{

	for(int i = 0; i < 4; i++)
	{
		for(int j = 128; j >= 1; j/=2)
		{
			str[i] += (getc(fp) - '0') * j;
		}
	}
}

dir_link * dir_to_list(FILE *fp)
{
	dir_link * head = NULL;
	bin_to_str(fp);
	if (str[0] == '\0')
		{
		return NULL;
		}
	else
	{
		head = malloc(sizeof(dir_link));
		strcpy(head -> name, str);
		for(int k = 0; k < 5; k++)
			str[k] = 0;
		head -> data_block = binary_changer(fp, 10);
		head -> next = dir_to_list(fp);
		return head;
	}
}
void binary(int time, char*str, int num)
{
	for(int i = 0; i < num-1; i++)
	{
		str[i] = 0;
	}

	for(int i = num-1; i >= 0; i--)
	{
		str[i] = time % 2 + '0';
		time /= 2;
	}
	return;
}
int binary_changer(FILE* fp,int size)
{
	char * cp;
	int num = 0, cnt = 1;
	cp = (char *)malloc(sizeof(size));

	for(int i = 0; i < size; i++)
	{
		cp[i] = getc(fp);
	}

	for(int i = size-1; i >= 0; i--)
	{
		num += (cp[i] - '0') * cnt;
		cnt *= 2;
	}
	
	free(cp);
	return num;
}

void print_list(dir_link * head)
	{
		if (head == NULL)
		{
			return;
		}
		else
		{
			printf("%s\n", head -> name);
			print_list(head -> next);
		}
	}

void print_list_i(dir_link* head)
	{
		if (head == NULL)
		{
			return;
		}
		else
		{
			printf("%d ", head -> data_block); 
			printf("%s\n", head -> name);
			getchar();
			print_list_i(head -> next);
		}
	}

void print_list_l(dir_link* head, FILE * fp)
	{
		if (head == NULL)
		{
			return;
		}
		else
		{
			print_inode_info(head -> data_block, fp);
			printf("%s\n", head -> name);
			getchar();
			print_list_l(head -> next, fp);
		}
	}
void print_inode_info(int num, FILE *fp)
{
	inode * ip = (inode *)malloc(sizeof(inode));
	get_inode(fp, ip, 3 + 78 * (num - 1));
	printf("%c ", (ip -> ftype) ? '-' : 'd');
	printf("%17d ", ip -> size);
	printf("%d ", ip -> time);
	free(ip);
}
		

void get_inode(FILE * fp, inode * ip, int num)
{
	fseek(fp, num, 0); // 원래는 1552+78*num

	ip -> ftype 	= binary_changer(fp, 1);
	ip -> time		= binary_changer(fp, 30);
	ip -> size		= binary_changer(fp, 17);
	ip -> direct	= binary_changer(fp, 10);
	ip -> single	= binary_changer(fp, 10);
	ip -> Double	= binary_changer(fp, 10);
	return;
}


