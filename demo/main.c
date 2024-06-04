#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "md5.h"
#include "sha1.h"
#include "sha2.h"

const char* filename(const char* filepath)
{
	const char* p = filepath + strlen(filepath) - 1;
	for(; p >= filepath && *p != '/' && *p != '\\'; p--);
	return p+1;
}

typedef void (*ResetFunc)(void*);
typedef void (*UpdateFunc)(void* state, const void* data, size_t length);
typedef const char* (*HexFunc)(void* state);

typedef struct HashMethod{
	const char* key;
	ResetFunc reset;
	UpdateFunc update;
	HexFunc hex;;
}HashMethod;


/* Options List [gcc -Wno-incompatible-pointer-types] */
HashMethod methodList[] = 
{
	{"md5", 	(ResetFunc)md5Reset, 	(UpdateFunc)md5Update, 		(HexFunc)md5Hex},
	{"sha1", 	(ResetFunc)sha1Reset, 	(UpdateFunc)sha1Update, 	(HexFunc)sha1Hex},
	{"sha224", 	(ResetFunc)sha224Reset, (UpdateFunc)sha224Update, 	(HexFunc)sha224Hex},
	{"sha256", 	(ResetFunc)sha256Reset, (UpdateFunc)sha256Update, 	(HexFunc)sha256Hex},
	{"sha384", 	(ResetFunc)sha384Reset, (UpdateFunc)sha384Update, 	(HexFunc)sha384Hex},
	{"sha512", 	(ResetFunc)sha512Reset, (UpdateFunc)sha512Update, 	(HexFunc)sha512Hex},
	{NULL, NULL, NULL, NULL}
};

int main(int argc, char* argv[])
{	

	if(argc >= 3)
	{
		HashMethod* hash = methodList;
		for(; hash->key != NULL; hash++)
		{
			if(strcmp(argv[1], hash->key) != 0)
				continue;

			for (int i = 2; i < argc; i++)
			{
				FILE* fp = fopen(argv[i], "rb");
				if(fp == NULL)
				{
					printf("%s\n",strerror(errno));
					return 1;
				}
				char state[sizeof(Sha512)];
				char buffer[1024];
				size_t len;
				hash->reset(state);

				while((len = fread(buffer, 1, 1024, fp)) > 0)
				{
					hash->update(state, buffer, len);
				}
				
				fclose(fp);
				printf("%s: %s\n", filename(argv[i]), hash->hex(state));
			}
			return 0;
		}
	}
	

	printf("Usage   : %s [md5|sha1|sha224|sha256|sha384|sha512] <file> [file...]\n", filename(argv[0]));
	printf("Example : %s md5 text.txt\n", filename(argv[0]));
	printf("          %s sha256 text.txt\n", filename(argv[0]));
		
	return 1;
}

