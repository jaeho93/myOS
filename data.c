#include <stdio.h>

typedef struct Inode{
	char FileType;
	char FileDate[10];
	int Filesize;

	int Direct;
	int *P_Direct;
	int Single;
	int *P_Single;
	int Double;
	// int *P_Double;
}InodeBlock;

typedef struct linked_list{
	char DB[128];
	linked_list *next;
}linked_list;

linked_list string_to_list(char s[], int i){
	linked_list head = NULL;
	if( s[i] == '\0')
		return NULL;
	else{
		head = malloc(sizeof(linked_list));
		head -> d[i] = s[i];
		head -> next = string_to_list(s, i+1);
		return head;
	}
}

int main(void){
	/*InodeBlock Inode;
	  DataBlock Data[1024];

	  Inode.P_Direct = &(Inode.Single);
	  Inode.P_Single = &(Inode.Double);

	  Inode.Direct = Data[0].Content;
	  */
	linked_list *P; //포인터 형 설정????

	FILE *ifp;
	int pos = 0;
	
	int i=0;
	
	ifp = fopen("zito.txt","r");
	fseek(ifp,SEEK_SET,SEEK_END);
	pos = ftell(ifp);

	if(pos<=128)
		P = (linked_list *)calloc(1,sizeof(linked_list));

	else{
		if((pos%128) == 0)
			P = (linked_list *)calloc((pos/128), sizeof(linked_list));
		else
			P = (linked_list *)calloc((pos/128+1), sizeof(linked_list));

	}
	return 0;
}
