# WJLSymmetricEncryption
WJLSymmetricEncryption is a symmetric encryption algorithm source program based on a weighted probability model, which has:
1, is based on the wheel function of the weight coefficient and, yes, the information entropy of the original data does not change.
2, supports custom secret key length, combined with WJLHA3.
3, employs random numbers (the length that can be defined), making the results different for each coding.

Use the method as below:

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
	int main(){
		long t1,t2;
		int i = 0, tmp = 0;
		int InBUFF_Length = 100000;
		unsigned int OutByteArray_length1,OutByteArray_length2;
		unsigned char *In_BUFF;
		unsigned char *out_BUFF1, *out_BUFF2;
		const char *PasswordBuff = "123456789ssdfsdf444asdfasdf";
		In_BUFF = (unsigned char *)malloc(InBUFF_Length);
		printf("\nData before the encryption:\n");
		for(i = 0; i < InBUFF_Length; ++i){
			In_BUFF[i] = (unsigned char)(i % 256);
			printf("%d,",In_BUFF[i]);
		}
		printf("\n");
		t1 = GetTickCount();
		out_BUFF1 = encrypt(PasswordBuff, In_BUFF, InBUFF_Length, &OutByteArray_length1);
		t2 = GetTickCount();
		printf("Encryption Time:%d ms\n", t2 - t1);
		printf("Number of bytes after encryption:%d\n", OutByteArray_length1);
		printf("\nEncrypted data:");
		for(i = 0; i < OutByteArray_length1; ++i){
			printf("%d,",out_BUFF1[i]);
		}
		printf("\n");

		t1 = GetTickCount();
		out_BUFF2 = decrypt(PasswordBuff, out_BUFF1, OutByteArray_length1, &OutByteArray_length2);
		t2 = GetTickCount();
		if(out_BUFF2 != 0){
			printf("Decryption Time::%d ms\n", t2 - t1);
			printf("Number of bytes after decryption:%d\n", OutByteArray_length2);
			printf("\nDecrypted data:");
			for(i = 0; i < OutByteArray_length2; ++i){
				printf("%d,",out_BUFF2[i]);
			}
			printf("\n");
		}
		system("pause");
		return 0;
	}
