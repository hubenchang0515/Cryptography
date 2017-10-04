# littleMD5
MD5 in littel-endian machine

## Demo

### Calculate the md5 of short string
```C
#include <stdio.h>
#include "md5.h"

int main()
{
	md5Init();
	char data[64] = "Hello World";
	md5Tail(data, 11, 11);
	char md5[17];
	md5Result(md5);
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

int main()
{
	md5Init();
	
	FILE* fp = fopen("C++ Primer.pdf","rb"); // about 196MB
	if(fp == NULL)
	{
		printf("Open file failed.\n");
		return 1;
	}
	char group[64];
	
	size_t totalSize   = 0;
	size_t currentSize = 0;
	while(currentSize = fread(group,1,64,fp) , currentSize == 64)
	{
		md5Count(group);
		totalSize += 64;
	}
	
	totalSize += currentSize;
	md5Tail(group, currentSize, totalSize);
	fclose(fp);
	
	char md5[17];
	md5Result(md5);
	
	printf("%s\n",md5);
	
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


## API

### md5Init
```C
void md5Init();
```
Initialzation, should be invoked onece every time of calculate


### md5Count
```C
void md5Count(void* data);
```
Calculate a group (64 bytes) data

### md5Tail
```C
void md5Tail(void* data, uint8_t currentBytes, uint64_t totalBytes);
```
Calculate the last group (64 bytes) data

### md5Result
```C
void md5Result(char* result);
```
Return the result of md5 as hex string
