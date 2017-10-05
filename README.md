# littleMD5
MD5 in littel-endian machine

## Build
```shell
gcc -o md5 main.c md5.c
```

## Usage
```
Usage : md5 <file>
        md5 text.txt
        md5 a.txt b.txt c.txt
```

## API

```C
void md5Init(Md5State* state);
```

```C
void md5Count(Md5State* state, void* data);
```

```C
void md5Tail(Md5State* state, void* data, uint8_t currentBytes, uint64_t totalBytes);
```

```C
void md5Result(Md5State* state, char* result);
```

```C
typedef int (*Md5Callback)(void* param, void* data);
void md5(Md5Callback callback, void* param, char* result);
```

## Demo

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

