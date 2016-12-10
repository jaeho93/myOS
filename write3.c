#include<stdio.h>
#include<stdlib.h>
void binary(int time, char *str,int num);
int f_sh_data_empty(FILE *fp);
void u_Sb_data(FILE * ,int );


void u_sb_data(FILE *fp, int cnt){
		fseek(fp, 527+cnt,0);
		putc('1',fp);
		fflush(fp);

}



int f_sh_data_empty(FILE *fp){   //

		int cnt = 0;
		char c;

		fseek(fp,528/*(16+512)*/,0); 

		for(int i = 0 ; i<1024 ; i++){
				if( (c = getc(fp)) == '0'){
						cnt = i+1;
						break;
				}
		}

		return cnt;
}





void write_content_to_data(int num, LINK P){
		char new_name[128];
		strcpy(new_dir, P -> DB);
		fseek(fp, 16+512+1024+79*512+128*8*(num-1),0);
		for(int i=0; i<128; i++){
				char tmp_str[9] = {};
				for(int k=0; i<8; k++){
						tmp_str[k] = new_name[i] / j + '0';
						new_name[i] %= j;
						j /= 2;
				}
				fprintf(fp, "%s", c);
		}
}



void set_address(int,int,FILE *);

int main(void){
		FILE *fp = fopen("myfs","r+");

		set_address(104, 2 ,fp);


		return 0;
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
				printf("%d 지호\n",direct_num);
				fseek(fp, 16+512+1024+79*(inode_num-1)+49 ,0);
				binary(direct_num,str,10);
				fprintf(fp,"%s",str);
				write_content_to_data(direct num, P);  // 나중에 동적할당된 포인터 P 적어주기

		}

		else if((size >1) && (size <=103)){

				direct_num = f_sh_data_empty(fp);
				u_sb_data(fp,direct_num);  
				printf("%d\n",direct_num);
				fseek(fp,16+512+1024+79*(inode_num-1)+49,0)	;
				binary(direct_num,str,10);	
				fprintf(fp,"%s",str);		

				write_content_to_data(direct_num,P);
				P = next(P);

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


						write_content_to_data(in_single,P);
						P = next(P);
				}

		}

		else {

				direct_num = f_sh_data_empty(fp);  
				u_sb_data(fp,direct_num);
				printf("%d ",direct_num);
				fseek(fp,16+512+1024+79*(inode_num-1)+49,0);
				binary(direct_num,str,10);
				fprintf(fp,"%s",str);

				write_content_to_data(direct_num,P);
				P = next(P);

				single_num = f_sh_data_empty(fp); //inode : single 부분에 저장
				u_sb_data(fp,single_num);
				printf("%d single \n",single_num);
				fseek(fp,16+512+1024+79*(inode_num-1)+59,0);
				binary(single_num,str,10);
				fprintf(fp,"%s",str);

				double_num = f_sh_data_empty(fp);  //inode : double부분에 저장 
				printf("%d double\n",double_num);
				u_sb_data(fp,double_num);
				fseek(fp,16+512+1024+79*(inode_num-1)+69,0);
				binary(double_num,str,10);
				fprintf(fp,"%s",str);


				for(int k = 0; k<102 ;k++){  //single 데이터 블록에 넣는 숫자들

						in_single = f_sh_data_empty(fp);
						u_sb_data(fp,in_single);
						printf("%d in_single\n",in_single);
						fseek(fp,16+512+1024+ 79*512 + 128*8*(single_num-1) + 10*k,0  );
						str = binary(in_single,str,10);
						fprintf(fp,"%s",str);

						write_content_to_data(in_single,P);
						P = next(P);		 
				}

				for(int i=0; i< 102 ;i++){
						in_double_in = f_sh_data_empty(fp);
						u_sb_data(fp,in_double_in);
						printf("%d in_double_in\n");
						fseek(fp,16+512+1024+ 79*512+ 128*8*(in_double-1)+10*i,0);
						binary(in_double_in,str,10);
						fprintf(fp,"%s",str);


						for(int j =0 ; j<102 ;j++){

								in = f_sh_data_empty(fp);
								u_sb_data(fp,in);
								printf("%d in",in);
								fseek(fp,16+512+1024+79*512+ 128*8*(in_double_in-1)+10*j,0);

								binary(in,str,10);
								fprintf(fp,"%s",str);
								count++;
								write_content_to_data(in,P);
								P = next(P);

								if(count == size-103)
										break;
						}
						if(count == size-103)
								break;
				}
		}
}




