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
	int InBUFF_Length = 100000;// 输入的字节长度
	unsigned int OutByteArray_length1,OutByteArray_length2;
	unsigned char *In_BUFF;
	unsigned char *out_BUFF1, *out_BUFF2;
	unsigned int RandomBytes_Length = 64; // 明文编码前先编码RandomBytes_Length个字节的随机数
	const char *PasswordBuff = "123456789s阿斯蒂芬感受到分sdfsdf444"; // 不限制长度
	In_BUFF = (unsigned char *)malloc(InBUFF_Length);
	// 随机生成In_BUFF_Len长度的数据
	printf("\n加密前的数据：\n");
	srand((unsigned)time(NULL));
	for(i = 0; i < InBUFF_Length; ++i){
		In_BUFF[i] = rand() % 2;//(unsigned char)(i % 255);
		//printf("%d,",In_BUFF[i]);
	}
	/*****************************************加密部分*****************************************/
	// 调用WJLSE3算法
	printf("\n");
	t1 = GetTickCount();
	out_BUFF1 = encrypt(PasswordBuff, In_BUFF, InBUFF_Length, &OutByteArray_length1, RandomBytes_Length);
	t2 = GetTickCount();
	printf("加密耗时 :%d ms\n", t2 - t1);
	printf("加密后字节数 :%d\n", OutByteArray_length1);
	printf("\n加密后的数据：");
	for(i = 0; i < OutByteArray_length1; ++i){
		//printf("%d,",out_BUFF1[i]);
	}
	printf("\n");

	/****************************************解密部分******************************************/
	t1 = GetTickCount();
	out_BUFF2 = decrypt(PasswordBuff, out_BUFF1, OutByteArray_length1, &OutByteArray_length2, RandomBytes_Length);
	t2 = GetTickCount();
	if(out_BUFF2 != 0){
		printf("解密耗时 :%d ms\n", t2 - t1);
		printf("解密后字节数 :%d\n", OutByteArray_length2);
		printf("\n解密后的数据：");
		for(i = 0; i < OutByteArray_length2; ++i){
			//printf("%d,",out_BUFF2[i]);
		}
		//printf("\n");
	}else{
		printf("解密耗时 :%d ms\n", t2 - t1);
		printf("解密失败\n");
	}
	
	system("pause");
	return 0;
}
