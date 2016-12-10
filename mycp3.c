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

	FILE *fp = fopen("myfs", "r+");

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

void merge(linked_list *P){
	// 아이노드를 쓴지 안 쓴지는 다른 함수에 구현 되어있음@@
	char c;
	int t=0;

	int pos_merge = 0;
	int pos_direct = 0;
	int pos_indirect = 0;
	int pos_D_indirect = 0;
	int pos_indirect_num = 0;
	int pos_D_indirect_num = 0;

	fseek(fp, 16+512+1024+(79* /* 새 파일의 아이노드*/)+32,SEEK_SET);
	pos_merge = binary_changer(fp, 17);  //pos_merge는 복사할 파일의 크기

	FILE *P_f1;
	char *F1;   // F1은 복사할 파일의 내용을 한 곳에 모을 포인터

	P_f1 = fopen(/*복사하고자하는파일명*/,"w");
	F1 = (char *)calloc(pos_merge,sizeof(char));

	if(pos_merge <= 128){
		fseek(fp, 16+512+1024+(79* /*새 파일의 아이노드번호*/)+49,SEEK_SET);
		pos_direct = binary_changer(fp, 10);  //pos_data = 아이노드 다이렉트의 값

		fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_direct-1)));   //pos_data 어디였는지 기억이 안남

		for(t=0; t < pos_merge; t++){
			c = getc(fp);
			F1[t] = c;
		}
	}

	else if((128<pos_merge)&&(pos_merge<=128*102)){

		fseek(fp, 16+512+1024+(79* /*새파일의 아이노드*/)+49,SEEK_SET);
		pos_direct = binary_changer(fp, 10);

		fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_direct-1)));

		for(t=0; t < 128; t++){
			c = getc(P->DB[t]);
			F1[t] = c;
		}

		next(P);


		fseek(fp, 16+512+1024+79* /*새파일의 아이노드*/+59,SEEK_SET);
		pos_indirect = binary_changer(fp, 10);

		fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_indirect-1)));

		for(t=1; t < (size-1); t++){
			pos_indirect_num = binary_changer(fp,10);
			fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_indirect_num-1)));
			for(int k=0; k<128; k++){
				c = getc(P->DB[k]);
				F1[128*t+k] = c;
			}
			fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_indirect-1)+10*t));
			next(P);
		}
	}

	else{		//next포인터가 NULL이면은 pos까지만 문자받음
		fseek(fp, 16+512+1024+(79* /*새파일의 아이노드*/)+49,SEEK_SET);
		pos_direct = binary_changer(fp, 10);

		fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_direct-1)));

		for(t=0; t < 128; t++){
			c = getc(P->DB[t]);
			F1[t] = c;
		}

		next(P);

		fseek(fp, 16+512+1024+79* /*새파일의 아이노드*/+59,SEEK_SET);
		pos_indirect = binary_changer(fp, 10);

		fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_indirect-1)));

		for(t=1; t<103; t++){
			pos_indirect_num = binary_changer(fp,10);
			fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_indirect_num-1)));
			for(int k=0; k<128; k++){
				c = getc(P->DB[k]);
				F1[128*t+k] = c;
			}
			fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_indirect-1)+10*t));
			next(P);
		}

		fseek(fp, 16+512+1024+(79* /*새파일의 아이노드*/)+69,SEEK_SET);
		pos_D_indirect = binary_changer(fp, 10);

		fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_D_indirect-1)));

		for(t=1; t<(size-1); t++){
			pos_D_indirect_num = binary_changer(fp,10);
			fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_indirect_num-1)));
			for(int k=0; k<7; k++){
				c = getc(P->DB[k]);
				F1[128*(103+t)+k] = c;
			} // P->next가 NULL이면 k<128말고 pos_merge%128까지 하게 하는 거 추가하기
			fseek(fp, 16+512+1024+(79*512)+(128*8*(pos_D_indirect-1)+10*t));
			next(P);
		}
	}

}
