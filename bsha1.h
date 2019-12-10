#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned char byte;

void muhsha1(const void * data, u64 datalen, byte * digest20);
void muhsha1_text(const void * data, u64 datalen, char * digest41);
