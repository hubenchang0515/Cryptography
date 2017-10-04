/* blog.kurukurumi.com */
#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stddef.h>

uint32_t rotateLeft(uint32_t n, uint8_t bits);
uint32_t rotateRight(uint32_t n, uint8_t bits);

void md5Init();
void md5Count(void* data);
void md5Tail(void* data, uint8_t currentBytes, uint64_t totalBytes);
void md5Result(char* result);

#endif
