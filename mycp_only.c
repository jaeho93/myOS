void mycp(FILE *,char*,char*);
void txt_sender(FILE*,int,int);
void Double_to_single(FILE *fp,int Double,int data_block);
void single_to_txt_sender(FILE *fp,int single,int data_block);


void mycp(FILE *fp, char * str1, char * str2)
{
	if(strlen(str1) == 0)
	{
		printf("복사할 파일 이름을 지정하시오.\n\n");
		return;
	}
	if(strlen(str2) == 0)
	{
		printf("복사될 파일 이름을 지정하시오.\n\n");
		return;
	}
	int inode1 = 0;
	LINK file = mvfind_file(ia[nowdir_inode-1], str1, &inode1);
	if(file == NULL || file -> sib != NULL)
	{
		printf("%s is not a file.\n\n", str1);
		return;
	}
	inode * ip = (inode*)malloc(sizeof(inode));
	get_inode(fp, ip, 1552+79*(inode1-1));
	int size = ip -> size;
	int dir  = ip -> direct;
	int single = ip -> single;
	int Double = ip -> Double;

	int sb_inode_empty = f_sb_inode_empty(fp);
	int sb_data_empty=f_sb_data_empty(fp);
	u_sb_inode(fp, sb_inode_empty);
	write_name_to_dir(fp, ia[nowdir_inode-1], sb_inode_empty, str2);
	link_newfile(ia[nowdir_inode-1], sb_inode_empty, str2);
	int data_block = size / 128;
	if((size % 128) != 0)
		data_block++;
	if(data_block==1){
		txt_sender(fp,sb_data_empty,dir);
	}
	else if(data_block>1 && data_block<=103){
		txt_sender(fp,sb_data_empty,dir);
		single_to_txt_sender(fp,single,data_block);
	}
	else if(data_block>103){
		txt_sender(fp,sb_data_empty,dir);
		single_to_txt_sender(fp,single,data_block);
		Double_to_single(fp,Double,data_block);	
		}
}
void txt_sender(FILE *fp,int sb_data_empty,int dir)
{
	fseek(fp,16+512+1024+512*79+128*8*(dir-1),0);
	int txt[128];
	char letter[8];
	for(int i=0;i<128;i++)
		txt[i]=binary_changer(fp,8);

	fseek(fp,16+512+1024+512*79+128*8*(sb_data_empty),0);
	for(int i=0;i<128;i++){
		binary(txt[i],letter,8);
		fprintf(fp,letter);
	}
	for(int j=0;j<8;j++)
		letter[j]='\0';
}
void single_to_txt_sender(FILE *fp,int single,int data_block)
{
	int address[102];
	int sb_data_empty;
	fseek(fp,16+512+1024+512+128*8*(single-1),0);
	for(int i=0;i<data_block-1;i++)//direct블록 1개를 제외
		address[i]=binary_changer(fp,10);
	for(int i=0;i<data_block-1;i++){
		sb_data_empty=f_sb_data_empty(fp);
		u_sb_data(fp,sb_data_empty);
		txt_sender(fp,sb_data_empty,address[i]);
	}
	for(int i=0;i<102;i++)
		address[i]='\0';
}
void Double_to_single(FILE *fp,int Double,int data_block)
{
	int address[102];
	int sb_data_empty;
	fseek(fp,16+512+1024+512+128*8*(Double-1),0);
	for(int i=0;i<data_block-103;i++)
		address[i]=binary_changer(fp,10);
	for(int i=0;i<data_block-103;i++){
		sb_data_empty=f_sb_data_empty(fp);
		u_sb_data(fp,sb_data_empty);
		single_to_txt_sender(fp,address[i],data_block);
	}
	for(int i=0;i<102;i++)
		address[i]='\0';
}

