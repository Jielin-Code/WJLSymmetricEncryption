#ifndef _WJLSD_h
#define _WJLSD_h
#include <stdio.h>
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
	// the Bit tree
	unsigned int *SymbolsFreqArray;
	// Array subscript pointer by In_buff
	unsigned int decodeInBufferLoop;
	// Array subscript pointer by EOut_buff EOut_buff_loop
	unsigned int decodeOutBufferLoop;
	// the Encoded cache array EOut_buff
	unsigned char *decodeOutBufferArray;
	// the S box
	unsigned char *Sbox;
	unsigned int RandomBytes_Length;
	// the Operational variables
	unsigned int Total;
	unsigned int CumFreq;
	unsigned int Freq;
	unsigned int values;
	// the Decode Result
	int Result;
}WJLDeCoder;

void InitializeWJLDeCoder(WJLDeCoder *coder, unsigned char *InByteArray, unsigned int InByteArray_length, const char *passwords, int passwords_Length);
int decryptingRandomBytes(WJLDeCoder *coder, unsigned char *InBytesArray, unsigned int InBytesArray_Length, const char *passwords, int passwords_Length);
unsigned int decryptingInArraylength(WJLDeCoder *coder, unsigned char *InBytesArray, unsigned int InBytesArray_Length, const char *passwords, int passwords_Length);
int WJLWeightedDecode(WJLDeCoder *coder, unsigned char *InByteArray, unsigned int InByteArray_length, const char *passwords, int passwords_Length, unsigned int byteslength, unsigned int *OutByteArray_length);
#endif
