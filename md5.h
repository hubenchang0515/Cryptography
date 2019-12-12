/* blog.kurukurumi.com */
#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

/* Old Interface */
typedef struct Md5State
{
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
}Md5State;

void md5Init(Md5State* state);
void md5Count(Md5State* state, const void* data);
void md5Tail(Md5State* state, void* data, uint8_t currentBytes, uint64_t totalBytes);
void md5Result(Md5State* state, char* result);

void md5(const void* data,size_t len, char* result);
typedef int (*Md5Callback)(void* userdata, size_t length, void* data);
void md5Universal(Md5Callback callback, void* userdata, char* result);


/* New Interface */
typedef struct Md5
{
	Md5State state;
	
	char hex[33];                // md5 result hex sum
	uint8_t buffer[64];          // buffer
	size_t used;                 // length of buffer used
	size_t length;               // length of total data
}Md5;

void md5Reset(Md5* md5);
void md5Update(Md5* md5, const void* data, size_t length);
const char* md5Hex(Md5* md5);
const char* md5OfData(Md5* md5, const void* data, size_t length);
const char* md5OfString(Md5* md5, const char* str);

#ifdef __cplusplus
	}
#endif

#endif
