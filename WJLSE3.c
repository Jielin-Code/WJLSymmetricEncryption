#include "WJLSE3.h"
#include "WJLSE.h"
#include "WJLSD.h"

unsigned char* encrypt(const char *PasswordBuff, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length, unsigned int RandomBytes_Length)
{
	WJLEnCoder coder;
	int PasswordBuff_Length = 0;
	unsigned char *OutBuff;
	while (PasswordBuff[PasswordBuff_Length] != '\0') PasswordBuff_Length++;

	// Initialize variable
	coder.symbolsMaxCount = 256;
	coder.SymbolsFreqArray = (unsigned int *)malloc(sizeof(unsigned int) * (coder.symbolsMaxCount + 1));
	coder.encodeOutBufferArray = (unsigned char *)malloc((double)InByteArray_length * 1.2);
	coder.Sbox = (unsigned char *)malloc( coder.symbolsMaxCount );
	if(RandomBytes_Length < 64){
		coder.RandomBytes_Length = 64;
	}else{
		coder.RandomBytes_Length = RandomBytes_Length;
	}
	coder.RandomBytes = (unsigned char *)malloc(coder.RandomBytes_Length);
	if(coder.SymbolsFreqArray == NULL || coder.encodeOutBufferArray == NULL || coder.Sbox == NULL || coder.RandomBytes == NULL){
		if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
		if(coder.encodeOutBufferArray) free(coder.encodeOutBufferArray);
		if(coder.Sbox) free(coder.Sbox);
		if(coder.RandomBytes) free(coder.RandomBytes);
		return 0;
	}
	// Initialize WJLEnCoder
	InitializeWJLEnCoder(&coder, PasswordBuff, PasswordBuff_Length);
	// Encrypt to 64 random numbers 64 = 512bit
	EncryptingTheRandomBytes(&coder, PasswordBuff, PasswordBuff_Length);
	// Encrypt the InByteArray_length
	EncryptingFileLength(&coder, InByteArray_length, PasswordBuff, PasswordBuff_Length);
	// Encrypt the InByteArray
	WJLWeightedEncode(&coder, PasswordBuff, PasswordBuff_Length, InByteArray, InByteArray_length, OutByteArray_length);
	
	OutBuff = (unsigned char *)malloc( *OutByteArray_length );
	if(OutBuff == NULL){
		if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
		if(coder.encodeOutBufferArray) free(coder.encodeOutBufferArray);
		if(coder.Sbox) free(coder.Sbox);
		if(coder.RandomBytes) free(coder.RandomBytes);
		return 0;
	}
	memcpy(OutBuff, coder.encodeOutBufferArray, *OutByteArray_length);

	if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
	if(coder.encodeOutBufferArray) free(coder.encodeOutBufferArray);
	if(coder.Sbox) free(coder.Sbox);
	if(coder.RandomBytes) free(coder.RandomBytes);
	return OutBuff;
}

unsigned char* decrypt(const char *PasswordBuff, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length, unsigned int RandomBytes_Length)
{
	WJLDeCoder coder;
	int PasswordBuff_Length = 0, InArraylength = 0;
	unsigned char *OutBuff;
	while (PasswordBuff[PasswordBuff_Length] != '\0') PasswordBuff_Length++;
	// Initialize variable
	coder.symbolsMaxCount = 256;
	coder.SymbolsFreqArray = (unsigned int *)malloc(sizeof(unsigned int) * (coder.symbolsMaxCount + 1));
	coder.decodeOutBufferArray = (unsigned char *)malloc( InByteArray_length * 8 );
	coder.Sbox = (unsigned char *)malloc( coder.symbolsMaxCount );
	if(RandomBytes_Length < 64){
		coder.RandomBytes_Length = 64;
	}else{
		coder.RandomBytes_Length = RandomBytes_Length;
	}
	if(coder.SymbolsFreqArray == NULL || coder.decodeOutBufferArray == NULL || coder.Sbox == NULL){
		if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
		if(coder.decodeOutBufferArray) free(coder.decodeOutBufferArray);
		if(coder.Sbox) free(coder.Sbox);
		return 0;
	}
	// Initialize WJLDeCoder
	InitializeWJLDeCoder(&coder, InByteArray, InByteArray_length, PasswordBuff, PasswordBuff_Length);
	// Decrypt to 64 random numbers 64 = 512bit
	if(decryptingRandomBytes(&coder, InByteArray, InByteArray_length, PasswordBuff, PasswordBuff_Length)){
		// Decrypt the InByteArray_length
		InArraylength = decryptingInArraylength(&coder, InByteArray, InByteArray_length, PasswordBuff, PasswordBuff_Length);
		if(InArraylength !=0){
			// Decrypt the datas
			if(WJLWeightedDecode(&coder, InByteArray, InByteArray_length, PasswordBuff, PasswordBuff_Length, InArraylength, OutByteArray_length)){
				OutBuff = (unsigned char *)malloc( InArraylength );
				if(OutBuff == NULL){
					if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
					if(coder.decodeOutBufferArray) free(coder.decodeOutBufferArray);
					if(coder.Sbox) free(coder.Sbox);
					return 0;
				}
				memcpy(OutBuff, coder.decodeOutBufferArray, *OutByteArray_length);
				if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
				if(coder.decodeOutBufferArray) free(coder.decodeOutBufferArray);
				if(coder.Sbox) free(coder.Sbox);
				return OutBuff;
			}else{
				if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
				if(coder.decodeOutBufferArray) free(coder.decodeOutBufferArray);
				if(coder.Sbox) free(coder.Sbox);
				return 0;
			}
		}else{
			if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
			if(coder.decodeOutBufferArray) free(coder.decodeOutBufferArray);
			if(coder.Sbox) free(coder.Sbox);
			return 0;
		}
	}else{
		if(coder.SymbolsFreqArray) free(coder.SymbolsFreqArray);
		if(coder.decodeOutBufferArray) free(coder.decodeOutBufferArray);
		if(coder.Sbox) free(coder.Sbox);
		return 0;
	}
}

