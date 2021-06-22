﻿#include "WJLHA3.h"
extern const unsigned char RandomOfUCharTable[256];
extern const unsigned char RandomOfbitTable[256];
extern const unsigned char bitOfByteTable[256][8];
extern const unsigned int Ps = 256;// Customizable
// Jielincode Encoding Struct
typedef struct
{
	unsigned int RC_CODE_BITS;
	unsigned int RC_SHIFT_BITS;
	unsigned int RC_MAX_RANGE;
	unsigned int RC_MIN_RANGE;
	double p0;
	double p1;
	double JIELINCOE;
	unsigned int EFLow;
	unsigned int EFRange;
	unsigned int EFDigits;
	unsigned int EFFollow;
	unsigned int EOut_buff_loop;
	unsigned char *HashValueBuFF;
	int HashValueBuFF_Length;
}WJLCoder;
// Random - generated byte 2D table,Customizable tables
const unsigned char RandomOfUCharTable[256] =	{
	143,254,212,99,131,164,146,48,66,137,58,219,171,251,193,50,
	94,43,86,32,68,48,217,36,242,56,183,150,73,48,82,213,
	143,12,156,62,127,28,182,213,23,235,49,190,164,108,121,92,
	178,59,73,121,131,85,96,153,126,112,204,252,183,237,213,249,
	220,75,88,129,35,170,182,14,162,35,179,67,94,67,132,38,
	122,96,58,232,187,17,3,218,35,105,75,82,137,114,160,2,
	140,157,84,106,186,209,79,221,213,41,37,228,47,74,141,115,
	189,203,124,9,33,28,206,90,7,206,30,230,136,89,149,128,
	221,216,214,146,133,135,208,157,79,85,45,84,1,23,95,97,
	62,148,184,231,37,249,204,152,146,227,97,35,113,170,167,197,
	107,63,233,178,99,118,249,249,155,99,152,204,23,223,214,222,
	199,86,226,230,84,90,249,64,214,173,94,242,209,202,229,147,
	146,55,115,158,202,125,19,58,180,187,127,151,24,135,11,125,
	186,2,102,215,97,187,126,146,39,91,208,115,54,174,197,18,
	41,69,133,125,6,111,70,193,192,249,161,224,78,238,178,138,
	218,250,158,88,134,71,46,8,248,233,103,224,141,74,174,5
};
// Random - generated bit 2D table, Customizable tables
const unsigned char RandomOfbitTable[256] =	{
	1,1,0,1,0,1,1,0,1,0,0,0,0,1,0,0,
	1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,1,
	0,1,1,1,0,1,1,0,1,0,0,1,0,1,0,0,
	1,0,0,1,0,0,1,0,1,1,1,0,1,1,0,1,
	1,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,
	0,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,
	1,0,1,1,1,1,0,1,0,0,0,1,1,0,0,1,
	1,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,
	0,1,1,1,1,1,0,0,1,0,1,0,1,1,0,0,
	0,0,0,0,1,1,0,1,0,0,1,0,0,0,1,1,
	0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,
	1,1,1,0,0,0,0,0,1,0,1,1,0,0,0,1,
	0,0,1,1,1,0,1,1,0,1,1,1,0,1,0,1,
	0,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1
};
// Bit value at each position in each byte
const unsigned char bitOfByteTable[256][8] = {
	{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,1,0},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,0,0},{0,0,0,0,0,1,0,1},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,1},	//0~7
	{0,0,0,0,1,0,0,0},{0,0,0,0,1,0,0,1},{0,0,0,0,1,0,1,0},{0,0,0,0,1,0,1,1},{0,0,0,0,1,1,0,0},{0,0,0,0,1,1,0,1},{0,0,0,0,1,1,1,0},{0,0,0,0,1,1,1,1},	//8~15	
	{0,0,0,1,0,0,0,0},{0,0,0,1,0,0,0,1},{0,0,0,1,0,0,1,0},{0,0,0,1,0,0,1,1},{0,0,0,1,0,1,0,0},{0,0,0,1,0,1,0,1},{0,0,0,1,0,1,1,0},{0,0,0,1,0,1,1,1},	//16~23
	{0,0,0,1,1,0,0,0},{0,0,0,1,1,0,0,1},{0,0,0,1,1,0,1,0},{0,0,0,1,1,0,1,1},{0,0,0,1,1,1,0,0},{0,0,0,1,1,1,0,1},{0,0,0,1,1,1,1,0},{0,0,0,1,1,1,1,1},	//24~31
	{0,0,1,0,0,0,0,0},{0,0,1,0,0,0,0,1},{0,0,1,0,0,0,1,0},{0,0,1,0,0,0,1,1},{0,0,1,0,0,1,0,0},{0,0,1,0,0,1,0,1},{0,0,1,0,0,1,1,0},{0,0,1,0,0,1,1,1},	//32~39
	{0,0,1,0,1,0,0,0},{0,0,1,0,1,0,0,1},{0,0,1,0,1,0,1,0},{0,0,1,0,1,0,1,1},{0,0,1,0,1,1,0,0},{0,0,1,0,1,1,0,1},{0,0,1,0,1,1,1,0},{0,0,1,0,1,1,1,1},	//40~47
	{0,0,1,1,0,0,0,0},{0,0,1,1,0,0,0,1},{0,0,1,1,0,0,1,0},{0,0,1,1,0,0,1,1},{0,0,1,1,0,1,0,0},{0,0,1,1,0,1,0,1},{0,0,1,1,0,1,1,0},{0,0,1,1,0,1,1,1},	//48~55
	{0,0,1,1,1,0,0,0},{0,0,1,1,1,0,0,1},{0,0,1,1,1,0,1,0},{0,0,1,1,1,0,1,1},{0,0,1,1,1,1,0,0},{0,0,1,1,1,1,0,1},{0,0,1,1,1,1,1,0},{0,0,1,1,1,1,1,1},	//56~63
	{0,1,0,0,0,0,0,0},{0,1,0,0,0,0,0,1},{0,1,0,0,0,0,1,0},{0,1,0,0,0,0,1,1},{0,1,0,0,0,1,0,0},{0,1,0,0,0,1,0,1},{0,1,0,0,0,1,1,0},{0,1,0,0,0,1,1,1},	//64~71
	{0,1,0,0,1,0,0,0},{0,1,0,0,1,0,0,1},{0,1,0,0,1,0,1,0},{0,1,0,0,1,0,1,1},{0,1,0,0,1,1,0,0},{0,1,0,0,1,1,0,1},{0,1,0,0,1,1,1,0},{0,1,0,0,1,1,1,1},	//72~79
	{0,1,0,1,0,0,0,0},{0,1,0,1,0,0,0,1},{0,1,0,1,0,0,1,0},{0,1,0,1,0,0,1,1},{0,1,0,1,0,1,0,0},{0,1,0,1,0,1,0,1},{0,1,0,1,0,1,1,0},{0,1,0,1,0,1,1,1},	//80~87
	{0,1,0,1,1,0,0,0},{0,1,0,1,1,0,0,1},{0,1,0,1,1,0,1,0},{0,1,0,1,1,0,1,1},{0,1,0,1,1,1,0,0},{0,1,0,1,1,1,0,1},{0,1,0,1,1,1,1,0},{0,1,0,1,1,1,1,1},	//88~95
	{0,1,1,0,0,0,0,0},{0,1,1,0,0,0,0,1},{0,1,1,0,0,0,1,0},{0,1,1,0,0,0,1,1},{0,1,1,0,0,1,0,0},{0,1,1,0,0,1,0,1},{0,1,1,0,0,1,1,0},{0,1,1,0,0,1,1,1},	//96~103
	{0,1,1,0,1,0,0,0},{0,1,1,0,1,0,0,1},{0,1,1,0,1,0,1,0},{0,1,1,0,1,0,1,1},{0,1,1,0,1,1,0,0},{0,1,1,0,1,1,0,1},{0,1,1,0,1,1,1,0},{0,1,1,0,1,1,1,1},	//104~111
	{0,1,1,1,0,0,0,0},{0,1,1,1,0,0,0,1},{0,1,1,1,0,0,1,0},{0,1,1,1,0,0,1,1},{0,1,1,1,0,1,0,0},{0,1,1,1,0,1,0,1},{0,1,1,1,0,1,1,0},{0,1,1,1,0,1,1,1},	//112~119
	{0,1,1,1,1,0,0,0},{0,1,1,1,1,0,0,1},{0,1,1,1,1,0,1,0},{0,1,1,1,1,0,1,1},{0,1,1,1,1,1,0,0},{0,1,1,1,1,1,0,1},{0,1,1,1,1,1,1,0},{0,1,1,1,1,1,1,1},	//120~127
	{1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,1},{1,0,0,0,0,0,1,0},{1,0,0,0,0,0,1,1},{1,0,0,0,0,1,0,0},{1,0,0,0,0,1,0,1},{1,0,0,0,0,1,1,0},{1,0,0,0,0,1,1,1},	//128~135
	{1,0,0,0,1,0,0,0},{1,0,0,0,1,0,0,1},{1,0,0,0,1,0,1,0},{1,0,0,0,1,0,1,1},{1,0,0,0,1,1,0,0},{1,0,0,0,1,1,0,1},{1,0,0,0,1,1,1,0},{1,0,0,0,1,1,1,1},	//136~143
	{1,0,0,1,0,0,0,0},{1,0,0,1,0,0,0,1},{1,0,0,1,0,0,1,0},{1,0,0,1,0,0,1,1},{1,0,0,1,0,1,0,0},{1,0,0,1,0,1,0,1},{1,0,0,1,0,1,1,0},{1,0,0,1,0,1,1,1},	//144~151
	{1,0,0,1,1,0,0,0},{1,0,0,1,1,0,0,1},{1,0,0,1,1,0,1,0},{1,0,0,1,1,0,1,1},{1,0,0,1,1,1,0,0},{1,0,0,1,1,1,0,1},{1,0,0,1,1,1,1,0},{1,0,0,1,1,1,1,1},	//152~159
	{1,0,1,0,0,0,0,0},{1,0,1,0,0,0,0,1},{1,0,1,0,0,0,1,0},{1,0,1,0,0,0,1,1},{1,0,1,0,0,1,0,0},{1,0,1,0,0,1,0,1},{1,0,1,0,0,1,1,0},{1,0,1,0,0,1,1,1},	//160~167
	{1,0,1,0,1,0,0,0},{1,0,1,0,1,0,0,1},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,1},{1,0,1,0,1,1,0,0},{1,0,1,0,1,1,0,1},{1,0,1,0,1,1,1,0},{1,0,1,0,1,1,1,1},	//168~175
	{1,0,1,1,0,0,0,0},{1,0,1,1,0,0,0,1},{1,0,1,1,0,0,1,0},{1,0,1,1,0,0,1,1},{1,0,1,1,0,1,0,0},{1,0,1,1,0,1,0,1},{1,0,1,1,0,1,1,0},{1,0,1,1,0,1,1,1},	//176~183
	{1,0,1,1,1,0,0,0},{1,0,1,1,1,0,0,1},{1,0,1,1,1,0,1,0},{1,0,1,1,1,0,1,1},{1,0,1,1,1,1,0,0},{1,0,1,1,1,1,0,1},{1,0,1,1,1,1,1,0},{1,0,1,1,1,1,1,1},	//184~191
	{1,1,0,0,0,0,0,0},{1,1,0,0,0,0,0,1},{1,1,0,0,0,0,1,0},{1,1,0,0,0,0,1,1},{1,1,0,0,0,1,0,0},{1,1,0,0,0,1,0,1},{1,1,0,0,0,1,1,0},{1,1,0,0,0,1,1,1},	//192~199
	{1,1,0,0,1,0,0,0},{1,1,0,0,1,0,0,1},{1,1,0,0,1,0,1,0},{1,1,0,0,1,0,1,1},{1,1,0,0,1,1,0,0},{1,1,0,0,1,1,0,1},{1,1,0,0,1,1,1,0},{1,1,0,0,1,1,1,1},	//200~207
	{1,1,0,1,0,0,0,0},{1,1,0,1,0,0,0,1},{1,1,0,1,0,0,1,0},{1,1,0,1,0,0,1,1},{1,1,0,1,0,1,0,0},{1,1,0,1,0,1,0,1},{1,1,0,1,0,1,1,0},{1,1,0,1,0,1,1,1},	//208~215
	{1,1,0,1,1,0,0,0},{1,1,0,1,1,0,0,1},{1,1,0,1,1,0,1,0},{1,1,0,1,1,0,1,1},{1,1,0,1,1,1,0,0},{1,1,0,1,1,1,0,1},{1,1,0,1,1,1,1,0},{1,1,0,1,1,1,1,1},	//216~223
	{1,1,1,0,0,0,0,0},{1,1,1,0,0,0,0,1},{1,1,1,0,0,0,1,0},{1,1,1,0,0,0,1,1},{1,1,1,0,0,1,0,0},{1,1,1,0,0,1,0,1},{1,1,1,0,0,1,1,0},{1,1,1,0,0,1,1,1},	//224~231
	{1,1,1,0,1,0,0,0},{1,1,1,0,1,0,0,1},{1,1,1,0,1,0,1,0},{1,1,1,0,1,0,1,1},{1,1,1,0,1,1,0,0},{1,1,1,0,1,1,0,1},{1,1,1,0,1,1,1,0},{1,1,1,0,1,1,1,1},	//232~239
	{1,1,1,1,0,0,0,0},{1,1,1,1,0,0,0,1},{1,1,1,1,0,0,1,0},{1,1,1,1,0,0,1,1},{1,1,1,1,0,1,0,0},{1,1,1,1,0,1,0,1},{1,1,1,1,0,1,1,0},{1,1,1,1,0,1,1,1},	//240~247
	{1,1,1,1,1,0,0,0},{1,1,1,1,1,0,0,1},{1,1,1,1,1,0,1,0},{1,1,1,1,1,0,1,1},{1,1,1,1,1,1,0,0},{1,1,1,1,1,1,0,1},{1,1,1,1,1,1,1,0},{1,1,1,1,1,1,1,1}		//248~255
};
// dissection function,Cut the data into ⌈InBytesBuFF_Length/256⌉ segments
int DissectionTheDatas(int InBytesBuFF_Length)
{
	int paragraphs = InBytesBuFF_Length / Ps;
	if(InBytesBuFF_Length > paragraphs * Ps){
		return paragraphs + 1;
	}else{
		return paragraphs;
	}
}
// Query 'RandomOfbitTable' table function
unsigned char QuerybitTable(int index, unsigned char bitSymbol)
{
	return RandomOfbitTable[ (index + bitSymbol) % 256 ] ^ bitSymbol;
};
// Query 'RandomOfUCharTable' table function
double QueryUCharTable(int index, unsigned char bitSymbol, double r, unsigned char OneKeytByte)
{
	return r - (double)RandomOfUCharTable[ (index + bitSymbol) % 256 ]/(100000.0 + OneKeytByte);
};
// Obtain a sequence Y and obtain a weighted coefficient---JIELINCODE Coefficient
double TheCOEofY(WJLCoder *coder, int partIndex, int paragraphs, unsigned char *Y, int Y_len){
	unsigned char tmpBit, tmpByte = 0;
	int i, j, Count0 = 0;
	double H = 0;
	// Number of statistical symbols 0
	for(i = 0; i < Y_len; ++i){
		tmpByte = 0x00;
		for(j = 0; j < 8; ++ j) {
			tmpByte <<= 1;
			tmpBit = QuerybitTable((i * 8 + j) % 256, bitOfByteTable[(int)Y[i]][j]);
			if(tmpBit == 0) {
				Count0 ++;
			}else{
				tmpByte |= tmpBit;
			}
		}
		Y[i] = tmpByte;
	}
	// Get the probability p0 of symbol 0 and the probability p1 of symbol 1
	coder->p0 = (double)Count0 / (double)(Y_len * 8.0);
	coder->p1 = 1.0 - coder->p0;
	// Binary sequences of all 0 or all 1 need to be preprocessed
	if(coder->p0 == 0.0){
		coder->p0 = 0.0000000001;
		coder->p1 = 1.0 - coder->p0;
	}else if(coder->p1 == 0.0){
		coder->p1 = 0.0000000001;
		coder->p0 = 1.0 - coder->p1;
	}
	// get the standard information entropy
	H = -coder->p0 * (log(coder->p0)/log(2.0))- coder->p1 * (log(coder->p1)/log(2.0));
	// get the Jielin code coefficient
	return pow( 2.0, H - ( ((double)coder->HashValueBuFF_Length) / (double)Y_len ));
}
// Output bytes to cache and weighted encoding
void OutPutByte(WJLCoder *coder, unsigned char ucByte)
{
	coder->HashValueBuFF[ coder->EOut_buff_loop % coder->HashValueBuFF_Length ] = coder->HashValueBuFF[ coder->EOut_buff_loop % coder->HashValueBuFF_Length ] ^ (unsigned char)ucByte;
	coder->EOut_buff_loop = coder->EOut_buff_loop + 1;
}
// Encode by JielinCeo
void Encode(WJLCoder *coder, unsigned char symbol)
{	
	unsigned int High = 0,i = 0;
	if (1 == symbol){// the Symbol 1
		coder->EFLow = coder->EFLow +  (unsigned int)((double)coder->EFRange * coder->p0);
		coder->EFRange = (unsigned int)((double)coder->EFRange * coder->p1 * coder->JIELINCOE);
	}else{
		coder->EFRange = (unsigned int)((double)coder->EFRange * coder->p0 * coder->JIELINCOE);
	}
	while(coder->EFRange <= coder->RC_MIN_RANGE){
		High = coder->EFLow + coder->EFRange - 1;
		if(coder->EFFollow != 0) {
			if (High <= coder->RC_MAX_RANGE) {
				OutPutByte(coder, coder->EFDigits);
				for (i = 1; i <= coder->EFFollow - 1; ++i){
					OutPutByte(coder, 0xFF);
				}
				coder->EFFollow = 0;
				coder->EFLow = coder->EFLow + coder->RC_MAX_RANGE;
			} else if (coder->EFLow >= coder->RC_MAX_RANGE) {
				OutPutByte(coder, coder->EFDigits + 1);
				for (i = 1; i <= coder->EFFollow - 1; ++i){
					OutPutByte(coder, 0x00);
				}									
				coder->EFFollow = 0;
			} else {
				coder->EFFollow += 1;
				coder->EFLow = (coder->EFLow << 8) & (coder->RC_MAX_RANGE - 1);
				coder->EFRange = coder->EFRange << 8;
				continue;
			}
		}
		if  (((coder->EFLow ^ High) & (0xFF << coder->RC_SHIFT_BITS)) == 0) {
			OutPutByte(coder, (unsigned char)(coder->EFLow >> coder->RC_SHIFT_BITS));
		}else{
			coder->EFLow = coder->EFLow - coder->RC_MAX_RANGE;
			coder->EFDigits = coder->EFLow >> coder->RC_SHIFT_BITS;
			coder->EFFollow = 1;
		}
		coder->EFLow = ( ( (coder->EFLow << 8) & (coder->RC_MAX_RANGE - 1) ) | (coder->EFLow & coder->RC_MAX_RANGE) );
		coder->EFRange = coder->EFRange << 8;
	}
}
// Finish Encode by JielinCeo
void FinishEncode(WJLCoder *coder)
{
	int n = 0;
	if (coder->EFFollow != 0) {
		if (coder->EFLow < coder->RC_MAX_RANGE) {
			OutPutByte(coder, coder->EFDigits);
			for (n = 1; n <= coder->EFFollow - 1; n++) {
				OutPutByte(coder, 0xFF);
			}
		} else {
			OutPutByte(coder, coder->EFDigits + 1);
			for (n = 1; n <= coder->EFFollow - 1; n++) {
				OutPutByte(coder, 0x00);
			}
		}
	}
	coder->EFLow = coder->EFLow << 1;
	n = coder->RC_CODE_BITS + 1;
	do {
		n -= 8;
		OutPutByte(coder, (unsigned char)(coder->EFLow >> n));
	} while (!(n <= 0));
}
// Initialization WJLCoder
void InitializationWJLCoder(WJLCoder *coder)
{
	coder->RC_CODE_BITS = 31;
	coder->RC_SHIFT_BITS = coder->RC_CODE_BITS - 8;
	coder->RC_MAX_RANGE = 1 << coder->RC_CODE_BITS;
	coder->RC_MIN_RANGE = 1 << coder->RC_SHIFT_BITS;
	coder->p0 = 0.0;
	coder->p1 = 0.0;
	coder->JIELINCOE = 0.0;
	coder->EFLow = coder->RC_MAX_RANGE;
	coder->EFRange = coder->RC_MAX_RANGE;
	coder->EFDigits = 0;
	coder->EFFollow = 0;
	coder->EOut_buff_loop = 0;
	coder->HashValueBuFF_Length = 0;
}
// the main function
void WJLHA3(unsigned char *InBytesBuFF, int InBytesBuFF_Length, unsigned char *keytBuFF, int keytBuFF_Length, unsigned char *HashValueBuFF, int HashValueBuFF_Length)
{
	unsigned char *Y, *tempInbuff;
	int Y_len = Ps, i = 0, j = 0;
	unsigned int k = 0;
	int partIndex = 0, paragraphs = 0;
	WJLCoder *coder;
	double coe = 0.0;
	if(HashValueBuFF == 0){
		HashValueBuFF = (unsigned char *)malloc(HashValueBuFF_Length);
	}
	// Initialization WJLCoder Object
	coder = (WJLCoder *)malloc(sizeof(WJLCoder));
	Y = (unsigned char *)malloc(Y_len * sizeof(unsigned char));
	paragraphs = DissectionTheDatas(InBytesBuFF_Length);
	InitializationWJLCoder(coder);
	coder->HashValueBuFF_Length = HashValueBuFF_Length;
	coder->HashValueBuFF  = (unsigned char *)malloc(HashValueBuFF_Length);

	if(InBytesBuFF_Length < 256){
		tempInbuff = (unsigned char *)malloc(256);
		memcpy(tempInbuff, InBytesBuFF, InBytesBuFF_Length);
		for(i = InBytesBuFF_Length; i < 256; ++i){
			tempInbuff[i] = (unsigned char)(RandomOfUCharTable[i] + (InBytesBuFF[i % InBytesBuFF_Length] << 1) | 0x01 ) ^ (InBytesBuFF[i % InBytesBuFF_Length] + RandomOfbitTable[255 - i] + RandomOfUCharTable[i]);
		}
		InBytesBuFF_Length = 256;
	}else{
		tempInbuff = InBytesBuFF;
	}

	if(keytBuFF_Length > InBytesBuFF_Length * 8){
		for(i = InBytesBuFF_Length * 8; i < keytBuFF_Length; ++i){
			keytBuFF[i % (InBytesBuFF_Length * 8)] = (unsigned char)(keytBuFF[i % (InBytesBuFF_Length * 8)] * keytBuFF[i]) ^ (keytBuFF[i] + keytBuFF[i % (InBytesBuFF_Length * 8)]);
		}
		keytBuFF_Length = InBytesBuFF_Length * 8;
	}

	// Segprocessing
	for(partIndex = 1; partIndex <= paragraphs; ++partIndex){
		if(partIndex == paragraphs){
			Y_len = InBytesBuFF_Length - (paragraphs - 1)* Ps;
		}
		memcpy(Y, tempInbuff + (partIndex - 1) * Ps, Y_len);
		// Get COE of Y
		coe = TheCOEofY(coder, partIndex, paragraphs, Y, Y_len);
		// Encode each bits
		for(i = 0; i < Y_len; ++i){
			for(j = 0; j < 8; ++j){
				coder->JIELINCOE = QueryUCharTable(i, bitOfByteTable[(int)Y[i]][j], coe, keytBuFF[ k % keytBuFF_Length ]);
				Encode(coder, bitOfByteTable[(int)Y[i]][j]);
				k ++;
			}
		}
	}
	FinishEncode(coder);
	memcpy(HashValueBuFF, coder->HashValueBuFF, HashValueBuFF_Length);
	// Free memory
	free(Y);
	free(tempInbuff);
	free(coder->HashValueBuFF);
	free(coder);
}