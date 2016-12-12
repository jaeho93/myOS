#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SB_SIZE 1536
#define BB_SIZE 16
#define INODE_SIZE 39936


void binary(int, char*, int);
		
int main(void)
{
	FILE *out;
	if((out = fopen("myfs", "r")) != NULL)
	{
		printf("error : myfs exists.\n\n");
		return 0;
	}
	else
		out = fopen("myfs", "w");


	fprintf(out, "0000000000000000"); // bootBlock 16비트

	fprintf(out, "1");				  // superBlock_inode 첫번째 비트 > 루트 디렉터리 사용

	for(int i = 0; i <511; i++)
	{
		fprintf(out, "0");			  // superBlock_inode 나머지 511개 비트
	}

	fprintf(out, "10");				  // superBlock_dataBlock 1,2번째 비트 > 루트 디렉터리 데이터, 루트디렉터리 다이렉트블록


	for(int i = 0; i < 1022; i++)
	{
		fprintf(out, "0");			  // superBlock_dataBlock 나머지 1022개 비트
	}

///////////////////////////아이노드리스트/////////1개당 79비트/////////////////////////////////////////////

	fprintf(out, "0");				  // 아이노드리스트_파일종류 > 루트 디렉터리 0 : 디렉터리 1 : 일반파일
	
	time_t  now;
	now = time(NULL);

	char strTime[32] = {};
	binary(now, strTime, 31);
	fprintf(out, "%s", strTime);	  // 아이노드리스트_파일생성날짜 > 루트디렉터리 30개
	fprintf(out, "00000000000000000");// 아이노드리스트_파일크기 > 최대로 가질수있는 크기 : 129,280바이트 1010 데이타블록 17개 (디렉터리는파일크기가0)
	fprintf(out, "0000000001");		  // 아이노드리스트_다이렉트블록 > 루트디렉터리의 데이터블록 주소
	fprintf(out, "0000000000");		  // 아이노드리스트_싱글다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값 10개
	fprintf(out, "0000000000");		  // 아이노드리스트_더블다이렉트블록 > 루트디렉터리는 필요 없으므로 NULL값

	for(int i = 0; i < 511; i++)
	{
		for(int j = 0; j < 79; j++)
		{
			fprintf(out, "0");
		}
	}
//////////////////////////////////데이타블록/////////////////////////////////////////////////////////
	fprintf(out, "00101110");									/// .           << 디렉터리 내용은 이런모양을 구현
	fprintf(out, "00000000");		// 한바이트                 /// 1
	fprintf(out, "00000000");		// 한바이트			        /// ..
	fprintf(out, "00000000");		// 한바이트			    	/// 1		  루트디렉터리의 ..은 자기 자신을 가리킨다.
	fprintf(out, "0000000001");

	fprintf(out, "00101110");
	fprintf(out, "00101110");
	fprintf(out, "00000000");		// 한바이트
	fprintf(out, "00000000");		// 한바이트
	fprintf(out, "0000000001");
	
	for(int i = 0; i < 117; i++)
	{
		fprintf(out, "00000000");
	}
	fprintf(out, "0000");

	////////////////////////////////////////////루트 디렉터리의 다이렉트블록
	
	for(int i = 0; i < 1023; i++)
	{
		for(int j = 0; j < 127; j++)
		{
			fprintf(out, "00000000");
		}
	}




////////////////////////////////////////////////////////////////////////////////////////////////////	
//	for(int i = 0; i < 126; i++)			// 디렉터리의 데이터블록 한칸 
//	{
//		fprintf(out, "00000000");		// 한바이트
//	}
//	fprintf(out, "00101111");			// '/' 아스키코드
//	fprintf(out, "00101110");			// '.' 아스키코드
//
//	for(int i = 0; i < 127; i++)			// 디렉터리의 다이렉트블록 한칸 
//	{
//		fprintf(out, "00000000");		// 한바이트
//	}
//	fprintf(out, "00000000");			// 0번 주소를 가리킴 -> 디렉터리의 데이터블록 주소
//
//	for(int i = 0; i < 1022; i++)
//	{
//		for(int j = 0; j < 128; j++)
//		{
//			fprintf(out, "00000000");
//		}
//	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
	fclose(out);

	return 0;
}

void binary(int time, char*str, int num)
{
	for(int i = num-1; i >= 0; i--)
	{
		str[i] = time % 2 + '0';
		time /= 2;
	}
	return;
}
