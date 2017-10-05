#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "md5.h"


int getData(void* fp, void* data);

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
		char md5Hex[33];
		md5(getData, fp, md5Hex);
		printf("%s : %s\n", argv[i], md5Hex);
		fclose(fp);
	}

	return 0;
}


int getData(void* fp, void* data)
{
	return fread(data,1,64,(FILE*)fp);
}
