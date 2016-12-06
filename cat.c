#include<stdio.h>
typedef struct inode{
	int ftype;
	int itime;
	int size;
	int direct;
	int single;
	int Double;
	char name[];
} inode;



int main(){

}


void tem_to_datablock(/*동적메모리 할당 포인터 */, int size, FILE *fp){
	char c;
	int inode_num = 0;
	int block_num = 0;

	//수퍼블록으로 가서 안쓰고있는 블록 번호 찾기
	fseek(fp,17,SEEK_SET);
	for(int i = 0; i<512 ;i++){   //안쓰고 있는 아이노드 번호 찾기; 
		c = getc(fp) 
			if( c == 0 )
			{
				inode_num = i;
				fseek(fp,1+inode_num,SEEK_SET);
				fprintf("fp","1");  // 수퍼블록 아이노드 사용여부 1로  바꾸기  맞나??
				break;
			}
	}


	fseek(fp, 513,SEEK_SET);   // 안쓰는 데이터블록 번호 찾기
	for(int j = 0; j<1024 ;j++){

		c = getc(fp)
			if(c == 0)
			{
				block_num = j;
				break;
			}
	}


	for(i=0; i<size ;i++){   //size : 동적메모리 할당된 개수
		P[i] = datablock[block_num+i];  //같은형의 구조체로 만들어서 쏙넣기 
		fseek(fp,513+block_num,SEEK_SET);	
	        //  수퍼블록의 데이터블록 사용여부 1 로 바꾸기//
	}


}




int seek_file_name(LINK head)
{
	short compare;

	if(head == NULL)
		return ;
	else{
		compare = strcmp(head->name, file1);
		if(compare != 0)   // 이름이  같지 않은경우
			seek_file_name(head->next);
		else 
			return head->inode_num;
	}
}


void mycat(LINK head, char *express){  //LINK : 아이노드 구조체 포인터

	//아이노드 구조체 주소를 매개 변수로 가져오기
	char express[20]; 

	char file1[5]; 
	char file2[5];
	char file3[5];
	char op;  //    > 나 >>받기
	int inode_num = 0;



	if ( sscanf(express,"%s %S %c %s", &file1 ,&file2 , &op, &file3) == 1) 
	{   //mycat app2 
		inode_num = seek_file_name(head->next);   // 파일의 아이노드 번호 찾기

		// 그 아이노드 번호의 데이터 블록 긁어 오기   (direct 가 가리키는 블록)



	}

	if( sscanf(express,"%s %s %c %S", &file1,&file2, &op, &file3)  == 4)   //mycat apple melon > app2
	{	   

         if(op == ">") 
		      //apple 이라는 파일이름을 가진 아이노드 찾기
		      //melon 이라는 파일이름을 가진 아이노드 찾기
                     //복사하는 함수 이용해서 새로운 데이터 블록에 복사 두개 이어서 한파일(file3)  mycp 있어야 되네
                

	}


}
