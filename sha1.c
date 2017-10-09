#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "sha1.h"

/* Magic Number in little-endian machine */
static const uint32_t magicH0 = 0x67452301;
static const uint32_t magicH1 = 0xefcdab89;
static const uint32_t magicH2 = 0x98badcfe;
static const uint32_t magicH3 = 0x10325476;
static const uint32_t magicH4 = 0xc3d2e1f0;


static uint32_t changeEndian32(uint32_t n)
{
	return 
	((n & 0xff000000) >> 24) |
	((n & 0x00ff0000) >> 8) |
	((n & 0x0000ff00) << 8) |
	((n & 0x000000ff) << 24) ;
}

static uint64_t changeEndian64(uint64_t n)
{
	return 
	((n & 0xff00000000000000ULL) >> 56) |
	((n & 0x00ff000000000000ULL) >> 40) |
	((n & 0x0000ff0000000000ULL) >> 24) |
	((n & 0x000000ff00000000ULL) >> 8) |
	
	((n & 0x00000000ff000000ULL) << 8) |
	((n & 0x0000000000ff0000ULL) << 24) |
	((n & 0x000000000000ff00ULL) << 40) |
	((n & 0x00000000000000ffULL) << 56) ;
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

void sha1Count(Sha1State* state, void* data)
{
	uint32_t DATA[80];

	int i = 0;
	for(; i < 16; i++)
	{
		DATA[i] = changeEndian32(((uint32_t*)data)[i]);
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
		*(uint64_t*)&oneMore[56] = changeEndian64(totalBytes * 8);
		sha1Count(state, oneMore);
	}
	/* needn't one more group */
	else
	{
		*(uint64_t*)((uint8_t*)data+56) = changeEndian64(totalBytes * 8);
		sha1Count(state, data);
	}
	
}



/* Get the result as hex string */
void sha1Result(Sha1State* state, char* result)
{
	sprintf(result, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
					(uint8_t)(state->A>>24),
					(uint8_t)(state->A>>16),
					(uint8_t)(state->A>>8),
					(uint8_t)(state->A),
	
					(uint8_t)(state->B>>24),
					(uint8_t)(state->B>>16),
					(uint8_t)(state->B>>8),
					(uint8_t)(state->B),
	
					(uint8_t)(state->C>>24),
					(uint8_t)(state->C>>16),
					(uint8_t)(state->C>>8),
					(uint8_t)(state->C),
	
					(uint8_t)(state->D>>24),
					(uint8_t)(state->D>>16),
					(uint8_t)(state->D>>8),
					(uint8_t)(state->D),
					
					(uint8_t)(state->E>>24),
					(uint8_t)(state->E>>16),
					(uint8_t)(state->E>>8),
					(uint8_t)(state->E)
		);
}


/* SHA1 */
void sha1(Sha1Callback callback, void* param, char* result)
{
	Sha1State state;
	sha1Init(&state);
	int currentSize = 0;
	uint64_t totalSize = 0;
	uint8_t data[64];
	while(currentSize = callback(param, data), currentSize == 64)
	{
		totalSize += 64;
		sha1Count(&state, data);
	}
	totalSize += currentSize;
	sha1Tail(&state, data, currentSize, totalSize);
	sha1Result(&state, result);
}
