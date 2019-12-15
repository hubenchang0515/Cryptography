# SHA2
SHA2 in littel-endian machine

## API
```C
/* Reset */
void sha224Reset(Sha224* sha224);
void sha256Reset(Sha256* sha256);
void sha384Reset(Sha384* sha384);
void sha512Reset(Sha512* sha512);

/* Add data */
void sha224Update(Sha224* sha224, const void* data, size_t length);
void sha256Update(Sha256* sha256, const void* data, size_t length);
void sha384Update(Sha384* sha384, const void* data, size_t length);
void sha512Update(Sha512* sha512, const void* data, size_t length);

/* Get the result */
const char* sha224Hex(Sha224* sha224);
const char* sha256Hex(Sha256* sha256);
const char* sha384Hex(Sha384* sha384);
const char* sha512Hex(Sha512* sha512);

/* Convenient method to calculate the hash sum of data */
const char* sha224OfData(Sha224* sha224, const void* data, size_t length);
const char* sha256OfData(Sha256* sha256, const void* data, size_t length);
const char* sha384OfData(Sha384* sha384, const void* data, size_t length);
const char* sha512OfData(Sha512* sha512, const void* data, size_t length);

/* Convenient method to calculate the hash sum of C-style string */
const char* sha224OfString(Sha224* sha224, const char* str);
const char* sha256OfString(Sha256* sha256, const char* str);
const char* sha384OfString(Sha384* sha384, const char* str);
const char* sha512OfString(Sha512* sha512, const char* str);
```

## Usage is same to SHA1 and MD5
[MD5](../doc/md5.md)  
[SHA1](../doc/sha224.md) 