# MD5
MD5 in littel-endian machine

## API

### md5
```C
void md5(const void* data,size_t len, char* result);
```
Calculate MD5 value of memory data.
* Parameter data is the data to calculate.
* Parameter length is length of data.
* Parameter result will return MD5 value as hex string.

### md5Universal
```C
typedef int (*Md5Callback)(void* param, void* data);

void md5Universal(Md5Callback callback, void* param, char* result);
```
Calculate MD5 value of any kind of stream.
* Parameter callback is used to get data.
* Parameter param will pass to callback. 
* Parameter result will return the MD5 value as hex string.
* Sha1Callback write a group (64 bytes) data into parameter data and return 64 while remanent data is more than 64 bytes.
* Sha1Callback write remanent data into parameter data and return then length while remanent data is less than 64 bytes.


## Demo

### Calculate the MD5 of short string
```C
#include <stdio.h>
#include <string.h>
#include "md5.h"

int main()
{
	char data[] = "https://github.com/hubenchang0515/Cryptography";
	char md5Value[33];
	md5(data, strlen(data), md5Value);
	printf("%s\n",md5Value);
	return 0;
}
```

### Calculate the SHA1 of big file
```C
#include <stdio.h>
#include "md5.h"

/* Md5Callback */
int getData(void* fp, void* data)
{
	return fread(data,1,64,(FILE*)fp);
}

int main()
{	
	FILE* fp = fopen("C++ Primer.pdf","rb"); // about 208MB
	if(fp == NULL)
	{
		printf("Open file failed.\n");
		return 1;
	}
	char md5Hex[33];
	md5Universal(getData, fp, md5Hex);
	fclose(fp);
	
	printf("%s\n",md5Hex);
	
	return 0;
}
```

