#pragma once
#include <stdint.h>

typedef uint32_t blasha1_u32_t;
typedef uint64_t blasha1_u64_t;
typedef unsigned char blasha1_byte_t;

void blasha1_binary(const void * data, blasha1_u64_t datalen, blasha1_byte_t * digest20);
void blasha1_text(const void * data, blasha1_u64_t datalen, char * digest41);

typedef struct blasha1 {
    blasha1_u64_t size;
    blasha1_byte_t data[64];
    blasha1_u32_t h[5];
} blasha1_t;

void blasha1_init(blasha1_t * c);
void blasha1_update(blasha1_t * c, const void * data, blasha1_u64_t datalen);
void blasha1_get_binary(const blasha1_t * c, blasha1_byte_t * digest20);
void blasha1_get_text(const blasha1_t * c, char * digest41);
