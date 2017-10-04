# littleMD5
MD5 in littel-endian machine

```C
#include <stdio.h>
#include <string.h>
#include "md5.h"

int main()
{
	const char* str = "Hello World";
	size_t len = strlen(str);
	md5_t m = md5(str,len);
	for(int i = 0; i < 16; i++)
	{
		printf("%02x",m.n[i]);
	}
	printf("\n");
  return 0;
}
```
