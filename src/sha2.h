#ifndef SHA2_H
#define SHA2_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

/* SHA256 */
typedef struct  Sha256State
{
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
	uint32_t E;
	uint32_t F;
	uint32_t G;
	uint32_t H;
}Sha256State; 


void sha256Init(Sha256State* state);
void sha256Count(Sha256State* state, const void* data);
void sha256Tail(Sha256State* state,void* data, size_t currentBytes, uint64_t totalBytes);
void sha256Result(Sha256State* state, char* result);

void sha256(const void* data, size_t length, char* result);

typedef int (*Sha256Callback)(void* userdata, size_t length, void* data);
void sha256Universal(Sha256Callback callback, void* userdata, char* result);


/* SHA224 */
typedef Sha256State Sha224State;

void sha224Init(Sha256State* state);
#define sha224Count(S,D) sha256Count(S,D)
#define sha224Tail(S,D,C,T) sha256Tail(S,D,C,T)
void sha224Result(Sha224State* state, char* result);

void sha224(const void* data, size_t length, char* result);

typedef int (*Sha224Callback)(void* userdata, size_t length, void* data);
void sha224Universal(Sha224Callback callback, void* userdata, char* result);

/* SHA512 */
typedef struct Sha512State
{
	uint64_t A;
	uint64_t B;
	uint64_t C;
	uint64_t D;
	uint64_t E;
	uint64_t F;
	uint64_t G;
	uint64_t H;
}Sha512State;

void sha512Init( Sha512State* state);
void sha512Count( Sha512State* state, const void* data);
void sha512Tail(Sha512State* state,void* data, size_t currentBytes, uint64_t totalBytesL, uint64_t totalBytesH);
void sha512Result( Sha512State* state, char* result);

void sha512(const void* data, size_t length, char* result);

typedef int (*Sha512Callback)(void* userdata, size_t length, void* data);
void sha512Universal(Sha512Callback callback, void* userdata, char* result);

/* SHA384 */
typedef Sha512State Sha384State;

void sha384Init(Sha384State* state);
#define sha384Count(S,D) sha512Count(S,D)
#define sha384Tail(S,D,C,TL,TH) sha512Tail(S,D,C,TL,TH)
void sha384Result(Sha384State* state, char* result);

void sha384(const void* data, size_t length, char* result);

typedef int (*Sha384Callback)(void* userdata, size_t length, void* data);
void sha384Universal(Sha224Callback callback, void* userdata, char* result);


/* New SHA256 Interface */
typedef struct Sha256
{
	Sha256State state;
	
	char hex[65];                // sha256 result hex sum
	uint8_t buffer[64];          // buffer
	size_t used;                 // length of buffer used
	size_t length;               // length of total data
}Sha256;

void sha256Reset(Sha256* sha256);
void sha256Update(Sha256* sha256, const void* data, size_t length);
const char* sha256Hex(Sha256* sha256);
const char* sha256OfData(Sha256* sha256, const void* data, size_t length);
const char* sha256OfString(Sha256* sha256, const char* str);

/* New SHA224 Interface */
typedef struct Sha224
{
	Sha224State state;
	
	char hex[57];                // sha224 result hex sum
	uint8_t buffer[64];          // buffer
	size_t used;                 // length of buffer used
	size_t length;               // length of total data
}Sha224;

void sha224Reset(Sha224* sha224);
void sha224Update(Sha224* sha224, const void* data, size_t length);
const char* sha224Hex(Sha224* sha224);
const char* sha224OfData(Sha224* sha224, const void* data, size_t length);
const char* sha224OfString(Sha224* sha224, const char* str);


/* New SHA512 Interface */
typedef struct Sha512
{
	Sha512State state;
	
	char hex[129];                // sha512 result hex sum
	uint8_t buffer[128];          // buffer
	uint64_t used;                 // length of buffer used
	uint64_t length_low;           // length of total data , low 
	uint64_t length_high;          // length of total data , high
}Sha512;

void sha512Reset(Sha512* sha512);
void sha512Update(Sha512* sha512, const void* data, size_t length);
const char* sha512Hex(Sha512* sha512);
const char* sha512OfData(Sha512* sha512, const void* data, size_t length);
const char* sha512OfString(Sha512* sha512, const char* str);


/* New SHA384 Interface */
typedef struct Sha384
{
	Sha384State state;
	
	char hex[97];                // sha384 result hex sum
	uint8_t buffer[128];          // buffer
	uint64_t used;                 // length of buffer used
	uint64_t length_low;           // length of total data , low 
	uint64_t length_high;          // length of total data , high
}Sha384;

void sha384Reset(Sha384* sha384);
void sha384Update(Sha384* sha384, const void* data, size_t length);
const char* sha384Hex(Sha384* sha384);
const char* sha384OfData(Sha384* sha384, const void* data, size_t length);
const char* sha384OfString(Sha384* sha384, const char* str);


#ifdef __cplusplus
	}
#endif

#endif
