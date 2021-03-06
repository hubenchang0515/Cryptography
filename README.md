# Cryptography
A simple cryptography hash functions library of C programming luanguage.

## Note
* Suggest using `-O3` compiler optimization. 
* It should support little-endian and big-endian BUT hasn't been tested in big-endian machine.

## Build
```bash
make
```
* then copy `crypto` to your PATH.

## Usage
```
Usage   : crypto [md5|sha1|sha224|sha256|sha384|sha512] <file>
Example : crypto md5 text.txt
          crypto sha256 text.txt
```

## API Index
[MD5](doc/md5.md)  
[SHA1](doc/sha1.md)  
[SHA2](doc/sha2.md)  

## Demo
```C
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "md5.h"
#include "sha1.h"
#include "sha2.h"

const char* filename(const char* filepath)
{
	const char* p = filepath + strlen(filepath) - 1;
	for(; p >= filepath && *p != '/' && *p != '\\'; p--);
	return p+1;
}

typedef struct CallListNode{
	const char* key;
	void (*initFunc)(void* state);
	void (*updateFunc)(void* state, const void* data, size_t length);
	const char* (*hexFunc)(void* state);
}CallListNode;


/* Options List [gcc -Wno-incompatible-pointer-types] */
CallListNode callList[] = 
{
	{"md5", md5Reset, md5Update, md5Hex},
	{"sha1", sha1Reset, sha1Update, sha1Hex},
	{"sha224", sha224Reset, sha224Update, sha224Hex},
	{"sha256", sha256Reset, sha256Update, sha256Hex},
	{"sha384", sha384Reset, sha384Update, sha384Hex},
	{"sha512", sha512Reset, sha512Update, sha512Hex},
	{NULL, NULL, NULL, NULL}
};

int main(int argc, char* argv[])
{	

	if(argc == 3)
	{
		CallListNode* node = callList;
		for(; node->key != NULL; node++)
		{
			if(!strcmp(argv[1],node->key) )
			{
				FILE* fp = fopen(argv[2], "rb");
				if(fp == NULL)
				{
					printf("%s\n",strerror(errno));
					return 1;
				}
				char state[sizeof(Sha512)];
				char buffer[1024];
				size_t len;
				node->initFunc(state);

				while((len = fread(buffer, 1, 1024, fp)) > 0)
				{
					node->updateFunc(state, buffer, len);
				}
				
				fclose(fp);
				printf("%s\n", node->hexFunc(state));
				return 0;
			}
		}
	}
	

	printf("Usage   : %s [md5|sha1|sha224|sha256|sha384|sha512] <file>\n", filename(argv[0]));
	printf("Example : %s md5 text.txt\n", filename(argv[0]));
	printf("          %s sha256 text.txt\n", filename(argv[0]));
		
	return 1;

}
```