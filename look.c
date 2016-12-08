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

int main(){

		int diret_num;
		int single_num;
		int double_num;
		int in_single;
		int in_double;

		if(size==1){
				direct_num = f_sh_data_empty(fp);  //  다이렉트 번호, 아이노드의 direct부분에 적어야함
				u_sb_data(fp, direct_num);
		}

		else if((size >1) && (size <=103)){

				direct_num = f_sh_data_empty(fp);
				u_sb_data(fp,direct_num);  // 파일에 쓰고있다고 적음

				single_num = f_sh_data_empty(fp);
				u_sb_data(fp,single_num);

				for(int k=0; k< size-1 ;k++){
						in_single = f_sh_data_empty(fp);
						u_sb_data(fp,in_single);
						//in_single 로 번호 넘어오자마자10 비트로 바꿧  데이터 블록(single_num)번째 에 적음 single에 적히는 파일 개수만큼 반복
						//내용도 적으머
				}


		}

		else {

				direct_num = f_sh_data_empty(fp);  //inode : direct부분에 저장
				u_sb_data(fp,direct_num);
				//direct_num 번째 데이터 블록에 내용 쓰기

				single_num = f_sh_data_empty(fp); //inode : single 부분에 저장
				u_sb_data(fp,single_num);
				double_num = f_sh_empty(fp);  //inode : double부분에 저장 
				u_sb_data(fp,double_num);


				for(int k = 0; k<102 ;k++){  //single 데이터 블록에 넣는 숫자들

						in_single = f_sh_data_empty(fp);
						u_sb_data(fp,in_ingle);
						//  in_single 번호 데이터 블록에 내용도 쓰기

				}

				//in_double = f_sh_data_empty(fp);
				//u_sb_data(fp,in_double);
				for(int i=0;i<102;i++){
						in_double_in = f_sh_data_empty(fp);
						u_sb_data(fp,in_double_in);
						for(int j =0 ; j<102 ;j++){

								in = f_sh_data_empty(fp);
								u_sb_data(fp,in);
								//in 번째 데이터블 록에 내용 적기

								//double_num 번째 데이터 블록에 10 비트로 바꿔서 in_double_in 차곡차곡 쓰기
						}

				}
		}
