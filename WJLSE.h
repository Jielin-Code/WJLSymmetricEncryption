#ifndef _WJLSE_h
#define _WJLSE_h
#include "time.h"
#define random(x) (rand() % x)
// Jielincode Encoding Struct
typedef struct
{
	// Use 32bit Variable and not use the signed, so 31bit of int
	int rangeCodeBitsLenght;
	// 31 - 8 = 23, Subtract 8 because need to leave one byte space.
	int rangeCodeShiftBitsLenght;
	// Maximum value of interval.
	unsigned int rangeCodeMaxIterval;
	// Minimum value of interval, Maximum value is one byte larger than the minimum.
	unsigned int rangeCodeMinIterval;
	// Maximum value of Counter.
	unsigned int counterMaxFreq;
	// Maximum value of Symbols.
	int symbolsMaxCount;
	// JieLin Code Coefficient.
	double coefficient;
	// interval subscript EFLow.
	unsigned int encodeFlow;
	// interval length EFRange.
	unsigned int encodeFlowRange;
	// Delayed value output EFDigits.
	int encodeDelay;
	// the Delayed value count EFFollow.
	int encodeDelayCount;
	// Array subscript pointer by EOut_buff EOut_buff_loop
	unsigned int encodeOutBufferLoop;
	// the Encoded cache array EOut_buff
	unsigned char *encodeOutBufferArray;
	// the Bit tree
	unsigned int *SymbolsFreqArray;
	// Array subscript pointer by In_buff
	unsigned int decodeInBufferLoop;
	// Array subscript pointer by EOut_buff EOut_buff_loop
	unsigned int decodeOutBufferLoop;
	// the Encoded cache array EOut_buff
	unsigned char *decodeOutBufferArray;
	// the Operational variables
	unsigned int Total;
	unsigned int CumFreq;
	unsigned int Freq;
	unsigned int values;
	// the Decode Result
	int Result;
}WJLEnCoder;

void InitializeWJLEnCoder(WJLEnCoder *coder, unsigned int InByteArray_length);
void EncryptingTheRandomBytes(WJLEnCoder *coder, const char *passwords, int passwords_Length, int RandomBuFF_Length);
void EncryptingTheHashValue(WJLEnCoder *coder, const char *passwords, int passwords_Length, int HashValueBuFF_Length);
void EncryptingFileLength(WJLEnCoder *coder, unsigned int InArrayLength);
unsigned char* WJLWeightedEncode(WJLEnCoder *coder,const char *passwords, int passwords_Length, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length);

#endif