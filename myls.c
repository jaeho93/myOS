#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct inode
{
	int ftype;
	int time;
	int size;
	int direct;
	int single;
	int Double;
} inode;
void get_inode(FILE * fp, inode * ip, int num);
int binary_changer(FILE* fp,int size);
void binary(int time, char*str, int num);
void myls(int mydir, FILE * fp, char * com2); // mydir은 현재 내 디렉터리의 아이노드순번

int main(void)
{
	FILE *fp;
	time_t  now;

	char com1[11];			// myls에서만 되니까 수정이 필요하다.
	char com2[3];
	int mydir = 1;

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
	fprintf(fp, "000000");
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	printf("[ / ]$ ");
	scanf("%4s", com1);
	scanf("%2s", com2);

	if(strcmp(com1, "myls") == 0)
	{
		myls(now_dir, fp, com2);
	}
	else printf("\n다시 해보자!");

	return 0;
}
void get_inode(FILE * fp, inode * ip, int num)
	return 0;
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

void myls(int mydir, FILE *fp, char *com2)
{
	if(strcmp(com2, "-i") == 0)
	{
		myls_i(mydir, FILE *fp);
	}
	else if(strcmp(com2, "-l") == 0)
	{
		myls_l(mydir, FILE *fp);
	}
	else
	{
		inode * ip = (inode *)malloc(sizeof(inode));
		get_inode(fp, ip, 3);
		ls_




