/**************************************************************************
 * Based on Weighted Probability Model Code(Jielin Code)
 * @author JieLin Wang(China)
 * @copyright JieLin Wang 2021-09-28
 * @Version 3.1.0
 * @email 254908447@qq.com
 */
#ifndef _WJLSE3_h
#define _WJLSE3_h

#include "stdlib.h"
#include "memory.h"
/*************************************************************************************
PasswordBuff:User password, type of string.
InByteArray: byte cache address waiting for encryption.
InByteArray_length: the InByteArray length.
OutByteArray_length: the outbuff length.
return *OutByteArray.
*************************************************************************************/
unsigned char* encrypt(const char *PasswordBuff, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length, unsigned int RandomBytes_Length);

/*************************************************************************************
PasswordBuff:User password, type of string.
InByteArray: byte cache address waiting for encryption.
InByteArray_length: the InByteArray length.
OutByteArray_length: the outbuff length.
return *OutByteArray.
*************************************************************************************/
unsigned char* decrypt(const char *PasswordBuff, unsigned char *InByteArray, unsigned int InByteArray_length, unsigned int *OutByteArray_length, unsigned int RandomBytes_Length);

#endif
