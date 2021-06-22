#include "WJLSE3.h"
#include "WJLSE.h"
#include "WJLSD.h"

unsigned char* encrypt(const char *PasswordBuff, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length)
{
	WJLEnCoder coder;
	int PasswordBuff_Length = 0;
	while (PasswordBuff[PasswordBuff_Length] != '\0') PasswordBuff_Length++;
	// Initialize WJLEnCoder
	InitializeWJLEnCoder(&coder, InByteArray_length);
	// Encrypt to 64 random numbers 64 = 512bit
	EncryptingTheRandomBytes(&coder,PasswordBuff, PasswordBuff_Length, 64);
	// Custom hash values of 65 bytes
	EncryptingTheHashValue(&coder, PasswordBuff, PasswordBuff_Length, 65);
	// Encrypt the InByteArray_length
	EncryptingFileLength(&coder, InByteArray_length);
	// Encrypt the InByteArray
	return WJLWeightedEncode(&coder, PasswordBuff, PasswordBuff_Length, InByteArray, InByteArray_length, OutByteArray_length);
}

unsigned char* decrypt(const char *PasswordBuff, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length)
{
	WJLDeCoder coder;
	int PasswordBuff_Length = 0, InArraylength = 0;
	while (PasswordBuff[PasswordBuff_Length] != '\0') PasswordBuff_Length++;
	// Initialize WJLDeCoder
	InitializeWJLDeCoder(&coder, InByteArray, InByteArray_length);
	// Decrypt to 64 random numbers 64 = 512bit
	if(decryptingRandomBytes(&coder,PasswordBuff, PasswordBuff_Length, InByteArray, InByteArray_length, 64)){
		// Decrypt the HashValue and check
		if(decryptingHashValue(&coder, InByteArray, InByteArray_length, PasswordBuff, PasswordBuff_Length, 65)){
			// Decrypt the InByteArray_length
			if(InArraylength = decryptingInArraylength(&coder, InByteArray, InByteArray_length)){
				// Decrypt the datas
				return WJLWeightedDecode(&coder, PasswordBuff, PasswordBuff_Length, InByteArray, InByteArray_length, InArraylength, OutByteArray_length);
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}

