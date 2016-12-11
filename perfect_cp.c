#include <stdio.h>
#include <stdlib.h>

int pos = 0;
int size = 0;

typedef struct linked_list{ 		//변함없음
	char DB[128];
	struct linked_list *next;
}linked_list;

linked_list* next(linked_list *P){		// P->next를 P의 다음 동적메모리할당 받은 공간으로 포인트

	for(int i=0; i<(size-1); i++){
		(P+i)->next = P+i+1;
	}

	return P;
}

linked_list* string_to_list(FILE *ifp, linked_list *P){
	char c;
	int t=0;

	if(pos<=128){						// 메모리공간 1 할당받았을 때
		for(; t<pos; t++){
			c = getc(ifp);
			P->DB[t] = c;
		}
	}

	else{
		if(P->next != NULL){			// next포인터가 NULL이 아닐 때 -> 0부터 127까지 다 문자받음
			for(t=0; t<128; t++){
				c = getc(ifp);
				P->DB[t] = c;
			}
		string_to_list(ifp, P->next); 
		}
		else								//next포인터가 NULL이면은 pos까지만 문자받음
			for(t=0; t<(pos-(128*(size-1))); t++){
				c = getc(ifp);
				P->DB[t] = c;
			}
	}

	return P;
}

void printf_list(linked_list *P){
	char c;
	int t=0;

	if(pos<=128){
		for(; t < pos; t++)
			printf("%c",P->DB[t]);
	}

	else{
		if(P->next != NULL){
			for(t=0; t<128; t++)
				printf("%c",P->DB[t]);
			printf_list(P->next);
		}

		else
			for(t=0; t<(pos-(128*(size-1))); t++)
				printf("%c",P->DB[t]);
	}

	return ;
}

int main(void){
	linked_list *P = NULL;
	FILE *ifp = NULL;

	ifp = fopen("zito.txt", "r");
	fseek(ifp,SEEK_SET,SEEK_END);
	pos = ftell(ifp);
	fseek(ifp,SEEK_SET,SEEK_SET);
	printf("%d",pos); //NULL문자 포함한 txt의 길이 알려줌

	if(pos<=128){
		P = (linked_list *)calloc(1,sizeof(linked_list));
		size = 1;
	}

	else{
		if((pos%128)==0){
			P = (linked_list *)calloc((pos/128), sizeof(linked_list));
			size = (pos/128);
		}

		else{
			P = (linked_list *)calloc((pos/128)+1, sizeof(linked_list));
			size = (pos/128)+1;
		}
	}

	P = next(P);
	P = string_to_list(ifp, P);
	printf_list(P);

	return 0;
}

