# SHA1
SHA1 in littel-endian machine

## API
```C
/* Reset */
void sha1Reset(Sha1* sha1);

/* Add data */
void sha1Update(Sha1* sha1, const void* data, size_t length);

/* Get the result */
const char* sha1Hex(Sha1* sha1);

/* Convenient method to calculate SHA1 of data */
const char* sha1OfData(Sha1* sha1, const void* data, size_t length);

/* Convenient method to calculate SHA1 of C-style string */
const char* sha1OfString(Sha1* sha1, const char* str);
```

## Demo

```C
#include <stdio.h>
#include <string.h>
#include "sha1.h"

int main(int argc, char* argv[])
{
	FILE* fp = fopen(argv[1], "rb");

	/* Calculate SHA1 */
	Sha1 sha1;
	sha1Reset(&sha1); // reset
	int bytes = 0;
	char data[4096];
	while((bytes = fread(data, 1, 4096, fp)) > 0)
	{
		sha1Update(&sha1, data, bytes); // add data
	}
	printf("%s\n", sha1Hex(&sha1)); // get the result

	fclose(fp);
	return 0;
}
```
