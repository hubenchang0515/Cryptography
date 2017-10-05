/* blog.kurukurumi.com */
#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct Md5State
{
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
}Md5State;

uint32_t rotateLeft(uint32_t n, uint8_t bits);
uint32_t rotateRight(uint32_t n, uint8_t bits);

void md5Init(Md5State* state);
void md5Count(Md5State* state, void* data);
void md5Tail(Md5State* state, void* data, uint8_t currentBytes, uint64_t totalBytes);
void md5Result(Md5State* state, char* result);

typedef int (*Md5Callback)(void* param, void* data);
void md5(Md5Callback callback, void* param, char* result);

#ifdef __cplusplus
	}
#endif

#endif
