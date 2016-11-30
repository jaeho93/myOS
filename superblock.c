#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <stdbool.h>

#define TRUE  1
#define FALSE 0
#define gsi(x) fseek(fp, 17+(x), 0)  	// go superblock inode


int 	f_sb_inode_empty(FILE *);		// 비어있는 superblock_inode 찾아서 숫자 리턴; 
void 	u_sb_inode(FILE *, int);		// superblock_inode 사용
void    r_sb_inode(FILE *, int);		// superblock_inode 제거
void	p_sb_inode_used(FILE *);		// 사용중인 superblock_inode 출력;


int 	f_sb_data_empty(FILE *);		// 비어있는 superblock_data 찾아서 숫자 리턴; 
void 	u_sb_data(FILE *, int);		// superblock_data 사용
void    r_sb_data(FILE *, int);		// superblock_data 제거
void	p_sb_data_used(FILE *);		// 사용중인 superblock_data 출력;

int main(void)
{
	FILE *fp;
	fp = fopen("myfs", "r+");
	int sb_inode_empty;

	
	r_sb_inode(fp, 2);
	sb_inode_empty = f_sb_inode_empty(fp);
	printf("%d", sb_inode_empty);

	fclose(fp);
	return 0;
}

int f_sb_inode_empty(FILE *fp)
{
	int cnt;

	fseek(fp, 16, 0);
	
	for(int i = 0; i < 512; i++)
	{
		if(getc(fp) == '0')
		{
			cnt = i;
			break;
		}
	}
	return cnt;
}

void u_sb_inode(FILE *fp, int cnt)
{
fseek(fp, 16+cnt, 0);  	// go superblock inode
	putc('1', fp);
	fflush(fp);
}

void r_sb_inode(FILE *fp, int cnt)
{
fseek(fp, 16+cnt, 0);  	// go superblock inode
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
			printf("%d  ", ftell(fp) - 17);
		}
	}
}

int 	f_sb_data_empty(FILE *);		// 비어있는 superblock_data 찾아서 숫자 리턴; 
{
	int cnt;

	fseek(fp, 528, 0);
	
	for(int i = 0; i < 1024; i++)
	{
		if(getc(fp) == '0')
		{
			cnt = i;
			break;
		}
	}
	return cnt;
}
void 	u_sb_data(FILE *, int);		// superblock_data 사용
{
fseek(fp, 528+cnt, 0);  	// go superblock inode
	putc('1', fp);
	fflush(fp);
}
void    r_sb_data(FILE *, int);		// superblock_data 제거
{
fseek(fp, 528+cnt, 0);  	// go superblock inode
	putc('0', fp);
	fflush(fp);
}
void	p_sb_data_used(FILE *);		// 사용중인 superblock_data 출력;
{
	//gsi(0);
	fseek(fp, 528, 0);
	
	printf("사용중인 sb_inode 번호 : ");
	for(int i = 0; i < 1024; i++)
	{
		if(getc(fp) == '1')
		{
			printf("%d  ", ftell(fp) - 17);
		}
	}
}
