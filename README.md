# Cryptography
A simple cryptography hash functions library of C programming luanguage.

## Note
* Callback function shouldn't meet exception , will be fixed.
* It should support little-endian and big-endian BUT hasn't been tested in big-endian machine.
* May need ``-fno-strict-aliasing`` compiling option.

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

/* callback */
int getData(void* fp, size_t length, void* data)
{
	return fread(data,1,length,(FILE*)fp);
}

int main(int argc, char* argv[])
{	
	if(argc != 2)
	{
		printf("Usage : %s <file>\n", argv[0]);
		printf("        %s text.txt\n", argv[0]);
		
		return 1;
	}
	FILE* fp = fopen(argv[1],"rb"); 
	if(fp == NULL)
	{
		printf("%s\n",strerror(errno));
		return 1;
	}
	char hex[129];
	md5Universal(getData, fp, hex);
	printf("   MD5: %s\n",hex);
	fseek(fp,SEEK_SET,0);
	
	sha1Universal(getData, fp, hex);
	printf("  SHA1: %s\n",hex);
	fseek(fp,SEEK_SET,0);

	sha224Universal(getData, fp, hex);
	printf("SHA224: %s\n",hex);
	fseek(fp,SEEK_SET,0);
	
	sha256Universal(getData, fp, hex);
	printf("SHA256: %s\n",hex);
	fseek(fp,SEEK_SET,0);
	
	sha384Universal(getData, fp, hex);
	printf("SHA384: %s\n",hex);
	fseek(fp,SEEK_SET,0);
	
	sha512Universal(getData, fp, hex);
	printf("SHA512: %s\n",hex);
	fseek(fp,SEEK_SET,0);
	
	fclose(fp);
	
	return 0;
}
```
In calculating a 200MB file, this program used 5.685s with ``-O3`` compiling option and 33.012s without the option.  
CPU: ``Intel Core i5-3230M CPU @ 3.2GHz``
