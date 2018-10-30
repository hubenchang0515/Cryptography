#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "md5.h"
#include "sha1.h"
#include "sha2.h"

/* callback */
int getData(void* fp, size_t length, void* data)
{
	return fread(data,1,length,(FILE*)fp);
}


typedef struct CallListNode{
	const char* key;
	void (*func)(Md5Callback , void*, char*);
}CallListNode;


/* Options List */
CallListNode callList[] = 
{
	{"md5", md5Universal},
	{"sha224", sha224Universal},
	{"sha256", sha256Universal},
	{"sha384", sha384Universal},
	{"sha512", sha512Universal},
	{NULL,NULL}
};


const char* filename(const char* filepath)
{
	const char* p = filepath + strlen(filepath) - 1;
	for(; p >= filepath && *p != '/' && *p != '\\'; p--);
	return p+1;
}

int main(int argc, char* argv[])
{	

	if(argc == 3)
	{
		CallListNode* node = callList;
		for(; node->key != NULL; node++)
		{
			if(!strcmp(argv[1],node->key) )
			{
				FILE* fp = fopen(argv[2],"rb");
				if(fp == NULL)
				{
					printf("%s\n",strerror(errno));
					return 1;
				}
				char output[256];
				node->func(getData, fp, output);
				fclose(fp);
				printf("%s\n", output);
				return 0;
			}
		}
	}
	

	printf("Usage   : %s [md5|sha1|sha224|sha256|sha384|sha512] <file>\n", filename(argv[0]));
	printf("Example : %s md5 text.txt\n", filename(argv[0]));
	printf("          %s sha256 text.txt\n", filename(argv[0]));
		
	return 1;

}

