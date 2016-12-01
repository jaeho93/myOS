#include <stdio.h>
#include <stdlib.h>

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

int pos = 0;

typedef struct linked_list{
	char DB[128];
	struct linked_list *next;
}linked_list;

linked_list* string_to_list(FILE * ifp, linked_list *P){
	char c = '\0';

	for(int t=0; t<128; t++){
		c = getc(ifp);
		if( c == EOF)
			return P;
		P -> DB[t] = c;
	}
	if(pos>128)
		P -> next = string_to_list(ifp, P+1);
	return P;
}

void printf_list(linked_list *P){
	char c = '\0';
	int t=0;

	while( (P-> next) != NULL ){
		printf("%c", P->DB[t]);
		t++;
		if(t ==128){  printf_list(P -> next);}
	}
	if(P -> next == NULL){
		printf("Rmx");
		return ;}
	//if ( P->DB[t] == '\0')
	//printf("NULL a");
	//		printf_list(P-> next);
	return ;
}
//	else(pos>128)
//	printf_list(P->next);

int main(void){
	/*InodeBlock Inode;
	  DataBlock Data[1024];

	  Inode.P_Direct = &(Inode.Single);
	  Inode.Direct = Data[0].Content;
	  */
	linked_list *P = NULL; //포인터 형 설정????
	linked_list *H = NULL;

	FILE *ifp = NULL;

	ifp = fopen("zito.txt","r");
	fseek(ifp,SEEK_SET,SEEK_END);
	pos = ftell(ifp);
	fseek(ifp,SEEK_SET,SEEK_SET);
	printf("%d",pos);

	if(pos<=128)
		P = (linked_list *)calloc(1,sizeof(linked_list));

	else{
		if((pos%128) == 0)
			P = (linked_list *)calloc((pos/128), sizeof(linked_list));
		else
			P = (linked_list *)calloc((pos/128)+1, sizeof(linked_list));
	}
	H = string_to_list(ifp, P);
	printf_list(H);

	return 0;
}
