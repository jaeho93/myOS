#include<stdio.h>
#include<stdlib.h>

typedef linked_list* LINK;

int pos = 0; // 파일 크기
int size = 0; // 동적할당 된 개수

typedef struct LINK{ 		//변함없음
	char DB[128];
	struct linked list *next;
}LINK;

LINK next(LINK P){		// P->next를 P의 다음 동적메모리할당 받은 공간으로 포인트

	for(int i=0; i<(size-1); i++){
		(P+i)->next = P+i+1;
	}

	return P;
}

LINK string_to_list(FILE *ifp, LINK P){
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

void printf_list(LINK P){
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

void original_main(void){ /// 1~103번째 줄까지의 main이였음
	LINK P = NULL;
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

	return 0;
}

void u_sb_data(FILE *fp, int cnt){
	fseek(fp, 527+cnt,0);
	putc('1',fp);
	fflush(fp);
}

int f_sh_data_empty(FILE *fp){

	int cnt = 0;
	char c;

	fseek(fp,528,0); 

	for(int i = 0 ; i<1024 ; i++){
		if( (c = getc(fp)) == '0'){
			cnt = i+1;
			break;
		}
	}

	return cnt;
}

void write_content_to_data(int num, LINK P, FILE *fp){
	char new_name[128];
	strcpy(new_name, P -> DB);
	fseek(fp, 16+512+1024+79*512+128*8*(num-1),0);  //num에는 다이렉트, 인다이렉트 번호를 담고있음
	for(int i=0; i<128; i++){
		char tmp_str[9] = {};
		for(int k=0; i<8; k++){
			tmp_str[k] = new_name[i] / j + '0';
			new_name[i] %= j;
			j /= 2;
		}
		fprintf(fp, "%s", tmp_str);
	}
}

void binary(int time, char *str, int num){
	for(int i = num-1;i>=0;i--){
		str[i] = time %2 + '0';
		time /= 2;
	}
	return;
}

void set_address(int size ,int inode_num,  FILE *fp  ){
	int checker=0;
	int direct_num;
	int single_num;
	int double_num;
	int in_single;
	int in_double;
	int in_double_in;
	int in;
	char str[11];
	int count = 0;
	// 전체 필요한 데이터 블록 개수  딱 개수만

	if(size==1){
		direct_num = f_sh_data_empty(fp);  		
		u_sb_data(fp, direct_num);
		printf("%d\n",direct_num);
		fseek(fp, 16+512+1024+79*(inode_num-1)+49 ,0);
		binary(direct_num,str,10);
		fprintf(fp,"%s",str);
		write_content_to_data(direct_num, P, fp);  // 나중에 동적할당된 포인터 P 적어주기

	}

	else if((size >1) && (size <=103)){

		direct_num = f_sh_data_empty(fp);
		u_sb_data(fp,direct_num);  
		printf("%d\n",direct_num);
		fseek(fp,16+512+1024+79*(inode_num-1)+49,0)	;
		binary(direct_num,str,10);	
		fprintf(fp,"%s",str);		

		write_content_to_data(direct num, P, fp);

		single_num = f_sh_data_empty(fp);
		u_sb_data(fp,single_num);
		printf("%d\n",single_num);
		fseek(fp,16+512+1024+79*(inode_num-1)+59,0);
		binary(single_num,str,10);
		fprintf(fp,"%s",str);

		for(int k=0; k< size-1 ;k++){
			in_single = f_sh_data_empty(fp);
			u_sb_data(fp,in_single);
			printf("%d\n",in_single);
			fseek(fp,16+512+1024+79*512+ 128*8*(single_num-1) + 10*k ,0);

			fprintf(fp,"%s",str);

			write_content_to_data(in_single, P, fp);
		}

	}

	else {

		direct_num = f_sh_data_empty(fp);  
		u_sb_data(fp,direct_num);
		printf("%d direct\n",direct_num);
		fseek(fp,16+512+1024+79*(inode_num-1)+49,0);
		fprintf(fp,"%s",str);

		write_content_to_data(direct_num, P, fp);

		single_num = f_sh_data_empty(fp); //inode : single 부분에 저장
		u_sb_data(fp,single_num);
		printf("%d single \n",single_num);
		fseek(fp,16+512+1024+79*(inode_num-1)+59,0);
		fprintf(fp,"%s",str);

		double_num = f_sh_data_empty(fp);  //inode : double부분에 저장 
		printf("%d double\n",double_num);
		u_sb_data(fp,double_num);
		fseek(fp,16+512+1024+79*(inode_num-1)+69,0);
		fprintf(fp,"%s",str);


		for(int k = 0; k<102 ;k++){  //single 데이터 블록에 넣는 숫자들

			in_single = f_sh_data_empty(fp);
			u_sb_data(fp,in_single);
			printf("%d in_single\n",in_single);
			fseek(fp,16+512+1024+ 79*512 + 128*8*(single_num-1) + 10*k,0  );
			fprintf(fp,"%s",str);

			write_content_to_data(in_single, P, fp);
		}

		for(int i=0; i< 102 ;i++){
			in_double_in = f_sh_data_empty(fp);
			u_sb_data(fp,in_double_in);
			printf("%d in double\n",in_double_in);
			fseek(fp,16+512+1024+ 79*512+ 128*8*(in_double-1)+10*i,0);
			fprintf(fp,"%s",str);


			for(int j =0 ; j<102 ;j++){

				in = f_sh_data_empty(fp);
				u_sb_data(fp,in);
				printf("%d in",in);
				fseek(fp,16+512+1024+79*512+ 128*8*(in_double_in-1)+10*j,0);
				fprintf(fp,"%s",str);
				count++;

				write_content_to_data(in, P, fp);

				if(count == size-103)
					break;
			}
			if(count == size-103)
				break;
		}
	}
}
