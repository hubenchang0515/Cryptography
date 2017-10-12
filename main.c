#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "md5.h"
#include "sha1.h"

int getData(void* fp,size_t length, void* data);

int main(int argc,char* argv[])
{
	if(argc < 2)
	{
#ifdef MD5
		printf("Usage : md5 <file>\n");
		printf("        md5 text.txt\n");
#elif defined(SHA1)
		printf("Usage : sha1 <file>\n");
		printf("        sha1 text.txt\n");
#else
	#error add '-DMD5' or '-DSHA1' 
#endif
		return 1;
	}

	for(int i = 1; i < argc; i++)
	{
		FILE* fp = fopen(argv[i],"rb");
		if(fp == NULL)
		{
			printf("%s : %s\n", argv[i], strerror(errno));
			continue;
		}
		char hex[64];
#ifdef MD5
		md5Universal(getData, fp, hex);
#elif defined(SHA1)
		sha1Universal(getData, fp, hex);
#else
	#error add '-DMD5' or '-DSHA1' 
#endif
		printf("%s : %s\n", argv[i], hex);
		fclose(fp);
	}

	return 0;
}


int getData(void* fp, size_t length, void* data)
{
	return fread(data, 1, length, (FILE*)fp);
}
