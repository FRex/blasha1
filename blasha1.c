#include "blasha1.h"
#include <string.h>

static blasha1_u32_t bigU32(const blasha1_byte_t * bytes)
{
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]);
}

static blasha1_u32_t leftrotate(blasha1_u32_t n, int amount)
{
    return (n << amount) | (n >> (32 - amount));
}

static void blow16to80(const blasha1_byte_t * chunk, blasha1_u32_t * w)
{
    for(int i = 0; i < 16; ++i)
        w[i] = bigU32(chunk + i * 4);

    for(int i = 16; i < 80; ++i)
        w[i] = leftrotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
}

static void dochunk(const blasha1_u32_t * w, blasha1_u32_t * h)
{
    blasha1_u32_t a = h[0];
    blasha1_u32_t b = h[1];
    blasha1_u32_t c = h[2];
    blasha1_u32_t d = h[3];
    blasha1_u32_t e = h[4];

#define QCHUNK(i, k, f){const blasha1_u32_t tmp = leftrotate(a, 5) + (f) + e + (k) + w[i]; e = d; d = c; c = leftrotate(b, 30); b = a; a = tmp;}
    QCHUNK(0, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(1, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(2, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(3, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(4, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(5, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(6, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(7, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(8, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(9, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(10, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(11, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(12, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(13, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(14, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(15, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(16, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(17, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(18, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(19, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(20, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(21, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(22, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(23, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(24, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(25, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(26, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(27, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(28, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(29, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(30, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(31, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(32, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(33, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(34, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(35, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(36, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(37, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(38, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(39, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(40, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(41, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(42, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(43, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(44, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(45, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(46, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(47, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(48, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(49, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(50, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(51, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(52, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(53, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(54, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(55, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(56, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(57, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(58, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(59, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(60, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(61, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(62, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(63, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(64, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(65, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(66, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(67, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(68, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(69, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(70, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(71, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(72, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(73, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(74, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(75, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(76, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(77, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(78, 0xca62c1d6u, b ^ c ^ d);
    QCHUNK(79, 0xca62c1d6u, b ^ c ^ d);
#undef QCHUNK

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

static void writeBigU64(blasha1_byte_t * ptr, blasha1_u64_t value)
{
    for(int i = 0; i < 8; ++i)
        ptr[i] = (value >> (8 * (7 - i))) & 0xff;
}

static void writeBigU32(blasha1_byte_t * ptr, blasha1_u32_t value)
{
    for(int i = 0; i < 4; ++i)
        ptr[i] = (value >> (8 * (3 - i))) & 0xff;
}

void blasha1_binary(const void * data, blasha1_u64_t datalen, blasha1_byte_t * digest20)
{
    const blasha1_byte_t * ptr = (const blasha1_byte_t*)data;
    blasha1_u64_t ptrlen = datalen;
    blasha1_u32_t h[5] = { 0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u, 0xc3d2e1F0u };
    blasha1_u32_t w[80];

    while(ptrlen >= 64)
    {
        blow16to80(ptr, w);
        dochunk(w, h);
        ptr += 64;
        ptrlen -= 64;
    }

    blasha1_byte_t tmp[2 * 64];
    memset(tmp, 0x0, 2 * 64);
    for(int i = 0; i < (int)ptrlen; ++i)
        tmp[i] = ptr[i];

    tmp[ptrlen] = 0x80u;
    if(ptrlen < 56)
    {
        writeBigU64(tmp + 56, datalen * 8);
        blow16to80(tmp, w);
        dochunk(w, h);
    }
    else
    {
        writeBigU64(tmp + 64 + 56, datalen * 8);
        blow16to80(tmp, w);
        dochunk(w, h);
        blow16to80(tmp + 64, w);
        dochunk(w, h);
    }

    for(int i = 0; i < 5; ++i)
        writeBigU32(digest20 + i * 4, h[i]);
}

static char hexdigit(int val)
{
    if(0 <= val && val < 10)
        return '0' + val;

    if(10 <= val && val < 16)
        return 'a' + (val - 10);

    return 'f';
}

static void binary_digest_to_hex(const blasha1_byte_t * d20, char * hex41)
{
    for(int i = 0; i < 20; ++i)
    {
        hex41[i * 2 + 0] = hexdigit((d20[i] >> 4) & 0xf);
        hex41[i * 2 + 1] = hexdigit((d20[i] >> 0) & 0xf);
    }
    hex41[40] = '\0';
}

void blasha1_text(const void * data, blasha1_u64_t datalen, char * digest41)
{
    blasha1_byte_t d[20];
    blasha1_binary(data, datalen, d);
    binary_digest_to_hex(d, digest41);
}
