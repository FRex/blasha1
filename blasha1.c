#include "blasha1.h"
#include <string.h>

static blasha1_u32_t blasha1_priv_bigU32(const blasha1_byte_t * bytes)
{
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]);
}

static blasha1_u32_t blasha1_priv_leftrotate(blasha1_u32_t n, int amount)
{
    return (n << amount) | (n >> (32 - amount));
}

static void blasha1_priv_blow16to80(const blasha1_byte_t * chunk, blasha1_u32_t * w)
{
    int i;

    for(i = 0; i < 16; ++i)
        w[i] = blasha1_priv_bigU32(chunk + i * 4);

    for(i = 16; i < 80; ++i)
        w[i] = blasha1_priv_leftrotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
}

static void blasha1_priv_dochunk(const blasha1_u32_t * w, blasha1_u32_t * h)
{
    blasha1_u32_t a = h[0];
    blasha1_u32_t b = h[1];
    blasha1_u32_t c = h[2];
    blasha1_u32_t d = h[3];
    blasha1_u32_t e = h[4];

#define BLASHA1_PRIV_QCHUNK(i, k, f){const blasha1_u32_t tmp = blasha1_priv_leftrotate(a, 5) + (f) + e + (k) + w[i]; e = d; d = c; c = blasha1_priv_leftrotate(b, 30); b = a; a = tmp;}
    BLASHA1_PRIV_QCHUNK(0, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(1, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(2, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(3, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(4, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(5, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(6, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(7, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(8, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(9, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(10, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(11, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(12, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(13, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(14, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(15, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(16, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(17, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(18, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(19, 0x5a827999u, (b & c) | ((~b) & d));
    BLASHA1_PRIV_QCHUNK(20, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(21, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(22, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(23, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(24, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(25, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(26, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(27, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(28, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(29, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(30, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(31, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(32, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(33, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(34, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(35, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(36, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(37, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(38, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(39, 0x6ed9eba1u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(40, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(41, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(42, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(43, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(44, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(45, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(46, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(47, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(48, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(49, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(50, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(51, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(52, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(53, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(54, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(55, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(56, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(57, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(58, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(59, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    BLASHA1_PRIV_QCHUNK(60, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(61, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(62, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(63, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(64, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(65, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(66, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(67, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(68, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(69, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(70, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(71, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(72, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(73, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(74, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(75, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(76, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(77, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(78, 0xca62c1d6u, b ^ c ^ d);
    BLASHA1_PRIV_QCHUNK(79, 0xca62c1d6u, b ^ c ^ d);
#undef BLASHA1_PRIV_QCHUNK

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

static void blasha1_priv_writeBigU64(blasha1_byte_t * ptr, blasha1_u64_t value)
{
    int i;

    for(i = 0; i < 8; ++i)
        ptr[i] = (value >> (8 * (7 - i))) & 0xff;
}

static void blasha1_priv_writeBigU32(blasha1_byte_t * ptr, blasha1_u32_t value)
{
    int i;

    for(i = 0; i < 4; ++i)
        ptr[i] = (value >> (8 * (3 - i))) & 0xff;
}

void blasha1_binary(const void * data, blasha1_u64_t datalen, blasha1_byte_t * digest20)
{
    blasha1_u32_t h[5] = { 0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u, 0xc3d2e1F0u };
    blasha1_u32_t w[80];
    blasha1_byte_t tmp[2 * 64];
    const blasha1_byte_t * ptr = (const blasha1_byte_t*)data;
    blasha1_u64_t ptrlen = datalen;
    int i;

    while(ptrlen >= 64)
    {
        blasha1_priv_blow16to80(ptr, w);
        blasha1_priv_dochunk(w, h);
        ptr += 64;
        ptrlen -= 64;
    }

    memset(tmp, 0x0, 2 * 64);
    for(i = 0; i < (int)ptrlen; ++i)
        tmp[i] = ptr[i];

    tmp[ptrlen] = 0x80u;
    if(ptrlen < 56)
    {
        blasha1_priv_writeBigU64(tmp + 56, datalen * 8);
        blasha1_priv_blow16to80(tmp, w);
        blasha1_priv_dochunk(w, h);
    }
    else
    {
        blasha1_priv_writeBigU64(tmp + 64 + 56, datalen * 8);
        blasha1_priv_blow16to80(tmp, w);
        blasha1_priv_dochunk(w, h);
        blasha1_priv_blow16to80(tmp + 64, w);
        blasha1_priv_dochunk(w, h);
    }

    for(i = 0; i < 5; ++i)
        blasha1_priv_writeBigU32(digest20 + i * 4, h[i]);
}

static char blasha1_priv_hexdigit(int val)
{
    if(0 <= val && val < 10)
        return '0' + val;

    if(10 <= val && val < 16)
        return 'a' + (val - 10);

    return 'f';
}

static void blasha1_priv_binary_digest_to_hex(const blasha1_byte_t * d20, char * hex41)
{
    int i;

    for(i = 0; i < 20; ++i)
    {
        hex41[i * 2 + 0] = blasha1_priv_hexdigit((d20[i] >> 4) & 0xf);
        hex41[i * 2 + 1] = blasha1_priv_hexdigit((d20[i] >> 0) & 0xf);
    }

    hex41[40] = '\0';
}

void blasha1_text(const void * data, blasha1_u64_t datalen, char * digest41)
{
    blasha1_byte_t d[20];
    blasha1_binary(data, datalen, d);
    blasha1_priv_binary_digest_to_hex(d, digest41);
}

void blasha1_init(blasha1_t * c)
{
    c->size = 0;
    c->h[0] = 0x67452301u;
    c->h[1] = 0xefcdab89u;
    c->h[2] = 0x98badcfeu;
    c->h[3] = 0x10325476u;
    c->h[4] = 0xc3d2e1F0u;
}

void blasha1_update(blasha1_t * c, const void * data, blasha1_u64_t datalen)
{
    const blasha1_byte_t * bytes = (const blasha1_byte_t *)data;

    if(c->size % 64 == 0)
    {
        while(datalen >= 64)
        {
            blasha1_u32_t w[80];
            blasha1_priv_blow16to80(bytes, w);
            blasha1_priv_dochunk(w, c->h);
            bytes += 64;
            datalen -= 64;
            c->size += 64;
        } /* while */
    } /* if */

    while(datalen > 0)
    {
        c->data[c->size % 64] = bytes[0];
        bytes += 1;
        datalen -= 1;
        c->size += 1;
        if(c->size % 64 == 0)
        {
            blasha1_u32_t w[80];
            blasha1_priv_blow16to80(c->data, w);
            blasha1_priv_dochunk(w, c->h);
            break;
        }
    }

    if(c->size % 64 == 0)
    {
        while(datalen >= 64)
        {
            blasha1_u32_t w[80];
            blasha1_priv_blow16to80(bytes, w);
            blasha1_priv_dochunk(w, c->h);
            bytes += 64;
            datalen -= 64;
            c->size += 64;
        } /* while */
    } /* if */

    while(datalen > 0)
    {
        c->data[c->size % 64] = bytes[0];
        bytes += 1;
        datalen -= 1;
        c->size += 1;
        if(c->size % 64 == 0)
        {
            blasha1_u32_t w[80];
            blasha1_priv_blow16to80(c->data, w);
            blasha1_priv_dochunk(w, c->h);
        }
    }
}

void blasha1_get_binary(const blasha1_t * c, blasha1_byte_t * digest20)
{
    blasha1_u32_t h[5] = { c->h[0], c->h[1], c->h[2], c->h[3], c->h[4] };
    blasha1_u32_t w[80];
    blasha1_byte_t tmp[2 * 64];
    int lastchunklen, i;

    memset(tmp, 0x0, 2 * 64);
    lastchunklen = (int)(c->size % 64);
    for(i = 0; i < lastchunklen; ++i)
        tmp[i] = c->data[i];

    tmp[lastchunklen] = 0x80u;
    if(lastchunklen < 56)
    {
        blasha1_priv_writeBigU64(tmp + 56, c->size * 8);
        blasha1_priv_blow16to80(tmp, w);
        blasha1_priv_dochunk(w, h);
    }
    else
    {
        blasha1_priv_writeBigU64(tmp + 64 + 56, c->size * 8);
        blasha1_priv_blow16to80(tmp, w);
        blasha1_priv_dochunk(w, h);
        blasha1_priv_blow16to80(tmp + 64, w);
        blasha1_priv_dochunk(w, h);
    }

    for(i = 0; i < 5; ++i)
        blasha1_priv_writeBigU32(digest20 + i * 4, h[i]);
}

void blasha1_get_text(const blasha1_t * c, char * digest41)
{
    blasha1_byte_t d[20];
    blasha1_get_binary(c, d);
    blasha1_priv_binary_digest_to_hex(d, digest41);
}
