#include<stdio.h>

void myls(char *);
void mycat(char *);
void myshowfie(char *);
void mypwd(char *);
void mycd(char *);
void mycp(char *);
void mycpto(char *);
void mycpfrom(char *);
void mymkdir(char *);
void myrmdir(char *);
void myrm(char *);
void mymv(char *);
void mytouch(char *);
void myshowinode(char *);
void myshowblock(char *);
void mystate(char *);
void mytree(char *);

int main(void){

		char express[33];
		char command[10];
		char command2[20];


		scanf("%[^\n]s ", express);  //처음에 한줄 쫙 다받음
 
 		sscanf("%s %[^\n]s", command, command2);  // 명령어는 command 배열에  그나머지 다 command2 배열에


		if(  strcmp(command, "myls" ) == 0 ){
				myls(command2);

		}

		if(strcmp(command , "mycat") == 0 ){
				mycat(command2)
		}

		if( strcmp(command, "myshowfile") == 0 ) {
				myshowfile(command2);
		}

		if(  strcmp(command, "mypwd" ) == 0 ){
				mypwd(command2);
		}

		if(  strcmp(command, "mycd" ) == 0 ){
				mycd(command2);
		}

		if(  strcmp(command, "mycp" ) == 0 ){
				mycp(cammand2);
		}

		if(  strcmp(command, "mycpto" ) == 0 ){
				mycpto(command2);
		}

		if(  strcmp(command, "mypfrom" ) == 0 ){
				mycpfrom(command2);
		}

		if(  strcmp(command, "mymkdir" ) == 0 ){
				mymkdir(command2);
		}

		if(  strcmp(command, "myrmdir" ) == 0 ){
				myrmdir(command2);
		}

		if(  strcmp(command, "myrm" ) == 0 ){

				myrm(cammand2);
		}

		if(  strcmp(command, "mymv" ) == 0 ){

				mymv(command2);
		}
		if(  strcmp(command, "mytouch" ) == 0 ){

				mytouch(command2);
		}

		if(  strcmp(command, "myinode" ) == 0 ){

				myinode(command2);
		}
		if(  strcmp(command, "myshowblock" ) == 0 ){

				myshowblock(command2);
		}
		if(  strcmp(command, "mystate" ) == 0 ){

				mystate(command2);
		}
		if(  strcmp(command, "mytree" ) == 0 ){

				mytree(command2);

		}

		return 0;
}

