#include<stdio.h>

int f_sh_data_empty(FILE *fp){

		int cnt = 0;


		fseek(fp,528,0); 

		for(int i = 0 ; i<1024 ; i++){
				if(getc(fp) == '0'){
						cnt = i+1;
						break;
				}
		}

		return cnt;
}

void set_inode_address(int size){

		int diret_num;
		int single_num;
		int double_num;
		int in_single;
		int in_double;
		char str[11];
		int count = 0;
		int size; // 전체 필요한 데이터 블록 개수  딱 개수만
		if(size==1){
				direct_num = f_sh_data_empty(fp);  		

				fseek(fp, 16+512+1024+79*(inode_num-1)+49 ,0);
				str = binary(direct_num,str,10);
				fprintf(fp,"%s",str);					

				u_sb_data(fp, direct_num);
		}

		else if((size >1) && (size <=103)){

				direct_num = f_sh_data_empty(fp);
				u_sb_data(fp,direct_num);  

				fseek(fp,16+512+1024+79*(inode_num-1)+49,0)	;
				str = binary(direct_num,str,10);	
				fprintf(fp,"%s",str);		

				/* direct_num번째 데이터 블록에 내용쓰기*/

				single_num = f_sh_data_empty(fp);
				u_sb_data(fp,single_num);

				fseek(fp,16+512+1024+79*(inode_num-1)+59,0);
				str = binary(single_num,str,10);
				fprintf(fp,"%s",str);

				for(int k=0; k< size-1 ;k++){
						in_single = f_sh_data_empty(fp);
						u_sb_data(fp,in_single);

						fseek(fp,16+512+1024+79*512 + 128*8*(single_num-1) + 10*k,0);
						str = binary(in_single,str,10);
						fprintf(fp,"%s",str);

						/*in_single번째 데이터블록에 내용도 적어야해요*/

				}




		}

		else {

				direct_num = f_sh_data_empty(fp);  
				u_sb_data(fp,direct_num);

				fseek(fp,16+512+1024+79*(inode_num-1)+49,0);
				str = binary(direct_num,str,10);
				fprintf(fp,"%s",str);

				/*direct_num 번째 데이터 블록에 내용 쓰기*/

				single_num = f_sh_data_empty(fp); //inode : single 부분에 저장
				u_sb_data(fp,single_num);

				fseek(fp,16+512+1024+79*(inode_num-1)+59,0);
				str = binary(single_num,str,10);
				fprintf(fp,"%s",str);

				double_num = f_sh_empty(fp);  //inode : double부분에 저장 
				u_sb_data(fp,double_num);

				fseek(fp,16+512+1024+79*(inode_num-1)+69,0);
				str = binary(double_num,str,10);
				fprintf(fp,"%s",str);


				for(int k = 0; k<102 ;k++){  //single 데이터 블록에 넣는 숫자들

						in_single = f_sh_data_empty(fp);
						u_sb_data(fp,in_ingle);

						fseek(fp,16+512+1024+ 79*512 + 128*8*(single_num-1) + 10*k  );
						str = binary(in_single,str,10);
						fprintf(fp,"%s",str);

						//  in_single 번호 데이터 블록에 내용도 쓰기

				}

				in_double = f_sh_data_empty(fp);
				u_sb_data(fp,in_double);

				fseek(fp,16+512+1024+79*512+ 128*8*(double_num-1) );
				str = binary(in_double,str,10);
				fprintf(fp,"%S",str);			

				for(int i=0; i< 102 ;i++){
						in_double_in = f_sh_data_empty(fp);
						u_sb_data(fp,in_double_in);

						fseek(fp,16+512+1024+ 79*512+ 128*8*(in_double-1)+10*i);
						str = binary(in_double_in,str,10);
						fprintf(fp,"%s",str);


						for(int j =0 ; j<102 ;j++){

								in = f_sh_data_empty(fp);
								u_sb_data(fp,in);

								fseek(fp,16+512+1024+79*512+ 128*8*(in_double_in-1)+10*j);
								str = binary(in,str,10);
								fprintf(fp,"%s",str);
								count++;
								//in 번째 데이터블록에 내용 적기


						}
						if(count == size-103);
				}
		}
}




