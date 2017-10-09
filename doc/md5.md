# MD5
MD5 in littel-endian machine

# API

### md5Init
```C
void md5Init(Md5State* state);
```
* Initialize state.


### md5Count
```C
void md5Count(Md5State* state, void* data);
```
* Count a group (64 bytes) data.
* Parameter data should have 64 bytes memory and 64 bytes data.

### md5Tail
```C
void md5Tail(Md5State* state, void* data, uint8_t currentBytes, uint64_t totalBytes);
```
* Count the last group data.
* Parameter data should have 64 bytes memory and no more than 64 bytes data.
* Parameter currentBytes is the bytes of this group
* Parameter totalBytes is the bytes of total data.

### md5Result
```C
void md5Result(Md5State* state, char* result);
```
* Get the result of MD5 value as hex string.

### md5
```C
typedef int (*Md5Callback)(void* param, void* data);

void md5(Md5Callback callback, void* param, char* result);
```
* Function md5 has 3 parameters.
* Parameter callback used to get data.
* Parameter param will pass to callback 
* Parameter result will return the MD5 value as hex string.
* Md5Callback write a group (64 bytes) data into parameter data and return 64 while remanent data is more than 64 bytes.
* Md5Callback write remanent data into parameter data and return then length while remanent data is less than 64 bytes.

## API Usage
```C
// init Md5State
Md5State state;
md5Init(&state);
// count a group data , parameter data should
md5Count(&state, data[64]);
md5Count(&state, data[64]);
md5Count(&state, data[64]);
// ...
// count the last group data
md5Tail(&state, data[64], currentBytes, totalBytes);
// get the md5 value as hex string
char md5Hex[33];
md5Result(&state, md5Hex);
```

### Demo

### Calculate the md5 of short string
```C
#include <stdio.h>
#include "md5.h"

int main()
{
	Md5State state;
	md5Init(&state);
	char data[64] = "Hello World";
	md5Tail(&state, data, 11, 11);
	char md5[33];
	md5Result(&state, md5);
	printf("%s\n",md5);
	return 0;
}

```

```shell
$ ./a.out
b10a8db164e0754105b7a99be72e3fe5
```

### Calculate the md5 of big file
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
	md5(getData, fp, md5Hex);
	fclose(fp);
	
	printf("%s\n",md5Hex);
	
	return 0;
}
```

```shell
$ time ./a.out 
f3b939272394ea923f2132d8432307af

real	0m1.984s
user	0m1.940s
sys	0m0.032s

```
