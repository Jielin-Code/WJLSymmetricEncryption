#include "WJLSE3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#ifdef WIN32
#define  inline __inline
#endif // WIN32

// test
int main(){
	long t1,t2;
	int i = 0, tmp = 0;
	int InBUFF_Length = 100000;// ������ֽڳ���
	unsigned int OutByteArray_length1,OutByteArray_length2;
	unsigned char *In_BUFF;
	unsigned char *out_BUFF1, *out_BUFF2;
	const char *PasswordBuff = "123456789s��˹�ٷҸ��ܵ���sdfsdf444"; // �����Ƴ���
	In_BUFF = (unsigned char *)malloc(InBUFF_Length);
	// �������In_BUFF_Len���ȵ�����
	printf("\n����ǰ�����ݣ�\n");
	for(i = 0; i < InBUFF_Length; ++i){
		In_BUFF[i] = (unsigned char)(i % 256);
		printf("%d,",In_BUFF[i]);
	}
	/*****************************************���ܲ���*****************************************/
	// ����WJLSE3�㷨
	printf("\n");
	t1 = GetTickCount();
	out_BUFF1 = encrypt(PasswordBuff, In_BUFF, InBUFF_Length, &OutByteArray_length1);
	t2 = GetTickCount();
	printf("���ܺ�ʱ :%d ms\n", t2 - t1);
	printf("���ܺ��ֽ��� :%d\n", OutByteArray_length1);
	printf("\n���ܺ�����ݣ�");
	for(i = 0; i < OutByteArray_length1; ++i){
		printf("%d,",out_BUFF1[i]);
	}
	printf("\n");
	/****************************************���ܲ���******************************************/
	
	t1 = GetTickCount();
	out_BUFF2 = decrypt(PasswordBuff, out_BUFF1, OutByteArray_length1, &OutByteArray_length2);
	t2 = GetTickCount();
	if(out_BUFF2 != 0){
		printf("���ܺ�ʱ :%d ms\n", t2 - t1);
		printf("���ܺ��ֽ��� :%d\n", OutByteArray_length2);
		printf("\n���ܺ�����ݣ�");
		for(i = 0; i < OutByteArray_length2; ++i){
			printf("%d,",out_BUFF2[i]);
		}
		printf("\n");
	}
	
	system("pause");
	return 0;
}
