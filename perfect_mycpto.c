#include<stdio.h>
#include<stdlib.h>

typedef linked_list* LINK;

int pos = 0; // 파일 크기
int size = 0; // 동적할당 된 개수

void mycpto(void){

	FILE *fp;
	fopen("myfs","r");

	char Express[23] ={}, Command[10]={}, File1[10]={}, File2[10]={};

	scanf("%s", Express);

	if(sscanf(Express,"%s %s %s", Command, File1 File2)==3)
		printf("%s %s %s", Command, File1, File2);

	//File1의 이름의 아이노드 번호 찾기 -> 그 번호를 inode_num1 이라 하겠음
	fseek(fp, 16+512+1024+79*(inode_num1-1)+32,SEEK_SET);
	pos = binary_changer(fp, 17);

	if(pos<=128)
		size = 1;

	else{
		if((pos%128)==0)
			size = pos/128;
		else
			size = pos/128 + 1;
	}

	FILE *ifp = NULL;
	ifp = fopen(File2,"w");

	if(size == 1){
		size_1_cpto();
	}

	else if((size>1)&&(size<=103)){
		size_1_cpto();
		size_2_cpto();
	}

	else{
		size_1_cpto();
		size_2_cpto();
		size_104_cpto();
	}
}

void size_1_cpto(){
	fseek(fp, 16+512+1024+79*(inode_num-1)+49,0);
	direct_num = binary(direct_num,str,10);		//direct번호 찾아 냈음
	
	fseek(fp, 16+512+1024+79*512+(128*8*(direct_num-1)));

	for(int i=0; i < ((pos%128==0)? (128) : (pos%128)); i++){		// 내용쓰기
		for(int t=128; t>=1; k/=2)
			c += (getc(fp)-'0')*t;				//2진으로 되있는 8비트를 char형으로 바꾸기
		fprintf(ifp,"%c",c);					// ifp에 써주기
	}
}

void size_2_cpto(){

	fseek(fp, 16+512+1024+79*(inode_num-1)+59,0);
	single_num = binary(single_num,str,10);		//single번호 알아냈음.

	for(int i=0; i < (size-1); i++){		// 데이터블록수만큼 for문 돌릴거임
		fseek(fp, 16+512+1024+79*512+(128*8*(single_num-1)+10*i));
		insingle = binary(insingle, str, 10);
		
		fseek(fp, 16+512+1024+79*512+128*8*(insingle-1));

		if( i == (size-2) )			// 마지막 데이터 블록일 때
			for(int k=0; k < ((pos%128==0)? (128):(pos%128)); k++){
				for(int t=128; t>=1; k/=2)
					c += (getc(fp)-'0')*t;
				fprintf(ifp, "%c", c);
			}

		else
			for(int k=0; k<128; k++){
				for(int t=128; t>=1; k/=2)
					c += (getc(fp)-'0')*t;
				fprintf(ifp, "%c", c);
			}
	}
}

void size_104_cpto(){

	fseek(fp, 16+512+1024+79*(inode_num-1)+69,0);
	double_num = binary(double_num,str,10);

	for(int i=0; i < ((size-103)/102+1); i++){
		fseek(fp, 16+512+1024+79*512+(128*8*(double_num-1)+10*i));
		indoublein = binary(indoublein, str, 10);

		if(i == (size-103)/128){

			for(int j=0; j < (((size-103)%102==0)? (102) :((size-103)%102)); j++){ 
				fseek(fp, 16+512+1024+79*512+128*8*(indoublein-1)+10*j);

				if( j == ((size-103)%102 - 1) )
					for(int k=0; k < ((pos%128==0)? (128) : (pos%128)); k++){
						for(int t=128; t>=1; k/=2)
							c += (getc(fp)-'0')*t;
						fprintf(ifp, "%c", c);
					}

				else
					for(int k=0; k<128; k++){
						for(int t=128; t>=1; k/=2)
							c += (getc(fp)-'0')*t;
						fprintf(ifp, "%c", c);
					}
			}
		}

		else{
			for(int j=0; j < 102; j++){ 
				fseek(fp, 16+512+1024+79*512+128*8*(indoublein-1)+10*j);

				if( j == ((size-103)%102 - 1) )
					for(int k=0; k < ((pos%128==0)? (128) : (pos%128)); k++){
						for(int t=128; t>=1; k/=2)
							c += (getc(fp)-'0')*t;
						fprintf(ifp, "%c", c);
					}

				else
					for(int k=0; k<128; k++){
						for(int t=128; t>=1; k/=2)
							c += (getc(fp)-'0')*t;
						fprintf(ifp, "%c", c);
					}
			}
		}

	}
}
