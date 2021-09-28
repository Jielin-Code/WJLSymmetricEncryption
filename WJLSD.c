#include "WJLSD.h"
// Random - generated byte 2D table,Customizable tables
unsigned char SDRandomOfUCharTable[256] =	{
	0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
	0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
	0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
	0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
	0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
	0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
	0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
	0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
	0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
	0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
	0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
	0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
	0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
	0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
	0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
	0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};
// Initialize structure
void InitializeWJLDeCoder(WJLDeCoder *coder, unsigned char *InByteArray, unsigned int InByteArray_length, const char *passwords, int passwords_Length)
{
	int i, n;
	unsigned int values = 0;
	coder->rangeCodeBitsLenght = 31;
	coder->rangeCodeShiftBitsLenght = coder->rangeCodeBitsLenght - 8;
	coder->rangeCodeMaxIterval = 1 << coder->rangeCodeBitsLenght;
	coder->rangeCodeMinIterval = 1 << coder->rangeCodeShiftBitsLenght;
	coder->counterMaxFreq = 0x00FFFFL;
	coder->symbolsMaxCount = 256;
	coder->coefficient = 1.0;
	coder->encodeFlow = 0;
	coder->encodeFlowRange = coder->rangeCodeMaxIterval;
	coder->encodeDelay = 0;
	coder->encodeDelayCount = 0;
	coder->decodeInBufferLoop = 0;
	coder->decodeOutBufferLoop = 0;
	coder->Total = 0;
	coder->CumFreq = 0;
	coder->Freq = 0;
	coder->values = 0;
	coder->Result = 0;
	for(i = 0; i <= coder->symbolsMaxCount; ++i){
		coder->SymbolsFreqArray[i] = 1;
	}
	// Total is the Sum of all symbol's freq
	coder->Total = coder->symbolsMaxCount + 1;
	// Initialize S-Box by passwords
	for(i = 0; i < coder->symbolsMaxCount; ++i){
		coder->Sbox[i] = passwords[ i % passwords_Length ] ^ passwords[ ( i + 1 ) % passwords_Length ] ^ SDRandomOfUCharTable[ i ];
	}
	// load the first 4bytes in InByteArray
	n = 32;
	do {
		n -= 8;
		values = (values << 8) | (unsigned char)InByteArray[coder->decodeInBufferLoop] ^ coder->Sbox[ coder->decodeInBufferLoop % coder->symbolsMaxCount ];
		coder->decodeInBufferLoop = coder->decodeInBufferLoop + 1;
	} while ( n > 0 );
	coder->values = values;
}
// Query 'SERandomOfUCharTable' table function
double getDeCoefficient(WJLDeCoder *coder, int index, unsigned char Symbol, const char *passwords, int passwords_Length)
{
	return 1.0 - (double)(((unsigned int)((unsigned char)passwords[  ((unsigned char)passwords[ index % passwords_Length ] ^ (unsigned char)passwords[ (index + 1) %  passwords_Length ]) % passwords_Length ]) * 256.0 + coder->Sbox[ ((Symbol ^ (unsigned char)passwords[ index % passwords_Length ]) % 16) * 16 + (Symbol ^ (unsigned char)passwords[ passwords_Length - (index % passwords_Length) ]) % 16 ])) / (100000000.0 + (unsigned char)passwords[ (index + 2) % passwords_Length ]);
};
// bytes decoding output
void OutPutDecode(WJLDeCoder *coder, int ucByte)
{
	coder->decodeOutBufferArray[coder->decodeOutBufferLoop] = (unsigned char)ucByte;
	coder->decodeOutBufferLoop = coder->decodeOutBufferLoop + 1;
}
// When coding, calculate frequency
void DeGetFreq(WJLDeCoder *coder)
{
	int i = 0;
	unsigned int Sum = 0;
	if(coder->Result == 0){
		coder->CumFreq = 0;
		coder->Freq = coder->SymbolsFreqArray[0];
		return;
	}
	// get the cumulative distribution function
	for(i = 0; i < coder->Result; ++i) {
		Sum += coder->SymbolsFreqArray[i];
	}
	coder->Freq = coder->SymbolsFreqArray[coder->Result];
	coder->CumFreq = Sum;
}
// Get Distribution Function
void GetDistributionFunction(WJLDeCoder *coder)
{
	int i = 0;
	unsigned int Sum = 0;
	unsigned int result = 0;
	unsigned int Code =coder->values >> 1;
	if(coder->encodeFlowRange == 0){
		return;
	}
	coder->encodeFlowRange = coder->encodeFlowRange / coder->Total;
	coder->encodeFlowRange = (unsigned int)((double)coder->encodeFlowRange * coder->coefficient);

	if (Code < coder->encodeFlow) {
		result = (Code + coder->rangeCodeMaxIterval - coder->encodeFlow) / coder->encodeFlowRange;
	} else {
		result = (Code - coder->encodeFlow) / coder->encodeFlowRange;
	}
	if(result < coder->SymbolsFreqArray[0]) {
		coder->Result = 0x00;
		return;
	}
	// find the symbol
	for(i = 0; i < coder->symbolsMaxCount; ++i) {
		Sum += coder->SymbolsFreqArray[i];
		if(result >= Sum && result < ( Sum + coder->SymbolsFreqArray[i + 1] ) ) {
			coder->Result = i + 1;
			return;
		}
	}
}
// Cumulative and reduced frequency
void DeIncFreq(WJLDeCoder *coder)
{
	int i = 0;
	unsigned int NewFreq = 0, Sum = 0;
	// Synchronously subtracts the statistics of each symbol
	if(coder->Total == coder->counterMaxFreq){
		for(i = 0; i <= coder->symbolsMaxCount; ++i) {
			NewFreq = coder->SymbolsFreqArray[i] >> 1;
			if(NewFreq == 0) {
				coder->SymbolsFreqArray[i] = 1;
			}else{
				coder->SymbolsFreqArray[i] = NewFreq;
			}
			Sum += coder->SymbolsFreqArray[i];
		}
		coder->Total = Sum;
	}
	// this symbol freq Add 1
	coder->SymbolsFreqArray[coder->Result] ++;
	// All symbol freq Add 1
	coder->Total += 1;
}
// update Range
void DeDecode(WJLDeCoder *coder, unsigned char *InBytesArray, unsigned int InBytesArray_Length)
{
	coder->encodeFlow += coder->CumFreq * coder->encodeFlowRange;
	coder->encodeFlowRange *= coder->Freq;
	while (coder->encodeFlowRange <=  coder->rangeCodeMinIterval){
		coder->encodeFlow = (coder->encodeFlow << 8) & (coder->rangeCodeMaxIterval - 1);
		coder->encodeFlowRange = coder->encodeFlowRange << 8;
		if(coder->decodeInBufferLoop < InBytesArray_Length){
			coder->values = (coder->values << 8) | InBytesArray[coder->decodeInBufferLoop] ^ coder->Sbox[ coder->decodeInBufferLoop % coder->symbolsMaxCount ];
			coder->decodeInBufferLoop = coder->decodeInBufferLoop + 1;
		}else{
			return;
		}
	}
}
// decoded a symbol
void decryptingasymbol(WJLDeCoder *coder, unsigned char *InBytesArray, unsigned int InBytesArray_Length)
{
	GetDistributionFunction(coder);
	DeGetFreq(coder);
	DeDecode(coder, InBytesArray, InBytesArray_Length);
	DeIncFreq(coder);
}
// decoded the RandomBytes by InByteArray
int decryptingRandomBytes(WJLDeCoder *coder, unsigned char *InBytesArray, unsigned int InBytesArray_Length, const char *passwords, int passwords_Length)
{
	int i = 0;
	// decoding Random Bytes
	for(i = 0; i < coder->RandomBytes_Length; ++i){
		decryptingasymbol(coder, InBytesArray, InBytesArray_Length);
		coder->coefficient = getDeCoefficient(coder, i, (unsigned char)coder->Result, passwords, passwords_Length);
		if(coder->Result == coder->symbolsMaxCount){
			return 0;
		}
	}
	return 1;
}
// decrypting the InBytesArray length
unsigned int decryptingInArraylength(WJLDeCoder *coder, unsigned char *InBytesArray, unsigned int InBytesArray_Length, const char *passwords, int passwords_Length)
{
	unsigned int byteslength = 0x00;
	int i = 0;
	// decoding
	for(i = 0; i < 4; ++i){
		decryptingasymbol(coder, InBytesArray, InBytesArray_Length);
		byteslength = byteslength << 8;
		byteslength = byteslength | (unsigned char)coder->Result;
		coder->coefficient = getDeCoefficient(coder, coder->RandomBytes_Length + i + 1, (unsigned char)coder->Result, passwords, passwords_Length);
		if(coder->Result == coder->symbolsMaxCount){
			return 0;
		}
	}
	return byteslength;
}
// decrypting the Datas
int WJLWeightedDecode(WJLDeCoder *coder, unsigned char *InByteArray, unsigned int InByteArray_length, const char *passwords, int passwords_Length, unsigned int byteslength, unsigned int *OutByteArray_length)
{
	int i = 0;
	// start decoding InByteArray
	for(i = 0; i < byteslength ; ++ i){
		decryptingasymbol(coder, InByteArray, InByteArray_length);
		OutPutDecode(coder, coder->Result);
		coder->coefficient = getDeCoefficient(coder, coder->RandomBytes_Length + 4 + i, (unsigned char)coder->Result, passwords, passwords_Length);
		if(coder->Result == coder->symbolsMaxCount){
			return 0;
		}
	}
	*OutByteArray_length = coder->decodeOutBufferLoop;
	coder->decodeOutBufferLoop = 0;
	return 1;
}
