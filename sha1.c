#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "sha1.h"

/* Magic Number in little-endian machine */
static const uint32_t magicH0 = 0x67452301;
static const uint32_t magicH1 = 0xefcdab89;
static const uint32_t magicH2 = 0x98badcfe;
static const uint32_t magicH3 = 0x10325476;
static const uint32_t magicH4 = 0xc3d2e1f0;



static uint32_t readAsBigEndian32(uint8_t* data)
{
	return 
	(((uint32_t)data[0]) << 24) |
	(((uint32_t)data[1]) << 16) |
	(((uint32_t)data[2]) << 8)  |
	(((uint32_t)data[3]));
}

static void writeAsBigEndian64(uint64_t value, uint8_t* data)
{
	data[0] = value >> 56;
	data[1] = value >> 48;
	data[2] = value >> 40;
	data[3] = value >> 32;
	data[4] = value >> 24;
	data[5] = value >> 16;
	data[6] = value >> 8;
	data[7] = value;
}


static uint32_t rotateLeft(uint32_t n, uint8_t bits)
{
	bits &= 0x1f;
	return (n << bits) | (n >> (32-bits));
}


/* 4 constant value in little-endian */
static uint32_t K(uint8_t t)
{
	assert(t <=79);
	
	if(t <= 19)
	{
		return 0x5A827999;
	}
	else if(20 <= t && t <= 39)
	{
		return 0x6ED9EBA1;
	}
	else if(40 <= t && t <= 59)
	{
		return 0x8F1BBCDC;
	}
	else
	{
		return 0xCA62C1D6;
	}
}

/* 4 (in fact 3) nonlinear calculate */
static uint32_t F(uint8_t t, uint32_t b, uint32_t c, uint32_t d)
{
	assert(t <=79);
	
	if(t <= 19)
	{
		return ((b & c) | ((~b) & d));
	}
	else if(40 <= t && t <= 59)
	{
		return ((b&c) | (b&d) | (c&d));
	}
	else
	{
		return (b^c^d);
	}
}

void sha1Init(Sha1State* state)
{
	state->A = magicH0;
	state->B = magicH1;
	state->C = magicH2;
	state->D = magicH3;
	state->E = magicH4;
}

void sha1Count(Sha1State* state, const void* data)
{
	uint32_t DATA[80];

	int i = 0;
	for(; i < 16; i++)
	{
		DATA[i] = readAsBigEndian32(((uint8_t*)data) + i*4);
	}
	
	for(; i < 80; i++)
	{
		DATA[i] = rotateLeft(DATA[i-3] ^ DATA[i-8] ^ DATA[i-14] ^ DATA[i-16], 1);
	}
	
	uint32_t A = state->A;
	uint32_t B = state->B;
	uint32_t C = state->C;
	uint32_t D = state->D;
	uint32_t E = state->E;
	uint32_t TEMP;
	
	for(i = 0; i < 80; i++)
	{
		TEMP = rotateLeft(A, 5) + F(i, B, C, D) + E + DATA[i] + K(i);
		E = D;
		D = C;
		C = rotateLeft(B, 30);
		B = A;
		A = TEMP;
	}
	
	state->A += A;
	state->B += B;
	state->C += C;
	state->D += D;
	state->E += E;
}


/* Fill the last group and calculate */
void sha1Tail(Sha1State* state, void* data, uint8_t currentBytes, uint64_t totalBytes)
{
	/* fill current group data by 1000... */
	if(currentBytes != 56)
	{
		((uint8_t*)data)[currentBytes] = 0x80;
		for(uint8_t i = currentBytes + 1; i < 64; i++)
		{
			((uint8_t*)data)[i] = 0;
		}
	}
	
	/* need one more group */
	if(currentBytes > 56)
	{
		sha1Count(state, data);
		uint8_t oneMore[64] = {0};
		writeAsBigEndian64(totalBytes * 8, oneMore+56);
		sha1Count(state, oneMore);
	}
	/* needn't one more group */
	else
	{
		writeAsBigEndian64(totalBytes * 8, ((uint8_t*)data+56));
		sha1Count(state, data);
	}
	
}



/* Get the result as hex string */
void sha1Result(Sha1State* state, char* result)
{
#if UINT32_MAX == UINT_MAX
	sprintf(result, "%08x%08x%08x%08x%08x",state->A,state->B,state->C,state->D,state->E);
#elif UINT32_MAX == ULONG_MAX
	sprintf(result, "%08lx%08lx%08lx%08lx%08lx",state->A,state->B,state->C,state->D,state->E);
#else
	#error uint32_t is not unsigned int or unsigned long in current compiler.
#endif
}


/* SHA1 of memory data */
void sha1(const void* data, size_t length, char* result)
{
	Sha1State state;
	sha1Init(&state);
	size_t len = length;
	const uint8_t* group = (const uint8_t*)data;
	for(; len > 64; len -= 64 , group += 64)
	{
		sha1Count(&state, group);
	}
	
	char tail[64];
	memcpy(tail, group, len);
	sha1Tail(&state, tail, len, length);
	sha1Result(&state, result);
}


/* SHA1 of any kind of stream */
void sha1Universal(Sha1Callback callback, void* userdata, char* result)
{
	Sha1State state;
	sha1Init(&state);
	int currentSize = 0;
	uint64_t totalSize = 0;
	uint8_t data[64];
	while(currentSize = callback(userdata, 64, data), currentSize == 64)
	{
		totalSize += 64;
		sha1Count(&state, data);
	}
	totalSize += currentSize;
	sha1Tail(&state, data, currentSize, totalSize);
	sha1Result(&state, result);
}
