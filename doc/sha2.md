# SHA2
SHA1 in littel-endian machine

## API
```C
void sha256(const void* data, size_t length, char* result);

void sha256Universal(Sha256Callback callback, void* userdata, char* result);

void sha224(const void* data, size_t length, char* result);

void sha224Universal(Sha224Callback callback, void* userdata, char* result);

void sha512(const void* data, size_t length, char* result);

void sha512Universal(Sha512Callback callback, void* userdata, char* result);

void sha384(const void* data, size_t length, char* result);

void sha384Universal(Sha224Callback callback, void* userdata, char* result);
```

## Usage is same to SHA1 and MD5
[MD5](../doc/md5.md)  
[SHA1](../doc/sha1.md) 