/* blog.kurukurumi.com */
#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stddef.h>

typedef struct md5_t
{
	uint8_t n[16];
}md5_t;

uint32_t rotateLeft(uint32_t n, uint8_t bits);
uint32_t rotateRight(uint32_t n, uint8_t bits);
md5_t md5(const void* data, size_t len);


#endif
