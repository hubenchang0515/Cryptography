# SHA1
SHA1 in littel-endian machine

# API

### sha1Init
```C
void sha1Init(Sha1State* state);
```
* Initialize state.


### sha1Count
```C
void sha1Count(Sha1State* state, void* data);
```
* Count a group (64 bytes) data.
* Parameter data should have 64 bytes memory and 64 bytes data.

### sha1Tail
```C
void sha1Tail(Sha1State* state, void* data, uint8_t currentBytes, uint64_t totalBytes);
```
* Count the last group data.
* Parameter data should have 64 bytes memory and no more than 64 bytes data.
* Parameter currentBytes is the bytes of this group
* Parameter totalBytes is the bytes of total data.

### sha1Result
```C
void sha1Result(Sha1State* state, char* result);
```
* Get the result of MD5 value as hex string.

### sha1
```C
typedef int (*Sha1Callback)(void* param, void* data);

void sha1(Sha1Callback callback, void* param, char* result);
```
* Function sha1 has 3 parameters.
* Parameter callback used to get data.
* Parameter param will pass to callback 
* Parameter result will return the MD5 value as hex string.
* Sha1Callback write a group (64 bytes) data into parameter data and return 64 while remanent data is more than 64 bytes.
* Sha1Callback write remanent data into parameter data and return then length while remanent data is less than 64 bytes.

## API Usage
```C
// init Sha1State
Sha1State state;
sha1Init(&state);
// count a group data , parameter data should
sha1Count(&state, data[64]);
sha1Count(&state, data[64]);
sha1Count(&state, data[64]);
// ...
// count the last group data
sha1Tail(&state, data[64], currentBytes, totalBytes);
// get the sha1 value as hex string
char sha1Hex[33];
sha1Result(&state, sha1Hex);
```

### Demo

### Calculate the sha1 of short string
```C
#include <stdio.h>
#include "sha1.h"

int main()
{
	Sha1State state;
	sha1Init(&state);
	char data[64] = "Hello World";
	sha1Tail(&state, data, 11, 11);
	char sha1[33];
	sha1Result(&state, sha1);
	printf("%s\n",sha1);
	return 0;
}

```

### Calculate the sha1 of big file
```C
#include <stdio.h>
#include "sha1.h"

/* Sha1Callback */
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
	char sha1Hex[33];
	sha1(getData, fp, sha1Hex);
	fclose(fp);
	
	printf("%s\n",sha1Hex);
	
	return 0;
}
```

