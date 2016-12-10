#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mystate(FILE*fp); 

void init(FILE *fp)
{
	else if(strcmp("mystate",ptr[0])==0){
		mystate(fp);
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

