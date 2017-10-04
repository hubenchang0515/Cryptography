#include <stdlib.h>
#include <string.h>
#include "md5.h"

static const uint32_t magicA = 0x67452301;
static const uint32_t magicB = 0xefcdab89;
static const uint32_t magicC = 0x98badcfe;
static const uint32_t magicD = 0x10325476;

uint32_t rotateLeft(uint32_t n, uint8_t bits)
{
	bits &= 0x1f;
	return (n << bits) | (n >> (32-bits));
}

uint32_t rotateRight(uint32_t n, uint8_t bits)
{
	bits &= 0x1f;
	return (n >> bits) | (n <<(32-bits));
}


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


/* fill data to able to grouping */
static void* md5Fill(const void* data, size_t* len)
{
	size_t size = *len;
	size_t remainder = *len % 64;
	if(remainder > 56)
	{
		size = *len + 64 - remainder + 56;
	}
	else if(remainder < 56)
	{
		size = *len + 56 - remainder;
	}
	
	void* newData = malloc(size + 8);
	
	if(newData != NULL)
	{
		memcpy(newData, data, *len);
		size_t index = *len;
		*(uint8_t*)(newData + index) = 0x80;
		for( index = *len + 1; index < size; index++)
		{
			*(uint8_t*)(newData + index) = 0;
		}
		size_t bits = *len * 8;
		*(uint64_t*)(newData + index) = bits;
		//*(uint32_t*)(newData + 4) = bits;
	}
	
	*len = size + 8;
	return newData;
}


static uint32_t A = 0;
static uint32_t B = 0;
static uint32_t C = 0;
static uint32_t D = 0;

/* calculate the md5 value */
static void md5Calculate(const void* data, size_t len)
{
	A = magicA;
	B = magicB;
	C = magicC;
	D = magicD;
	
	for(size_t i = 0; i < len / 64; i++)
	{
		 uint32_t* M = (uint32_t*)(data + i * 64);
		 
		 uint32_t a = A;
		 uint32_t b = B;
		 uint32_t c = C;
		 uint32_t d = D;
		 
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
		 
		 A += a;
		 B += b;
		 C += c;
		 D += d;
	}
}

md5_t md5(const void* data, size_t len)
{
	size_t size = len;
	void* newData = md5Fill(data,&size);
	
	md5Calculate(newData,size);
	
	free(newData);
	md5_t md5Value;
	
	md5Value.n[0] = (uint8_t)A;
	md5Value.n[1] = (uint8_t)(A>>8);
	md5Value.n[2] = (uint8_t)(A>>16);
	md5Value.n[3] = (uint8_t)(A>>24);
	
	md5Value.n[4] = (uint8_t)B;
	md5Value.n[5] = (uint8_t)(B>>8);
	md5Value.n[6] = (uint8_t)(B>>16);
	md5Value.n[7] = (uint8_t)(B>>24);
	
	md5Value.n[8] =  (uint8_t)C;
	md5Value.n[9] =  (uint8_t)(C>>8);
	md5Value.n[10] = (uint8_t)(C>>16);
	md5Value.n[11] = (uint8_t)(C>>24);
	
	md5Value.n[12] = (uint8_t)D;
	md5Value.n[13] = (uint8_t)(D>>8);
	md5Value.n[14] = (uint8_t)(D>>16);
	md5Value.n[15] = (uint8_t)(D>>24);
	
	return md5Value;
}

