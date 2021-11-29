#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "md5.h"

static const uint32_t magicA = 0x67452301;
static const uint32_t magicB = 0xefcdab89;
static const uint32_t magicC = 0x98badcfe;
static const uint32_t magicD = 0x10325476;

static uint32_t readAsLittleEndian32(uint8_t* data)
{
	return 
	(((uint32_t)data[0])) |
	(((uint32_t)data[1]) << 8) |
	(((uint32_t)data[2]) << 16)  |
	(((uint32_t)data[3]) << 24);
}

static void writeAsLittleEndian64(uint64_t value, uint8_t* data)
{
	data[0] = value;
	data[1] = value >> 8;
	data[2] = value >> 16;
	data[3] = value >> 24;
	data[4] = value >> 32;
	data[5] = value >> 40;
	data[6] = value >> 48;
	data[7] = value >> 56;
}


static uint32_t rotateLeft(uint32_t n, uint8_t bits)
{
	bits &= 0x1f;
	return (n << bits) | (n >> (32-bits));
}

/*
static uint32_t rotateRight(uint32_t n, uint8_t bits)
{
	bits &= 0x1f;
	return (n >> bits) | (n <<(32-bits));
}
*/

#define F(X,Y,Z) ((X&Y)|((~X)&Z))  
#define G(X,Y,Z) ((X&Z)|(Y&(~Z)))  
#define H(X,Y,Z) (X^Y^Z)  
#define I(X,Y,Z) (Y^(X|(~Z))) 

/* The 4 functions of MD5 */
static void FF(uint32_t* a, uint32_t b, uint32_t c, uint32_t d,
            uint32_t Mj, uint8_t s, uint32_t ti)
{
    *a = b + rotateLeft(( *a + F(b,c,d) + Mj + ti ), s);
}

static void GG(uint32_t* a, uint32_t b, uint32_t c, uint32_t d,
            uint32_t Mj, uint8_t s, uint32_t ti)
{
    *a = b + rotateLeft(( *a + G(b,c,d) + Mj + ti ), s);
}

static void HH(uint32_t* a, uint32_t b, uint32_t c, uint32_t d, 
            uint32_t Mj, uint8_t s, uint32_t ti)
{
    *a = b + rotateLeft(( *a + H(b,c,d) + Mj + ti ), s);
}

static void II(uint32_t* a, uint32_t b, uint32_t c, uint32_t d, 
            uint32_t Mj, uint8_t s, uint32_t ti)
{
    *a = b + rotateLeft(( *a + I(b,c,d) + Mj + ti ), s);
}



/* Initialize */
void md5Init(Md5State* state)
{
	state->A = magicA;
	state->B = magicB;
	state->C = magicC;
	state->D = magicD;
}


/* calculate one group */
void md5Count(Md5State* state, const void* data)
{
	uint32_t M[16];
	for(size_t i = 0; i < 16; i++)
	{
		M[i] = readAsLittleEndian32(((uint8_t*)data) + i*4);
	}

	uint32_t a = state->A;
	uint32_t b = state->B;
	uint32_t c = state->C;
	uint32_t d = state->D;

	FF(&a,b,c,d,M[0],7,0xd76aa478);
	FF(&d,a,b,c,M[1],12,0xe8c7b756);  
	FF(&c,d,a,b,M[2],17,0x242070db); 
	FF(&b,c,d,a,M[3],22,0xc1bdceee);  
	FF(&a,b,c,d,M[4],7,0xf57c0faf);
	FF(&d,a,b,c,M[5],12,0x4787c62a);  
	FF(&c,d,a,b,M[6],17,0xa8304613); 
	FF(&b,c,d,a,M[7],22,0xfd469501);  
	FF(&a,b,c,d,M[8],7,0x698098d8);  
	FF(&d,a,b,c,M[9],12,0x8b44f7af);  
	FF(&c,d,a,b,M[10],17,0xffff5bb1);  
	FF(&b,c,d,a,M[11],22,0x895cd7be);  
	FF(&a,b,c,d,M[12],7,0x6b901122); 
	FF(&d,a,b,c,M[13],12,0xfd987193);  
	FF(&c,d,a,b,M[14],17,0xa679438e);  
	FF(&b,c,d,a,M[15],22,0x49b40821);  

	GG(&a,b,c,d,M[1],5,0xf61e2562);  
	GG(&d,a,b,c,M[6],9,0xc040b340);  
	GG(&c,d,a,b,M[11],14,0x265e5a51); 
	GG(&b,c,d,a,M[0],20,0xe9b6c7aa);  
	GG(&a,b,c,d,M[5],5,0xd62f105d);
	GG(&d,a,b,c,M[10],9,0x02441453);  
	GG(&c,d,a,b,M[15],14,0xd8a1e681);
	GG(&b,c,d,a,M[4],20,0xe7d3fbc8);
	GG(&a,b,c,d,M[9],5,0x21e1cde6);
	GG(&d,a,b,c,M[14],9,0xc33707d6);
	GG(&c,d,a,b,M[3],14,0xf4d50d87);
	GG(&b,c,d,a,M[8],20,0x455a14ed);
	GG(&a,b,c,d,M[13],5,0xa9e3e905);
	GG(&d,a,b,c,M[2],9,0xfcefa3f8);
	GG(&c,d,a,b,M[7],14,0x676f02d9);
	GG(&b,c,d,a,M[12],20,0x8d2a4c8a);

	HH(&a,b,c,d,M[5],4,0xfffa3942);
	HH(&d,a,b,c,M[8],11,0x8771f681);
	HH(&c,d,a,b,M[11],16,0x6d9d6122);
	HH(&b,c,d,a,M[14],23,0xfde5380c);
	HH(&a,b,c,d,M[1],4,0xa4beea44);
	HH(&d,a,b,c,M[4],11,0x4bdecfa9);
	HH(&c,d,a,b,M[7],16,0xf6bb4b60);
	HH(&b,c,d,a,M[10],23,0xbebfbc70);
	HH(&a,b,c,d,M[13],4,0x289b7ec6);
	HH(&d,a,b,c,M[0],11,0xeaa127fa);
	HH(&c,d,a,b,M[3],16,0xd4ef3085);
	HH(&b,c,d,a,M[6],23,0x04881d05);
	HH(&a,b,c,d,M[9],4,0xd9d4d039);
	HH(&d,a,b,c,M[12],11,0xe6db99e5);
	HH(&c,d,a,b,M[15],16,0x1fa27cf8);
	HH(&b,c,d,a,M[2],23,0xc4ac5665);
	
	II(&a,b,c,d,M[0],6,0xf4292244);
	II(&d,a,b,c,M[7],10,0x432aff97);
	II(&c,d,a,b,M[14],15,0xab9423a7);
	II(&b,c,d,a,M[5],21,0xfc93a039);
	II(&a,b,c,d,M[12],6,0x655b59c3);
	II(&d,a,b,c,M[3],10,0x8f0ccc92);
	II(&c,d,a,b,M[10],15,0xffeff47d);
	II(&b,c,d,a,M[1],21,0x85845dd1);
	II(&a,b,c,d,M[8],6,0x6fa87e4f);
	II(&d,a,b,c,M[15],10,0xfe2ce6e0);
	II(&c,d,a,b,M[6],15,0xa3014314);
	II(&b,c,d,a,M[13],21,0x4e0811a1);
	II(&a,b,c,d,M[4],6,0xf7537e82);
	II(&d,a,b,c,M[11],10,0xbd3af235);
	II(&c,d,a,b,M[2],15,0x2ad7d2bb);
	II(&b,c,d,a,M[9],21,0xeb86d391);
	
	state->A += a;
	state->B += b;
	state->C += c;
	state->D += d;
}


/* Fill the last group and calculate */
void md5Tail(Md5State* state, void* data, uint8_t currentBytes, uint64_t totalBytes)
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
		md5Count(state, data);
		uint8_t oneMore[64] = {0};
		writeAsLittleEndian64(totalBytes * 8, oneMore+56);
		md5Count(state, oneMore);
	}
	/* needn't one more group */
	else
	{
		writeAsLittleEndian64(totalBytes * 8, ((uint8_t*)data+56));
		md5Count(state, data);
	}
	
}



/* Get the result as hex string */
void md5Result(Md5State* state, char* result)
{
	sprintf(result, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
					(uint8_t)state->A,
					(uint8_t)(state->A>>8),
					(uint8_t)(state->A>>16),
					(uint8_t)(state->A>>24),
	
					(uint8_t)state->B,
					(uint8_t)(state->B>>8),
					(uint8_t)(state->B>>16),
					(uint8_t)(state->B>>24),
	
					(uint8_t)state->C,
					(uint8_t)(state->C>>8),
					(uint8_t)(state->C>>16),
					(uint8_t)(state->C>>24),
	
					(uint8_t)state->D,
					(uint8_t)(state->D>>8),
					(uint8_t)(state->D>>16),
					(uint8_t)(state->D>>24)
		);
}


/* MD5 of memory data */
void md5(const void* data,size_t length, char* result)
{
	Md5State state;
	md5Init(&state);
	size_t len = length;
	const uint8_t* group = (const uint8_t*)data;
	for(; len > 64; len -= 64 , group += 64)
	{
		md5Count(&state, group);
	}
	
	char tail[64];
	memcpy(tail, group, len);
	md5Tail(&state, tail, len, length);
	md5Result(&state, result);
}

/* MD5 of any kind of stream */
void md5Universal(Md5Callback callback, void* userdata, char* result)
{
	Md5State state;
	md5Init(&state);
	int currentSize = 0;
	uint64_t totalSize = 0;
	uint8_t data[64];
	while(currentSize = callback(userdata, 64, data), currentSize == 64)
	{
		totalSize += 64;
		md5Count(&state, data);
	}
	totalSize += currentSize;
	md5Tail(&state, data, currentSize, totalSize);
	md5Result(&state, result);
}


/* Reset Md5 */
void md5Reset(Md5* md5)
{
	md5Init(&(md5->state));
	md5->length = 0;
	md5->used = 0;
	md5->hex[0] = 0;
}

/* Add data into Md5State autoly */
void md5Update(Md5* md5, const void* data, size_t length)
{
	/* Forgot reset */
	if(md5->hex[0] != 0)
	{
		md5Reset(md5);
	}

	size_t need = 64 - md5->used;
	while(length >= need)
	{
		void* p = md5->buffer + md5->used;
		memcpy(p, data, need);
		md5Count(&(md5->state), md5->buffer);
		md5->length += 64;
		md5->used = 0;

		data += need;
		length -= need;
		need = 64;
	}

	void* p = md5->buffer + md5->used;
	memcpy(p, data, length);
	md5->used += length;
	// not md5->length += length , because didn't md5Count
}


/* Get the hex */
const char* md5Hex(Md5* md5)
{
	/* Invoke repeatedly */
	if(md5->hex[0] != 0)
	{
		return md5->hex;
	}

	md5->length += md5->used;
	md5Tail(&(md5->state), md5->buffer, md5->used, md5->length);
	md5Result(&(md5->state), md5->hex);
	return (const char*)(md5->hex);
}

const char* md5OfData(Md5* md5, const void* data, size_t length)
{
	md5Reset(md5);
	md5Update(md5, data, length);
	return md5Hex(md5);
}

const char* md5OfString(Md5* md5, const char* str)
{
	return md5OfData(md5, (const void*)(str), strlen(str));
}