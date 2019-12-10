#include "bsha1.h"
#include <string.h>

static u32 bigU32(const byte * bytes)
{
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]);
}

static u32 leftrotate(u32 n, int amount)
{
    return (n << amount) | (n >> (32 - amount));
}

static void blow16to80(const byte * chunk, u32 * w)
{
    for(int i = 0; i < 16; ++i)
        w[i] = bigU32(chunk + i * 4);

    for(int i = 16; i < 80; ++i)
        w[i] = leftrotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
}

static void dochunk(const u32 * w, u32 * h)
{
    u32 a = h[0];
    u32 b = h[1];
    u32 c = h[2];
    u32 d = h[3];
    u32 e = h[4];

#define QCHUNK(start, k, f) for(int i = start; i < (start + 20); ++i){const u32 tmp = leftrotate(a, 5) + (f) + e + (k) + w[i]; e = d; d = c; c = leftrotate(b, 30); b = a; a = tmp;}
    QCHUNK(00, 0x5a827999u, (b & c) | ((~b) & d));
    QCHUNK(20, 0x6ed9eba1u, b ^ c ^ d);
    QCHUNK(40, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    QCHUNK(60, 0xca62c1d6u, b ^ c ^ d);
#undef QCHUNK

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

static void writeBigU64(byte * ptr, u64 value)
{
    for(int i = 0; i < 8; ++i)
        ptr[i] = (value >> (8 * (7 - i))) & 0xff;
}

static void writeBigU32(byte * ptr, u32 value)
{
    for(int i = 0; i < 4; ++i)
        ptr[i] = (value >> (8 * (3 - i))) & 0xff;
}

void muhsha1(const void * data, u64 datalen, byte * digest20)
{
    const byte * ptr = (const byte*)data;
    u64 ptrlen = datalen;
    u32 h[5] = { 0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u, 0xc3d2e1F0u };
    u32 w[80];

    while(ptrlen >= 64)
    {
        blow16to80(ptr, w);
        dochunk(w, h);
        ptr += 64;
        ptrlen -= 64;
    }

    byte tmp[2 * 64];
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

static void binary_digest_to_hex(const byte * d20, char * hex41)
{
    for(int i = 0; i < 20; ++i)
    {
        hex41[i * 2 + 0] = hexdigit((d20[i] >> 4) & 0xf);
        hex41[i * 2 + 1] = hexdigit((d20[i] >> 0) & 0xf);
    }
    hex41[40] = '\0';
}

void muhsha1_text(const void * data, u64 datalen, char * digest41)
{
    byte d[20];
    muhsha1(data, datalen, d);
    binary_digest_to_hex(d, digest41);
}
