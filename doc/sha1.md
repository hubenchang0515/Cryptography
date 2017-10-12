# SHA1
SHA1 in littel-endian machine

# API

### sha1
```C
void sha1(const void* data, size_t length, char* result);
```
Calculate SHA1 value of memory data.
* Parameter data is the data to calculate.
* Parameter length is length of data.
* Parameter result will return SHA1 value as hex string.

### sha1Universal
```C
void sha1Universal(Sha1Callback callback, void* userdata, char* result);
```
Calculate SHA1 value of any kind of stream.
* Parameter callback is used to get data.
* Parameter userdata will pass to callback. 
* Parameter result will return the SHA1 value as hex string.


### Sha1Callback
```C
typedef int (*Sha1Callback)(void* userdata, size_t length, void* data);
```
Parameter userdata is passed from function md5Universal.
Parameter length means how many bytes of data want to get.
Parameter data will return the data.
This callback function will return the true length of gotten data.

## Demo

### Calculate the SHA1 of short string
```C
#include <stdio.h>
#include <string.h>
#include "sha1.h"

int main()
{
	char data[] = "https://github.com/hubenchang0515/Cryptography";
	char sha1Value[41];
	sha1(data, strlen(data), sha1Value);
	printf("%s\n",sha1Value);
	return 0;
}
```

### Calculate the SHA1 of big file
```C
#include <stdio.h>
#include "sha1.h"

/* Sha1Callback */
int getData(void* fp,size_t length, void* data)
{
	return fread(data,1,length,(FILE*)fp);
}

int main()
{	
	FILE* fp = fopen("C++ Primer.pdf","rb"); // about 208MB
	if(fp == NULL)
	{
		printf("Open file failed.\n");
		return 1;
	}
	char sha1Hex[41];
	sha1Universal(getData, fp, sha1Hex);
	fclose(fp);
	
	printf("%s\n",sha1Hex);
	
	return 0;
}
```

