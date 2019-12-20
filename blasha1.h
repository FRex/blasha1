#include <stdint.h>

typedef uint32_t blasha1_u32_t;
typedef uint64_t blasha1_u64_t;
typedef unsigned char blasha1_byte_t;

void blasha1_binary(const void * data, blasha1_u64_t datalen, blasha1_byte_t * digest20);
void blasha1_text(const void * data, blasha1_u64_t datalen, char * digest41);
