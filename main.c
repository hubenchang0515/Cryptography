#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "md5.h"

int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("Usage : md5 <file>\n");
		printf("        md5 text.txt\n");

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
		md5Init();
		uint8_t group[64];
		size_t totalSize = 0;
		size_t currentSize = 0;
		while(currentSize = fread(group,1,64,fp) , currentSize == 64)
		{
			totalSize += 64;
			md5Count(group);
		}
		totalSize += currentSize;
		md5Tail(group,currentSize,totalSize);
		char md5[17];
		md5Result(md5);
		printf("%s : %s\n", argv[i], md5);
		fclose(fp);
	}

	return 0;
}
