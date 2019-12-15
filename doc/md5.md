# MD5
MD5 in littel-endian machine

## API
```C
/* Reset */
void md5Reset(Md5* md5);

/* Add data */
void md5Update(Md5* md5, const void* data, size_t length);

/* Get the result */
const char* md5Hex(Md5* md5);

/* Convenient method to calculate MD5 of data */
const char* md5OfData(Md5* md5, const void* data, size_t length);

/* Convenient method to calculate MD5 of C-style string */
const char* md5OfString(Md5* md5, const char* str);
```

## Demo

```C
#include <stdio.h>
#include <string.h>
#include "md5.h"

int main(int argc, char* argv[])
{
	FILE* fp = fopen(argv[1], "rb");

	/* Calculate MD5 */
	Md5 md5;
	md5Reset(&md5); // reset
	int bytes = 0;
	char data[4096];
	while((bytes = fread(data, 1, 4096, fp)) > 0)
	{
		md5Update(&md5, data, bytes); // add data
	}
	printf("%s\n", md5Hex(&md5)); // get the result

	fclose(fp);
	return 0;
}
```
