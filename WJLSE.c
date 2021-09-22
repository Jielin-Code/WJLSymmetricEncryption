#include "WJLSE.h"
#include "WJLHA3.h"
// Random - generated byte 2D table,Customizable tables
unsigned char SERandomOfUCharTable[256] =	{
	226,134,150,101,199,177,118,188,179,220,74,27,95,216,14,166,
	23,81,214,203,151,41,224,31,141,108,56,92,135,120,196,9,
	178,162,110,146,76,3,79,217,206,208,87,94,246,41,147,112,
	119,217,75,34,193,94,147,238,174,5,104,130,236,219,117,58,
	246,200,185,153,22,8,184,34,96,14,165,158,236,169,251,88,
	123,45,60,68,141,71,29,164,177,32,102,111,236,96,182,13,
	128,172,9,20,33,73,75,232,125,137,150,65,105,232,158,250,
	62,190,168,176,65,159,18,33,111,196,152,116,241,4,253,43,
	197,25,52,130,77,92,95,231,147,87,2,63,168,226,91,123,
	40,15,178,77,50,74,93,141,65,217,184,59,44,23,232,235,
	59,112,27,214,194,178,79,149,203,193,251,64,116,91,237,102,
	99,85,119,12,65,72,170,209,141,96,190,254,38,182,184,89,
	240,114,162,60,145,163,252,178,186,155,235,240,207,119,137,208,
	151,92,48,210,165,220,6,60,145,225,14,4,153,136,7,108,
	105,68,240,32,149,185,156,28,74,211,208,154,235,158,171,187,
	232,176,146,167,233,251,202,120,107,41,232,78,124,188,186,143
};
// Initialize structure
void InitializeWJLEnCoder(WJLEnCoder *coder, unsigned int InByteArray_length)
{
	int i = 0;
	coder->rangeCodeBitsLenght = 31;
	coder->rangeCodeShiftBitsLenght = coder->rangeCodeBitsLenght - 8;
	coder->rangeCodeMaxIterval = 1 << coder->rangeCodeBitsLenght;
	coder->rangeCodeMinIterval = 1 << coder->rangeCodeShiftBitsLenght;
	coder->counterMaxFreq = 0x00FFFFL;
	coder->symbolsMaxCount = 256;
	coder->coefficient = 1.0;
	coder->encodeFlow = coder->rangeCodeMaxIterval;
	coder->encodeFlowRange = coder->rangeCodeMaxIterval;
	coder->encodeDelay = 0;
	coder->encodeDelayCount = 0;
	coder->encodeOutBufferLoop = 0;
	coder->decodeInBufferLoop = 0;
	coder->decodeOutBufferLoop = 0;
	coder->Total = 0;
	coder->CumFreq = 0;
	coder->Freq = 0;
	coder->values = 0;
	coder->Result = 0;
	coder->SymbolsFreqArray = (unsigned int *)malloc(sizeof(unsigned int) * (coder->symbolsMaxCount + 1));
	for(i = 0; i <= coder->symbolsMaxCount; ++i){
		coder->SymbolsFreqArray[i] = 1;
	}
	// Total is the Sum of all symbol's freq
	coder->Total = coder->symbolsMaxCount + 1;
	// Initialize the OutBufferArray
	coder->encodeOutBufferArray = (unsigned char *)malloc((double)InByteArray_length*1.2);
}
// Query 'SERandomOfUCharTable' table function
double getEnCoefficient(int index, unsigned char Symbol, unsigned char OneKeytByte)
{
	return 1.0 - (double)(SERandomOfUCharTable[ (index + Symbol) % 256 ] ^  OneKeytByte) / (100000.0 + OneKeytByte);
};
// Gets the Random Bytes
void getEnRandomBytes(unsigned char *RandomBuFF, int RandomBuFF_Length)
{
	int i = 0;
	srand((int)time(0));
	for(i = 0;i < RandomBuFF_Length; ++i){
		RandomBuFF[i] = (unsigned char)random(255);
	}
}
// bytes encoding output
void OutPutEncode(WJLEnCoder *coder, int ucByte)
{
	coder->encodeOutBufferArray[coder->encodeOutBufferLoop] = (unsigned char)ucByte;
	coder->encodeOutBufferLoop += 1;
}
// When coding, calculate frequency
void EnGetFreq(WJLEnCoder *coder, int Symbol)
{
	int i = 0;
	unsigned int Sum = 0;
	if(Symbol == 0){
		coder->CumFreq = 0;
		coder->Freq = coder->SymbolsFreqArray[0];
		return;
	}
	// get the cumulative distribution function
	for(i = 0; i < Symbol; ++i) {
		Sum += coder->SymbolsFreqArray[i];
	}
	coder->Freq = coder->SymbolsFreqArray[Symbol];
	coder->CumFreq = Sum;
}
// Cumulative and reduced frequency
void EnIncFreq(WJLEnCoder *coder, int Symbol)
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
	coder->SymbolsFreqArray[Symbol] ++;
	// All symbol freq Add 1
	coder->Total += 1;
}
// Encode by Adaptive weighted probability model
void SEEncode(WJLEnCoder *coder)
{
	unsigned int High;
	int i = 0;
	// Value of distribution function for weighted probability model
	coder->encodeFlowRange = coder->encodeFlowRange / coder->Total;
	coder->encodeFlowRange =(unsigned int)((double)coder->encodeFlowRange *  coder->coefficient); 
	coder->encodeFlow += coder->CumFreq * coder->encodeFlowRange;
	coder->encodeFlowRange *= coder->Freq;

	while (coder->encodeFlowRange <= coder->rangeCodeMinIterval) {
		High = coder->encodeFlow + coder->encodeFlowRange - 1;
		if (coder->encodeDelayCount != 0) {
			if (High <= coder->rangeCodeMaxIterval) {
				OutPutEncode(coder, coder->encodeDelay);
				for (i = 1; i <= coder->encodeDelayCount - 1; ++i) {
					OutPutEncode(coder, 0xFF);
				}
				coder->encodeDelayCount = 0;
				coder->encodeFlow += coder->rangeCodeMaxIterval;
			} else if (coder->encodeFlow >= coder->rangeCodeMaxIterval) {
				OutPutEncode(coder, coder->encodeDelay + 1);
				for (i = 1; i <= coder->encodeDelayCount - 1; ++i) {
					OutPutEncode(coder, 0x00);
				}
				coder->encodeDelayCount = 0;
			} else {
				coder->encodeDelayCount ++;
				coder->encodeFlow = (coder->encodeFlow << 8) & (coder->rangeCodeMaxIterval - 1);
				coder->encodeFlowRange <<= 8;
				continue;
			}
		}
		if (((coder->encodeFlow ^ High) & (0x00FF << coder->rangeCodeShiftBitsLenght)) == 0) {
			OutPutEncode(coder, (int) (coder->encodeFlow >> coder->rangeCodeShiftBitsLenght));
		} else {
			coder->encodeFlow -= coder->rangeCodeMaxIterval;
			coder->encodeDelay = coder->encodeFlow >> coder->rangeCodeShiftBitsLenght;
			coder->encodeDelayCount = 1;
		}
		coder->encodeFlow = ((coder->encodeFlow << 8) & (coder->rangeCodeMaxIterval - 1))
			| (coder->encodeFlow & coder->rangeCodeMaxIterval);
		coder->encodeFlowRange <<= 8;
	}
}
//To Finish Encode
void SEFinishEncode(WJLEnCoder *coder)
{
	int n = 0;
	if (coder->encodeDelayCount != 0) {
		if (coder->encodeFlow < coder->rangeCodeMaxIterval) {
			OutPutEncode(coder, coder->encodeDelay);
			for (n = 1; n <= coder->encodeDelayCount - 1; n++) {
				OutPutEncode(coder, 0xFF);
			}
		} else {
			OutPutEncode(coder, coder->encodeDelay + 1);
			for (n = 1; n <= coder->encodeDelayCount - 1; n++) {
				OutPutEncode(coder, 0x00);
			}
		}
	}
	coder->encodeFlow <<= 1;
	n = coder->rangeCodeBitsLenght + 1;
	do {
		n -= 8;
		OutPutEncode(coder, (int) (coder->encodeFlow >> n));
	} while ( n > 0 );
}
// encrypting a symbol
void encryptingASymbol(WJLEnCoder *coder,int symbol)
{
	EnGetFreq(coder, symbol);
	SEEncode(coder);
	EnIncFreq(coder, symbol);
}
// encrypting the end symbol
void EncryptingTheEndSymbol(WJLEnCoder *coder)
{
	encryptingASymbol(coder, coder->symbolsMaxCount);
	SEFinishEncode(coder);
}
// encrypting the Random byte 
void EncryptingTheRandomBytes(WJLEnCoder *coder, const char *passwords, int passwords_Length, int RandomBuFF_Length)
{
	int i = 0, symbol = 0;
	unsigned char *RandomBuFF;
	unsigned char *keytBuFF;
	keytBuFF = (unsigned char *)malloc(RandomBuFF_Length);
	RandomBuFF = (unsigned char *)malloc(RandomBuFF_Length);
	WJLHA3((unsigned char *)passwords, passwords_Length, SERandomOfUCharTable, 256, keytBuFF, RandomBuFF_Length);
	getEnRandomBytes(RandomBuFF, RandomBuFF_Length);
	for(i = 0; i < RandomBuFF_Length; ++ i) {
		coder->coefficient = getEnCoefficient(i, (keytBuFF[i % RandomBuFF_Length] + i ) % 0xFF, keytBuFF[ i % RandomBuFF_Length ]);
		symbol = (int)RandomBuFF[i];
		encryptingASymbol(coder, symbol);
	}
	free(RandomBuFF);
	free(keytBuFF);
}
// encrypting the hash value
void EncryptingTheHashValue(WJLEnCoder *coder, const char *passwords, int passwords_Length, int HashValueBuFF_Length)
{
	int i = 0, symbol = 0;
	unsigned char *HashValueBuFF;
	HashValueBuFF = (unsigned char *)malloc(HashValueBuFF_Length);
	// Use the hash value length of 512bit = 64Byte
	WJLHA3((unsigned char *)passwords, passwords_Length, SERandomOfUCharTable, 256, HashValueBuFF, HashValueBuFF_Length);
	// Encoding 64 bytes all in the hashvalue
	for(i = 0; i < HashValueBuFF_Length; ++ i) {
		symbol = (int)HashValueBuFF[i];
		encryptingASymbol(coder, symbol);
	}
	free(HashValueBuFF);
}
// encrypting the length
void EncryptingFileLength(WJLEnCoder *coder, unsigned int InArrayLength)
{
	int symbol = 0;
	symbol = (int)((InArrayLength >> 24) & 0x00FF);
	encryptingASymbol(coder, symbol);
	symbol = (int)((InArrayLength >> 16) & 0x00FF);
	encryptingASymbol(coder, symbol);
	symbol = (int)((InArrayLength >> 8) & 0x00FF);
	encryptingASymbol(coder, symbol);
	symbol = (int)(InArrayLength & 0x00FF);
	encryptingASymbol(coder, symbol);
}
unsigned char* WJLWeightedEncode(WJLEnCoder *coder, const char *passwords, int passwords_Length, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length)
{
	int i = 0;
	unsigned char *keytBuFF;
	int keytBuFF_Length = InByteArray_length / 4;
	keytBuFF = (unsigned char *)malloc(keytBuFF_Length);
	// The Secret Key length is greater than the data length
	WJLHA3((unsigned char *)passwords, passwords_Length, SERandomOfUCharTable, 256, keytBuFF, keytBuFF_Length);
	// start encoding InByteArray
	for(i = 0; i < InByteArray_length ; ++ i){
		encryptingASymbol(coder, (int)InByteArray[i]);
		coder->coefficient = getEnCoefficient(i, InByteArray[i] ^ keytBuFF[ keytBuFF_Length - i % keytBuFF_Length - 1 ], keytBuFF[ i % keytBuFF_Length ]);
	}
	// End Encrypting
	EncryptingTheEndSymbol(coder);
	// return
	*OutByteArray_length = coder->encodeOutBufferLoop;
	coder->encodeOutBufferLoop = 0;
	free(coder->SymbolsFreqArray);
	return coder->encodeOutBufferArray;
}
