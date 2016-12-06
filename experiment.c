#include <stdio.h>
#include <stdlib.h>

typedef struct inode{
	int ftype;
	int itime;
	int size;

	int Direct;
	int *P_Direct;
	int Single;
	int *P_Single;
	int Double;
}inode;

int main(void){
	char Express[23]={}, Command[10]={}, File1[10]={}, File2[10] = {}, Sign;

	scanf("%[^\n]s", Express);

	if(sscanf(Express,"%s %s %c %s", Command, File1, Sign, File2)==1)
		printf("%s %s %c %s", Command, File1, Sign, File2);

	if(sscanf(Express,"%s %s %c %s", Command, File1, Sign, File2)==2)
		printf("%s %s %c %s", Command, File1, Sign, File2);


	return 0;
}
