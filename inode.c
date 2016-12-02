#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define b_c binary_changer

typedef struct inode
{
	int ftype;
	int time;
	int size;
	int direct;
	int single;
	int Double;
} inode;

void	get_inode(FILE *, inode *, int); // 슈퍼블록으로부터 순번을 받아 아이노드리스트에서 그 순번에 해당하는 정보를 구조체 포인터로 할당한다.
int 	binary_changer(FILE*, int); //현재 파일지시자가 위치한곳에서 부터 size만큼 수를 읽어 int형 정수로 변환한다. 파일지시자의 위치도 변한다.

void binary(int, char*, int);

int main(void)
{
	FILE * fp;
	fp = fopen("test", "w+");
	inode * ip = (inode *)malloc(sizeof(inode));
	time_t now;
	now = time(NULL);
	char strTime[31];
	
	binary(now, strTime, 30); 

	fprintf(fp, "000");
	fprintf(fp, "1");
	fprintf(fp, "%s", strTime);
	fprintf(fp, "00000000000000011");
	fprintf(fp, "0000000011");
	fprintf(fp, "0000000100");
	fprintf(fp, "0000000101");

	fseek(fp, 3, 0);

	get_inode(fp, ip, 3);
	printf("파일의 종류 	: 	%30d\n", ip -> ftype);
	printf("파일의 시간 	: 	%30d\n", ip -> time);
	printf("파일의 크기 	: 	%30d\n", ip -> size);
	printf("파일의 다이렉트 : 	%30d\n", ip -> direct);
	printf("파일의 싱글 	: 	%30d\n", ip -> single);
	printf("파일의 더블 	: 	%30d\n", ip -> Double);

	free(ip);
	fclose(fp);
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
	for(int i = num-1; i >= 0; i--)
	{
		str[i] = time % 2 + '0';
		time /= 2;
	}
	return;
}
