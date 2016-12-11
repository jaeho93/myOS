
void direct_block_reader(FILE *fp,int direct_num)
	//single내 direct중 128이 안되는 애를 접근해야하는 경우 rest에 그 애를 넣는다
{
	int letter;
	int basic=128;

	fseek(fp,16+512+1024+79*512+128*8*(direct_num-1),0);
	for(int i=0;i<basic;i++){
		letter=binary_changer(fp,8);
		show_cnt++;
		if(num1==0 && num2==0)
			printf("%c",letter);
		else if(num1<=show_cnt && show_cnt <=num2)
			printf("%c",letter);
	}
}
//single indirect의 주소로 접근해서 주소들을 10진수로 바꾼상태로 저장 하여 해당 >주소를 direct_block_reader로 보내기
void single_block_reader(FILE *fp,int single_num)
	//cnt는 single내에서 쓰이는 direct의 갯수입니다
{
	int basic=102;
	int direct[102];
	fseek(fp,16+512+1024+79*512+128*8*(single_num-1),0);
	for(int i=0;i<basic;i++){
		direct[i]=binary_changer(fp,10);
	}
	for(int j=0;j<basic;j++)
		direct_block_reader(fp,direct[j]);
}
void Double_block_reader(FILE *fp,int Double_num)
	//single_block_reader랑 같은 이유
{
	int basic=102;
	int single[102];
	fseek(fp,16+512+1024+79*512+128*8*(Double_num-1),0);
	for(int i=0;i<basic;i++){
		single[i]=binary_changer(fp,10);
	}
	for(int j=0;j<basic;j++)
		single_block_reader(fp,single[j]);
}
void mycat(FILE *fp,LINK head,char*file_name)
{
	int direct_db=0,single_db=0,Double_db=0;

	inode *ip=(inode*)malloc(sizeof(inode));
	LINK k=head;
	if(strcmp(k->name,file_name)==0){
		get_inode(fp,ip,16+512+1024+79*(k->inode_num-1));//inode의 시작위치까지만보내주면 아이노드 정보 10진화 해서 ip에 다 리턴해줌
			direct_db=ip->direct;//몇번째 db에 있는지 10진수 형태로주소가지고 있음
		single_db=ip->single;
		Double_db=ip->Double;

		if(Double_db==0 && single_db==0)//direct만 쓴경우
			direct_block_reader(fp,direct_db);
		else if(Double_db==0){//single까지 쓴경우
			direct_block_reader(fp,direct_db);
			single_block_reader(fp,single_db);
		}
		else{//double까지 다쓴경우
			direct_block_reader(fp,direct_db);
			single_block_reader(fp,single_db);
			Double_block_reader(fp,Double_db);
		}
	}
	else
		mycat(fp,head->next,file_name);

}
