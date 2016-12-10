#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char fname[5];
void write_content_to_Data(FILE *fp);
void bin_to_str(FILE *fp);

int main(void){
	FILE *fp;
	fopen("test.txt","r");

	strcpy(fname,"apple");

	printf("%s",fname);

	write_content_to_Data(fp);


	return 0;
}

void bin_to_str(FILE *fp){
	for(int i=0; i<4; i++){
		for(int j=128; j>=1; j/=2)
			fname[i] += (getc(fp) - '0') * j;
	}
}

void write_content_to_Data(FILE *fp){
	int num = 5;
	char new_name[5];
	int j=0;

	strcmp(new_name, fname);

	fseek(fp,10*(num-1),0);

	for(int i=0; i<128; i++){
		char tmp_str[5] = {};
		
		for(int k=0; k<5; k++){
			tmp_str[k] = new_name[i] / j + '0';
			new_name[i] %= j;
			j /= 2;
		}
		
		fprintf(fp, "%s\n", tmp_str);
	}
}

