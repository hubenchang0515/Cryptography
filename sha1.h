#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct Sha1State
{
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
	uint32_t E;
}Sha1State; 


void sha1Init(Sha1State* state);
void sha1Count(Sha1State* state, const void* data);
void sha1Tail(Sha1State* state,void* data, uint8_t currentBytes, uint64_t totalBytes);
void sha1Result(Sha1State* state, char* result);

void sha1(const void* data, size_t length, char* result);

typedef int (*Sha1Callback)(void* param, void* data);
void sha1Universal(Sha1Callback callback, void* param, char* result);

#ifdef __cplusplus
	}
#endif

#endif
