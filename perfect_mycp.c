#include<stdio.h>
#include<stdlib.h>

typedef linked_list* LINK;

int pos = 0; // 파일 크기
int size = 0; // 동적할당 된 개수

void mycp(void){

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
	fopen(File2,"w");

	if(size == 1){
		size_1_cp();
	}

	else if((size>1)&&(size<=103)){
		size_1_cp();
		size_2_cp(); // 싱글인다이렉트부분만 적혀있음!!
	}

	else{
		size_1_cp();
		size_2_cp();
		size_104_cp(); // 더블인다이렉트부분만 적혀있음!!
	}
}

void size_1_cp(){
	fseek(fp, 16+512+1024+79*(inode_num-1)+49,0);
	direct_num = binary_changer(fp,10);
	fseek(fp, 16+512+1024+79*512+(128*8*(direct_num-1)));
	for(int i=0; i < (pos%128); i++){
		for(int t=128; t>=1; k/=2)
			c += (getc(fp)-'0')*t;
		fprintf(ifp,"%c",c);
	}
}

void size_2_cp(){

	fseek(fp, 16+512+1024+79*(inode_num-1)+59,0);
	single_num = binary_changer(fp,10);				// single의 데이터블록 번호를 알아내겠다.

	for(int i=0; i < (size-1); i++){
		fseek(fp, 16+512+1024+79*512+(128*8*(single_num-1)+10*i));
		insingle = binary_changer(fp, 10);		// insingle의 데이터블록 번호를 알아내겠다.

		fseek(fp, 16+512+1024+79*512+128*8*(insingle-1)); // insingle 그 데이터블록으로 가서 이제부터 데이터내용을 적을거임

		if( i == (size-2) )						// i는 size-1이 되기 전까지 돌기 때문에 즉, size-2까지 도는 거임. 근데 size-2(내용이 써질 마지막 데이터블록)일 때, 데이터내용이 128바이트로 가득 안차는 경우가 있음. 그 때를 대비하여 if를 나눠놓은 거임.
			for(int k=0; k < (pos%128); k++){	
				for(int t=128; t>=1; k/=2)			// bin_to_str함수를 풀어적은 거임. 8비트를 char형 1바이트로 다시 되돌리는 과정
					c += (getc(fp)-'0')*t;
				fprintf(ifp, "%c", c);			//이제 char형으로 되돌렸으니 file2파일에 옮겨적겠다.
			}
		else
			for(int k=0; k<128; k++){
				for(int t=128; t>=1; k/=2)
					c += (getc(fp)-'0')*t;
				fprintf(ifp, "%c", c);
			}
	}
}

void size_104_cp(){

	fseek(fp, 16+512+1024+79*(inode_num-1)+69,0);
	double_num = binary_changer(fp,10);		// double 데이터블록 번호를 알아내겠다.

	for(int i=0; i < ((size-103)/102+1); i++){		
		fseek(fp, 16+512+1024+79*512+(128*8*(double_num-1)+10*i));
		in_double_in = binary_changer(fp, 10);		// indoublein 데이터블록 번호를 알아내겠다.

		if(i == (size-103)/128){				// 앞과 마찬가지로, 마지막 데이터블록일 때를 대비하여 나눠놓은 것.

			for(int j=0; j < (size-103)%102; j++){ 		// 이 밑에 나오는 if는 indoublein이 가득 안찼을 때(102개를 못 채웠을 때를 대비하여 나눠놓은 것!
				fseek(fp, 16+512+1024+79*512+128*8*(insingle-1)+10*j);

				if( j == ((size-103)%102 - 1) )
					for(int k=0; k < (pos%128); k++){	// 얘는 진짜 완전 마지막블록일 때, 128바이트가 안 차 있을 수도 있으니까 그 때 대비한 것임.
						for(int t=128; t>=1; k/=2)		// 이 for문은 앞과 마찬가지로 2진수 8비트를 char형 1바이트로 바꾸는 과정
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
				fseek(fp, 16+512+1024+79*512+128*8*(insingle-1)+10*j);

				if( j == ((size-103)%102 - 1) )
					for(int k=0; k < (pos%128); k++){
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
