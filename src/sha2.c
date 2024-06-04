#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "sha2.h"

/*
static uint32_t rotateLeft(uint32_t n, uint8_t bits)
{
	bits &= 0x1f;
	return (n << bits) | (n >> (32-bits));
}*/

static uint32_t rotateRight32(uint32_t n, uint8_t bits)
{
	bits &= 0x1f;
	return (n >> bits) | (n << (32-bits));
}

static uint64_t rotateRight64(uint64_t n, uint8_t bits)
{
	bits &= 0x3f;
	return (n >> bits) | (n << (64-bits));
}
/*
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
	((n & 0x000000ff00000000ULL) >> 8)  |
	
	((n & 0x00000000ff000000ULL) << 8)  |
	((n & 0x0000000000ff0000ULL) << 24) |
	((n & 0x000000000000ff00ULL) << 40) |
	((n & 0x00000000000000ffULL) << 56) ;
}
*/
static uint32_t readAsBigEndian32(uint8_t* data)
{
	return 
	(((uint32_t)data[0]) << 24) |
	(((uint32_t)data[1]) << 16) |
	(((uint32_t)data[2]) << 8)  |
	(((uint32_t)data[3]));
}

/*
static void writeAsBigEndian32(uint32_t value, uint8_t* data)
{
	data[0] = value >> 24;
	data[1] = value >> 16;
	data[2] = value >> 8;
	data[3] = value;
}
*/
static uint64_t readAsBigEndian64(uint8_t* data)
{
	return 
	(((uint64_t)data[0]) << 56) |
	(((uint64_t)data[1]) << 48) |
	(((uint64_t)data[2]) << 40) |
	(((uint64_t)data[3]) << 32) |
	(((uint64_t)data[4]) << 24) |
	(((uint64_t)data[5]) << 16) |
	(((uint64_t)data[6]) << 8)  |
	(((uint64_t)data[7]));
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

/* SHA256 START */

static uint32_t sha256K[64] = 
{
0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/* Init Sha256State */
void sha256Init( Sha256State* state)
{
	state->A = 0x6a09e667;
	state->B = 0xbb67ae85;
	state->C = 0x3c6ef372;
	state->D = 0xa54ff53a;
	state->E = 0x510e527f;
	state->F = 0x9b05688c;
	state->G = 0x1f83d9ab;
	state->H = 0x5be0cd19;
}


/* Calculate a group data */
void sha256Count(Sha256State* state, const void* data)
{
	uint32_t DATA[64];
	
	size_t i = 0;
	for(; i < 16; i++)
	{
		DATA[i] = readAsBigEndian32(((uint8_t*)data) + i*4);//changeEndian32(((uint32_t*)data)[i]);
	}
	
	for(; i < 64; i++)
	{
		uint32_t S0 = rotateRight32(DATA[i-15],7) ^ rotateRight32(DATA[i-15],18) ^ (DATA[i-15] >> 3);
		uint32_t S1 = rotateRight32(DATA[i-2],17) ^ rotateRight32(DATA[i-2],19) ^ (DATA[i-2] >> 10);
		DATA[i] = DATA[i-16] + S0 + DATA[i-7] + S1;
	}
	
	uint32_t a = state->A;
	uint32_t b = state->B;
	uint32_t c = state->C;
	uint32_t d = state->D;
	uint32_t e = state->E;
	uint32_t f = state->F;
	uint32_t g = state->G;
	uint32_t h = state->H;
	
	for(i = 0; i < 64; i++)
	{
		uint32_t T0 = rotateRight32(e,6) ^ rotateRight32(e,11) ^ rotateRight32(e,25);
		uint32_t T1 = (e & f) ^ ((~e) & g);
		uint32_t T2 = h + T0 + T1 + sha256K[i] + DATA[i];
		uint32_t T3 = rotateRight32(a,2) ^ rotateRight32(a,13) ^ rotateRight32(a,22);
		uint32_t T4 = (a & b) ^ (a & c) ^ (b & c);
		uint32_t T5 = T3 + T4;
		
		h = g;
		g = f;
		f = e;
		e = d + T2;
		d = c;
		c = b;
		b = a;
		a = T2 + T5;
	}
	
	state->A += a;
	state->B += b;
	state->C += c;
	state->D += d;
	state->E += e;
	state->F += f;
	state->G += g;
	state->H += h;
}


/* Calculate the last group data */
void sha256Tail(Sha256State* state,void* data, size_t currentBytes, uint64_t totalBytes)
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
		sha256Count(state, data);
		uint8_t oneMore[64] = {0};
		writeAsBigEndian64(totalBytes * 8, oneMore+56);
		sha256Count(state, oneMore);
	}
	/* needn't one more group */
	else
	{
		writeAsBigEndian64(totalBytes * 8, ((uint8_t*)data+56));
		sha256Count(state, data);
	}
}


/* Get the SHA256 value as hex string */
void sha256Result(Sha256State* state, char* result)
{
sprintf(result, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
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
			(uint8_t)(state->E),
			
			(uint8_t)(state->F>>24),
			(uint8_t)(state->F>>16),
			(uint8_t)(state->F>>8),
			(uint8_t)(state->F),
			
			(uint8_t)(state->G>>24),
			(uint8_t)(state->G>>16),
			(uint8_t)(state->G>>8),
			(uint8_t)(state->G),
			
			(uint8_t)(state->H>>24),
			(uint8_t)(state->H>>16),
			(uint8_t)(state->H>>8),
			(uint8_t)(state->H));
}


/* SHA256 of memory data */
void sha256(const void* data, size_t length, char* result)
{
	Sha256State state;
	sha256Init(&state);
	size_t len = length;
	const uint8_t* group = (const uint8_t*)data;
	for(; len > 64; len -= 64 , group += 64)
	{
		sha256Count(&state, group);
	}
	
	char tail[64];
	memcpy(tail, group, len);
	sha256Tail(&state, tail, len, length);
	sha256Result(&state, result);
}

/* SHA256 of any kind of stream */
void sha256Universal(Sha256Callback callback, void* userdata, char* result)
{
	Sha256State state;
	sha256Init(&state);
	int currentSize = 0;
	uint64_t totalSize = 0;
	uint8_t data[64];
	while(currentSize = callback(userdata, 64, data), currentSize == 64)
	{
		totalSize += 64;
		sha256Count(&state, data);
	}
	totalSize += currentSize;
	sha256Tail(&state, data, currentSize, totalSize);
	sha256Result(&state, result);
}

/* SHA256 END */

/* SHA224 START */
void sha224Init( Sha256State* state)
{
	state->A = 0xc1059ed8;
	state->B = 0x367cd507;
	state->C = 0x3070dd17;
	state->D = 0xf70e5939;
	state->E = 0xffc00b31;
	state->F = 0x68581511;
	state->G = 0x64f98fa7;
	state->H = 0xbefa4fa4;
}


/* Get the SHA224 value as hex string */
void sha224Result( Sha224State* state, char* result)
{
sprintf(result, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
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
			(uint8_t)(state->E),
			
			(uint8_t)(state->F>>24),
			(uint8_t)(state->F>>16),
			(uint8_t)(state->F>>8),
			(uint8_t)(state->F),
			
			(uint8_t)(state->G>>24),
			(uint8_t)(state->G>>16),
			(uint8_t)(state->G>>8),
			(uint8_t)(state->G));
}

/* SHA224 of memory data */
void sha224(const void* data, size_t length, char* result)
{
	Sha224State state;
	sha224Init(&state);
	size_t len = length;
	const uint8_t* group = (const uint8_t*)data;
	for(; len > 64; len -= 64 , group += 64)
	{
		sha224Count(&state, group);
	}
	
	char tail[64];
	memcpy(tail, group, len);
	sha224Tail(&state, tail, len, length);
	sha224Result(&state, result);
}

/* SHA224 of any kind of stream */
void sha224Universal(Sha224Callback callback, void* userdata, char* result)
{
	Sha224State state;
	sha224Init(&state);
	int currentSize = 0;
	uint64_t totalSize = 0;
	uint8_t data[64];
	while(currentSize = callback(userdata, 64, data), currentSize == 64)
	{
		totalSize += 64;
		sha224Count(&state, data);
	}
	totalSize += currentSize;
	sha224Tail(&state, data, currentSize, totalSize);
	sha224Result(&state, result);
}

/* SHA224 END */


/* SHA512 START */

static uint64_t sha512K[80] = 
{
0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 
0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 
0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 
0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 
0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 
0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 
0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 
0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 
0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 
0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 
0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 
0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 
0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 
0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 
0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 
0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 
0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 
0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 
0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 
0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};


/* Init Sha512State */
void sha512Init(Sha512State* state)
{
	state->A = 0x6a09e667f3bcc908ULL;
	state->B = 0xbb67ae8584caa73bULL;
	state->C = 0x3c6ef372fe94f82bULL;
	state->D = 0xa54ff53a5f1d36f1ULL;
	state->E = 0x510e527fade682d1ULL;
	state->F = 0x9b05688c2b3e6c1fULL;
	state->G = 0x1f83d9abfb41bd6bULL;
	state->H = 0x5be0cd19137e2179ULL;
}

/* Calculate a group data */
void sha512Count( Sha512State* state, const void* data)
{
	uint64_t DATA[80];
	
	size_t i = 0;
	for(; i < 16; i++)
	{
		DATA[i] = readAsBigEndian64(((uint8_t*)data) + i*8);
	}
	
	for(; i < 80; i++)
	{
		uint64_t S0 = rotateRight64(DATA[i-15],1) ^ rotateRight64(DATA[i-15],8) ^ (DATA[i-15] >> 7);
		uint64_t S1 = rotateRight64(DATA[i-2],19) ^ rotateRight64(DATA[i-2],61) ^ (DATA[i-2] >> 6);
		DATA[i] = DATA[i-16] + S0 + DATA[i-7] + S1;
	}
	
	uint64_t a = state->A;
	uint64_t b = state->B;
	uint64_t c = state->C;
	uint64_t d = state->D;
	uint64_t e = state->E;
	uint64_t f = state->F;
	uint64_t g = state->G;
	uint64_t h = state->H;
	
	for(i = 0; i < 80; i++)
	{
		uint64_t T0 = rotateRight64(e,14) ^ rotateRight64(e,18) ^ rotateRight64(e,41);
		uint64_t T1 = (e & f) ^ ((~e) & g);
		uint64_t T2 = h + T0 + T1 + sha512K[i] + DATA[i];
		uint64_t T3 = rotateRight64(a,28) ^ rotateRight64(a,34) ^ rotateRight64(a,39);
		uint64_t T4 = (a & b) ^ (a & c) ^ (b & c);
		uint64_t T5 = T3 + T4;
		
		h = g;
		g = f;
		f = e;
		e = d + T2;
		d = c;
		c = b;
		b = a;
		a = T2 + T5;
	}
	
	state->A += a;
	state->B += b;
	state->C += c;
	state->D += d;
	state->E += e;
	state->F += f;
	state->G += g;
	state->H += h;
}

/* Append the size bits */
static void sha512Set128BitsSize(uint8_t* addr, uint64_t totalBytesL, uint64_t totalBytesH)
{
	uint64_t bitsL = totalBytesL * 8;
	uint64_t carry = totalBytesL / (UINT64_MAX / 8); // totalBytesL * 8 / UINT64_MAX 乘法交换律
	uint64_t bitsH = totalBytesH * 8 + carry;
	writeAsBigEndian64(bitsH, addr);
	writeAsBigEndian64(bitsL, addr + 8);
}

/* Calculate the last group data */
void sha512Tail(Sha512State* state,void* data, size_t currentBytes, uint64_t totalBytesL, uint64_t totalBytesH)
{
	/* fill current group data by 1000... */
	if(currentBytes != 112)
	{
		((uint8_t*)data)[currentBytes] = 0x80;
		for(size_t i = currentBytes + 1; i < 128; i++)
		{
			((uint8_t*)data)[i] = 0;
		}
	}

	/* need one more group */
	if(currentBytes > 112)
	{
		sha512Count(state, data);
		uint8_t oneMore[128] = {0};
		uint8_t* bits = &oneMore[112];
		sha512Set128BitsSize(bits,totalBytesL,totalBytesH);
		sha512Count(state, oneMore);
	}
	/* needn't one more group */
	else
	{
		uint8_t* bits = (((uint8_t*)data)+112);
		sha512Set128BitsSize(bits,totalBytesL,totalBytesH);
		sha512Count(state, data);
	}
}

/* Get the SHA512 value as hex string */
void sha512Result(Sha512State* state, char* result)
{
	sprintf(result, "%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x",
			(uint8_t)(state->A>>56),
			(uint8_t)(state->A>>48),
			(uint8_t)(state->A>>40),
			(uint8_t)(state->A>>32),
			(uint8_t)(state->A>>24),
			(uint8_t)(state->A>>16),
			(uint8_t)(state->A>>8),
			(uint8_t)(state->A),
			
			(uint8_t)(state->B>>56),
			(uint8_t)(state->B>>48),
			(uint8_t)(state->B>>40),
			(uint8_t)(state->B>>32),
			(uint8_t)(state->B>>24),
			(uint8_t)(state->B>>16),
			(uint8_t)(state->B>>8),
			(uint8_t)(state->B),
			
			(uint8_t)(state->C>>56),
			(uint8_t)(state->C>>48),
			(uint8_t)(state->C>>40),
			(uint8_t)(state->C>>32),
			(uint8_t)(state->C>>24),
			(uint8_t)(state->C>>16),
			(uint8_t)(state->C>>8),
			(uint8_t)(state->C),
			
			(uint8_t)(state->D>>56),
			(uint8_t)(state->D>>48),
			(uint8_t)(state->D>>40),
			(uint8_t)(state->D>>32),
			(uint8_t)(state->D>>24),
			(uint8_t)(state->D>>16),
			(uint8_t)(state->D>>8),
			(uint8_t)(state->D),
			
			(uint8_t)(state->E>>56),
			(uint8_t)(state->E>>48),
			(uint8_t)(state->E>>40),
			(uint8_t)(state->E>>32),
			(uint8_t)(state->E>>24),
			(uint8_t)(state->E>>16),
			(uint8_t)(state->E>>8),
			(uint8_t)(state->E),
			
			(uint8_t)(state->F>>56),
			(uint8_t)(state->F>>48),
			(uint8_t)(state->F>>40),
			(uint8_t)(state->F>>32),
			(uint8_t)(state->F>>24),
			(uint8_t)(state->F>>16),
			(uint8_t)(state->F>>8),
			(uint8_t)(state->F),
			
			(uint8_t)(state->G>>56),
			(uint8_t)(state->G>>48),
			(uint8_t)(state->G>>40),
			(uint8_t)(state->G>>32),
			(uint8_t)(state->G>>24),
			(uint8_t)(state->G>>16),
			(uint8_t)(state->G>>8),
			(uint8_t)(state->G),
			
			(uint8_t)(state->H>>56),
			(uint8_t)(state->H>>48),
			(uint8_t)(state->H>>40),
			(uint8_t)(state->H>>32),
			(uint8_t)(state->H>>24),
			(uint8_t)(state->H>>16),
			(uint8_t)(state->H>>8),
			(uint8_t)(state->H));
}

/* SHA512 of memory data */
void sha512(const void* data, size_t length, char* result)
{
	Sha512State state;
	sha512Init(&state);
	size_t len = length;
	const uint8_t* group = (const uint8_t*)data;
	for(; len > 128; len -= 128 , group += 128)
	{
		sha512Count(&state, group);
	} 
	
	char tail[128];
	memcpy(tail, group, len);
	sha512Tail(&state, tail, len, length, 0);
	sha512Result(&state, result);
}


/* SHA512 of any kind of stream */
void sha512Universal(Sha512Callback callback, void* userdata, char* result)
{
	Sha512State state;
	sha512Init(&state);
	uint64_t currentSize = 0;
	uint64_t totalSizeL = 0;
	uint64_t totalSizeH = 0;
	uint8_t data[128];
	while(currentSize = callback(userdata, 128, data), currentSize == 128)
	{
		if(totalSizeL <= (UINT64_MAX - 128))
		{
			totalSizeL += 128;
		}
		else
		{
			totalSizeH += 1;
			totalSizeL += 128;
		}
		sha512Count(&state, data);
	}
	
	if(totalSizeL <= (UINT64_MAX - currentSize))
	{
		totalSizeL += currentSize;
	}
	else
	{
		totalSizeH += 1;
		totalSizeL += currentSize;
	}
	
	sha512Tail(&state, data, currentSize, totalSizeL, totalSizeH);
	sha512Result(&state, result);
}

/* SHA512 END */

/*SHA384 START */

/* Init Sha384State */
void sha384Init(Sha384State* state)
{
	state->A = 0xcbbb9d5dc1059ed8;
	state->B = 0x629a292a367cd507;
	state->C = 0x9159015a3070dd17;
	state->D = 0x152fecd8f70e5939;
	state->E = 0x67332667ffc00b31;
	state->F = 0x8eb44a8768581511;
	state->G = 0xdb0c2e0d64f98fa7;
	state->H = 0x47b5481dbefa4fa4;
}

/* Get the SHA384 value as hex string */
void sha384Result(Sha384State* state, char* result)
{
	sprintf(result, "%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x"
					"%02x%02x%02x%02x%02x%02x%02x%02x",
			(uint8_t)(state->A>>56),
			(uint8_t)(state->A>>48),
			(uint8_t)(state->A>>40),
			(uint8_t)(state->A>>32),
			(uint8_t)(state->A>>24),
			(uint8_t)(state->A>>16),
			(uint8_t)(state->A>>8),
			(uint8_t)(state->A),
			
			(uint8_t)(state->B>>56),
			(uint8_t)(state->B>>48),
			(uint8_t)(state->B>>40),
			(uint8_t)(state->B>>32),
			(uint8_t)(state->B>>24),
			(uint8_t)(state->B>>16),
			(uint8_t)(state->B>>8),
			(uint8_t)(state->B),
			
			(uint8_t)(state->C>>56),
			(uint8_t)(state->C>>48),
			(uint8_t)(state->C>>40),
			(uint8_t)(state->C>>32),
			(uint8_t)(state->C>>24),
			(uint8_t)(state->C>>16),
			(uint8_t)(state->C>>8),
			(uint8_t)(state->C),
			
			(uint8_t)(state->D>>56),
			(uint8_t)(state->D>>48),
			(uint8_t)(state->D>>40),
			(uint8_t)(state->D>>32),
			(uint8_t)(state->D>>24),
			(uint8_t)(state->D>>16),
			(uint8_t)(state->D>>8),
			(uint8_t)(state->D),
			
			(uint8_t)(state->E>>56),
			(uint8_t)(state->E>>48),
			(uint8_t)(state->E>>40),
			(uint8_t)(state->E>>32),
			(uint8_t)(state->E>>24),
			(uint8_t)(state->E>>16),
			(uint8_t)(state->E>>8),
			(uint8_t)(state->E),
			
			(uint8_t)(state->F>>56),
			(uint8_t)(state->F>>48),
			(uint8_t)(state->F>>40),
			(uint8_t)(state->F>>32),
			(uint8_t)(state->F>>24),
			(uint8_t)(state->F>>16),
			(uint8_t)(state->F>>8),
			(uint8_t)(state->F));
}

/* SHA384 of memory data */
void sha384(const void* data, size_t length, char* result)
{
	Sha384State state;
	sha384Init(&state);
	size_t len = length;
	const uint8_t* group = (const uint8_t*)data;
	for(; len > 128; len -= 128 , group += 128)
	{
		sha384Count(&state, group);
	}
	
	char tail[128];
	memcpy(tail, group, len);
	sha384Tail(&state, tail, len, length, 0);
	sha384Result(&state, result);
}


/* SHA384 of any kind of stream */
void sha384Universal(Sha384Callback callback, void* userdata, char* result)
{
	Sha384State state;
	sha384Init(&state);
	uint64_t currentSize = 0;
	uint64_t totalSizeL = 0;
	uint64_t totalSizeH = 0;
	uint8_t data[128];
	while(currentSize = callback(userdata, 128, data), currentSize == 128)
	{
		if(totalSizeL <= (UINT64_MAX - 128))
		{
			totalSizeL += 128;
		}
		else
		{
			totalSizeH += 1;
			totalSizeL += 128;
		}
		sha384Count(&state, data);
	}
	
	if(totalSizeL <= (UINT64_MAX - currentSize))
	{
		totalSizeL += currentSize;
	}
	else
	{
		totalSizeH += 1;
		totalSizeL += currentSize;
	}
	
	sha384Tail(&state, data, currentSize, totalSizeL, totalSizeH);
	sha384Result(&state, result);
}

/* SHA384 END */


/* Reset Sha256 */
void sha256Reset(Sha256* sha256)
{
	sha256Init(&(sha256->state));
	sha256->length = 0;
	sha256->used = 0;
	sha256->hex[0] = 0;
}

/* Add data into Sha256State autoly */
void sha256Update(Sha256* sha256, const void* data, size_t length)
{
	/* Forgot reset */
	if(sha256->hex[0] != 0)
	{
		sha256Reset(sha256);
	}

	size_t need = 64 - sha256->used;
	while(length >= need)
	{
		void* p = sha256->buffer + sha256->used;
		memcpy(p, data, need);
		sha256Count(&(sha256->state), sha256->buffer);
		sha256->length += 64;
		sha256->used = 0;

		data = (uint8_t*) data + need;
		length -= need;
		need = 64;
	}

	void* p = sha256->buffer + sha256->used;
	memcpy(p, data, length);
	sha256->used += length;
	// not sha256->length += length , because didn't sha256Count
}


/* Get the hex */
const char* sha256Hex(Sha256* sha256)
{
	/* Invoke repeatedly */
	if(sha256->hex[0] != 0)
	{
		return sha256->hex;
	}

	sha256->length += sha256->used;
	sha256Tail(&(sha256->state), sha256->buffer, sha256->used, sha256->length);
	sha256Result(&(sha256->state), sha256->hex);
	return (const char*)(sha256->hex);
}

const char* sha256OfData(Sha256* sha256, const void* data, size_t length)
{
	sha256Reset(sha256);
	sha256Update(sha256, data, length);
	return sha256Hex(sha256);
}

const char* sha256OfString(Sha256* sha256, const char* str)
{
	return sha256OfData(sha256, (const void*)(str), strlen(str));
}

/* Reset Sha224 */
void sha224Reset(Sha224* sha224)
{
	sha224Init(&(sha224->state));
	sha224->length = 0;
	sha224->used = 0;
	sha224->hex[0] = 0;
}

/* Add data into Sha224State autoly */
void sha224Update(Sha224* sha224, const void* data, size_t length)
{
	/* Forgot reset */
	if(sha224->hex[0] != 0)
	{
		sha224Reset(sha224);
	}

	size_t need = 64 - sha224->used;
	while(length >= need)
	{
		void* p = sha224->buffer + sha224->used;
		memcpy(p, data, need);
		sha224Count(&(sha224->state), sha224->buffer);
		sha224->length += 64;
		sha224->used = 0;

		data = (uint8_t*) data + need;
		length -= need;
		need = 64;
	}

	void* p = sha224->buffer + sha224->used;
	memcpy(p, data, length);
	sha224->used += length;
	// not sha224->length += length , because didn't sha224Count
}


/* Get the hex */
const char* sha224Hex(Sha224* sha224)
{
	/* Invoke repeatedly */
	if(sha224->hex[0] != 0)
	{
		return sha224->hex;
	}

	sha224->length += sha224->used;
	sha224Tail(&(sha224->state), sha224->buffer, sha224->used, sha224->length);
	sha224Result(&(sha224->state), sha224->hex);
	return (const char*)(sha224->hex);
}

const char* sha224OfData(Sha224* sha224, const void* data, size_t length)
{
	sha224Reset(sha224);
	sha224Update(sha224, data, length);
	return sha224Hex(sha224);
}

const char* sha224OfString(Sha224* sha224, const char* str)
{
	return sha224OfData(sha224, (const void*)(str), strlen(str));
}


/* Reset Sha512 */
void sha512Reset(Sha512* sha512)
{
	sha512Init(&(sha512->state));
	sha512->length_low = 0;
	sha512->length_high = 0;
	sha512->used = 0;
	sha512->hex[0] = 0;
}

/* Add data into Sha512State autoly */
void sha512Update(Sha512* sha512, const void* data, size_t length)
{
	/* Forgot reset */
	if(sha512->hex[0] != 0)
	{
		sha512Reset(sha512);
	}

	size_t need = 128 - sha512->used;
	while(length >= need)
	{
		void* p = sha512->buffer + sha512->used;
		memcpy(p, data, need);
		sha512Count(&(sha512->state), sha512->buffer);
		sha512->used = 0;
		if(sha512->length_low <= (UINT64_MAX - 128))
		{
			sha512->length_low += 128;
		}
		else
		{
			sha512->length_high += 1;
			sha512->length_low += 128;
		}

		data = (uint8_t*) data + need;
		length -= need;
		need = 128;
	}

	void* p = sha512->buffer + sha512->used;
	memcpy(p, data, length);
	sha512->used += length;
	// not sha512->length += length , because didn't sha512Count
}


/* Get the hex */
const char* sha512Hex(Sha512* sha512)
{
	/* Invoke repeatedly */
	if(sha512->hex[0] != 0)
	{
		return sha512->hex;
	}

	if(sha512->length_low <= (UINT64_MAX - sha512->used))
	{
		sha512->length_low += sha512->used;
	}
	else
	{
		sha512->length_high += 1;
		sha512->length_low += sha512->used;
	}

	sha512Tail(&(sha512->state), sha512->buffer, sha512->used, sha512->length_low, sha512->length_high);
	sha512Result(&(sha512->state), sha512->hex);
	return (const char*)(sha512->hex);
}

const char* sha512OfData(Sha512* sha512, const void* data, size_t length)
{
	sha512Reset(sha512);
	sha512Update(sha512, data, length);
	return sha512Hex(sha512);
}

const char* sha512OfString(Sha512* sha512, const char* str)
{
	return sha512OfData(sha512, (const void*)(str), strlen(str));
}


/* Reset Sha384 */
void sha384Reset(Sha384* sha384)
{
	sha384Init(&(sha384->state));
	sha384->length_low = 0;
	sha384->length_high = 0;
	sha384->used = 0;
	sha384->hex[0] = 0;
}

/* Add data into Sha384State autoly */
void sha384Update(Sha384* sha384, const void* data, size_t length)
{
	/* Forgot reset */
	if(sha384->hex[0] != 0)
	{
		sha384Reset(sha384);
	}

	size_t need = 128 - sha384->used;
	while(length >= need)
	{
		void* p = sha384->buffer + sha384->used;
		memcpy(p, data, need);
		sha384Count(&(sha384->state), sha384->buffer);
		sha384->used = 0;
		if(sha384->length_low <= (UINT64_MAX - 128))
		{
			sha384->length_low += 128;
		}
		else
		{
			sha384->length_high += 1;
			sha384->length_low += 128;
		}

		data = (uint8_t*) data + need;
		length -= need;
		need = 128;
	}

	void* p = sha384->buffer + sha384->used;
	memcpy(p, data, length);
	sha384->used += length;
	// not sha384->length += length , because didn't sha384Count
}


/* Get the hex */
const char* sha384Hex(Sha384* sha384)
{
	/* Invoke repeatedly */
	if(sha384->hex[0] != 0)
	{
		return sha384->hex;
	}

	if(sha384->length_low <= (UINT64_MAX - sha384->used))
	{
		sha384->length_low += sha384->used;
	}
	else
	{
		sha384->length_high += 1;
		sha384->length_low += sha384->used;
	}

	sha384Tail(&(sha384->state), sha384->buffer, sha384->used, sha384->length_low, sha384->length_high);
	sha384Result(&(sha384->state), sha384->hex);
	return (const char*)(sha384->hex);
}

const char* sha384OfData(Sha384* sha384, const void* data, size_t length)
{
	sha384Reset(sha384);
	sha384Update(sha384, data, length);
	return sha384Hex(sha384);
}

const char* sha384OfString(Sha384* sha384, const char* str)
{
	return sha384OfData(sha384, (const void*)(str), strlen(str));
}