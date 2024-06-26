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
	data[0] = (uint8_t) (value >> 56);
	data[1] = (uint8_t) (value >> 48);
	data[2] = (uint8_t) (value >> 40);
	data[3] = (uint8_t) (value >> 32);
	data[4] = (uint8_t) (value >> 24);
	data[5] = (uint8_t) (value >> 16);
	data[6] = (uint8_t) (value >> 8);
	data[7] = (uint8_t) value;
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
void sha1Tail(Sha1State* state, void* data, size_t currentBytes, uint64_t totalBytes)
{
	/* fill current group data by 1000... */
	if(currentBytes != 56)
	{
		((uint8_t*)data)[currentBytes] = 0x80;
		for(size_t i = currentBytes + 1; i < 64; i++)
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
			(uint8_t)(state->E));
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


/* Reset Sha1 */
void sha1Reset(Sha1* sha1)
{
	sha1Init(&(sha1->state));
	sha1->length = 0;
	sha1->used = 0;
	sha1->hex[0] = 0;
}

/* Add data into Sha1State autoly */
void sha1Update(Sha1* sha1, const void* data, size_t length)
{
	/* Forgot reset */
	if(sha1->hex[0] != 0)
	{
		sha1Reset(sha1);
	}

	size_t need = 64 - sha1->used;
	while(length >= need)
	{
		void* p = sha1->buffer + sha1->used;
		memcpy(p, data, need);
		sha1Count(&(sha1->state), sha1->buffer);
		sha1->length += 64;
		sha1->used = 0;

		data = (uint8_t*) data + need;
		length -= need;
		need = 64;
	}

	void* p = sha1->buffer + sha1->used;
	memcpy(p, data, length);
	sha1->used += length;
	// not sha1->length += length , because didn't sha1Count
}


/* Get the hex */
const char* sha1Hex(Sha1* sha1)
{
	/* Invoke repeatedly */
	if(sha1->hex[0] != 0)
	{
		return sha1->hex;
	}

	sha1->length += sha1->used;
	sha1Tail(&(sha1->state), sha1->buffer, sha1->used, sha1->length);
	sha1Result(&(sha1->state), sha1->hex);
	return (const char*)(sha1->hex);
}

const char* sha1OfData(Sha1* sha1, const void* data, size_t length)
{
	sha1Reset(sha1);
	sha1Update(sha1, data, length);
	return sha1Hex(sha1);
}

const char* sha1OfString(Sha1* sha1, const char* str)
{
	return sha1OfData(sha1, (const void*)(str), strlen(str));
}