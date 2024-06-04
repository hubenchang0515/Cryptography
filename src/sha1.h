#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

/* Old Interface */
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
void sha1Tail(Sha1State* state,void* data, size_t currentBytes, uint64_t totalBytes);
void sha1Result(Sha1State* state, char* result);

void sha1(const void* data, size_t length, char* result);

typedef int (*Sha1Callback)(void* userdata, size_t length, void* data);
void sha1Universal(Sha1Callback callback, void* userdata, char* result);

/* New Interface */
typedef struct Sha1
{
	Sha1State state;
	
	char hex[41];                // sha1 result hex sum
	uint8_t buffer[64];          // buffer
	size_t used;                 // length of buffer used
	size_t length;               // length of total data
}Sha1;

void sha1Reset(Sha1* sha1);
void sha1Update(Sha1* sha1, const void* data, size_t length);
const char* sha1Hex(Sha1* sha1);
const char* sha1OfData(Sha1* sha1, const void* data, size_t length);
const char* sha1OfString(Sha1* sha1, const char* str);

#ifdef __cplusplus
	}
#endif

#endif
