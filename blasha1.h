#ifndef BLASHA1_H
#define BLASHA1_H

/* include this header in any file you need these sha1 apis in, but also make
 * sure to include it in at least one .c or .cpp/.cc/.cxx file, after defining
 * BLASHA1_IMPLEMENTATION, to put the implementation in that one file, both the
 * header and implementation parts should work as any C or C++ version, so if
 * you mix C and C++ and use this lib in both you'll need own extern "C" */


/* fixed size unsigned types needed (the only problematic one is uint64): */
typedef unsigned blasha1_u32_t;
typedef unsigned long long blasha1_u64_t;
typedef unsigned char blasha1_byte_t;


/* one call (ptr, len) -> hash apis (the text one includes nul terminator): */
void blasha1_binary(const void * data, blasha1_u64_t datalen, blasha1_byte_t * digest20);
void blasha1_text(const void * data, blasha1_u64_t datalen, char * digest41);


/* incremental apis: */

/* sha1 state, safe to memcpy, move, etc. e.g. when hashing buffers with same prefix */
typedef struct blasha1 {
    blasha1_u64_t size;
    blasha1_byte_t data[64];
    blasha1_u32_t h[5];
} blasha1_t;

/* initialize the sha1 state (this is not just a memset to 0, so it must be called) */
void blasha1_init(blasha1_t * c);

/* feed data into the state, which changes it */
void blasha1_update(blasha1_t * c, const void * data, blasha1_u64_t datalen);

/* get the digest and reinit the state, text version includes nul term */
void blasha1_finish_binary(blasha1_t * c, blasha1_byte_t * digest20);
void blasha1_finish_text(blasha1_t * c, char * digest41);

#endif /* BLASHA1_H */


/* implementation part (define BLASHA1_IMPLEMENTATION in one source file only) */
#ifdef BLASHA1_IMPLEMENTATION

/* let's check that our 2 fixed width types are right sizes... */
#define BLASHA1_PRIV_STATIC_ASSERT(msg, expr) typedef int BLASHA1_PRIV_STATIC_ASSERT_##msg[(expr) * 2 - 1]
BLASHA1_PRIV_STATIC_ASSERT(uint32_is_4_bytes, sizeof(blasha1_u32_t) == 4);
BLASHA1_PRIV_STATIC_ASSERT(uint64_is_8_bytes, sizeof(blasha1_u64_t) == 8);
#undef BLASHA1_PRIV_STATIC_ASSERT

static blasha1_u32_t blasha1_priv_bigU32(const blasha1_byte_t * bytes)
{
    /* casts needed as explained on https://justine.lol/endian.html to not
    shift an uchar promoted to int left 24 places, it did right thing (movbe
    or mov + bswap) on gcc and clang but was still UB, with casts it isn't */
    return
        (((blasha1_u32_t)bytes[0]) << 24u) |
        (((blasha1_u32_t)bytes[1]) << 16u) |
        (((blasha1_u32_t)bytes[2]) << 8u) |
        (((blasha1_u32_t)bytes[3]));
}

static blasha1_u32_t blasha1_priv_leftrotate(blasha1_u32_t n, int amount)
{
    return (n << amount) | (n >> (32 - amount));
}

static void blasha1_priv_dochunk(const blasha1_byte_t * chunk, blasha1_u32_t * h)
{
    int i;
    blasha1_u32_t w[16];
    blasha1_u32_t a = h[0];
    blasha1_u32_t b = h[1];
    blasha1_u32_t c = h[2];
    blasha1_u32_t d = h[3];
    blasha1_u32_t e = h[4];

    for(i = 0; i < 16; ++i)
        w[i] = blasha1_priv_bigU32(chunk + i * 4);

    /* a w[80] array and precomputing into it was slower than this */
    /* todo: clean up */

#define BLASHA1_PRIV_QCHUNK(i, k, f){\
    blasha1_u32_t tmp;\
    if(i >= 16)\
    w[i % 16] =\
    blasha1_priv_leftrotate(w[(i - 3) % 16] ^ w[(i - 8) % 16] ^ w[(i - 14) % 16] ^ w[(i - 16) % 16], 1);\
    tmp = blasha1_priv_leftrotate(a, 5) + (f) + e + (k) + w[(i % 16)];\
    e = d;\
    d = c;\
    c = blasha1_priv_leftrotate(b, 30);\
    b = a;\
    a = tmp;\
    }

    for(i = 0; i < 20; ++i)
    {
        BLASHA1_PRIV_QCHUNK(i, 0x5a827999u, (b & c) | ((~b) & d));
    }

    for(i = 20; i < 40; ++i)
    {
        BLASHA1_PRIV_QCHUNK(i, 0x6ed9eba1u, b ^ c ^ d);
    }


    for(i = 40; i < 60; ++i)
    {
        BLASHA1_PRIV_QCHUNK(i, 0x8f1bbcdcu, (b & c) | (b & d) | (c & d));
    }


    for(i = 60; i < 80; ++i)
    {
        BLASHA1_PRIV_QCHUNK(i, 0xca62c1d6u, b ^ c ^ d);
    }

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
    blasha1_t state;

    blasha1_init(&state);

#define BLASHA1_PRIV_CHUNKSIZE (64 * 1024)
    while(datalen >= BLASHA1_PRIV_CHUNKSIZE)
    {
        blasha1_update(&state, data, BLASHA1_PRIV_CHUNKSIZE);
        datalen -= BLASHA1_PRIV_CHUNKSIZE;
        data = ((const unsigned char*)data) + BLASHA1_PRIV_CHUNKSIZE;
    } /* while */
#undef BLASHA1_PRIV_CHUNKSIZE

    blasha1_update(&state, data, datalen);
    blasha1_finish_binary(&state, digest20);
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
    /* constants from sha1 spec: */
    c->h[0] = 0x67452301u;
    c->h[1] = 0xefcdab89u;
    c->h[2] = 0x98badcfeu;
    c->h[3] = 0x10325476u;
    c->h[4] = 0xc3d2e1F0u;
}

void blasha1_update(blasha1_t * c, const void * data, blasha1_u64_t datalen)
{
    const blasha1_byte_t * bytes = (const blasha1_byte_t *)data;

    if(!data || datalen == 0u)
        return;

    /* the if and while are copy pasted twice to handle all situations:
     * 1. state has bytes from incomplete block or not
     * 2. new data is enough to complete a block or not
     * 3. new data completes a block then starts new incomplete block or not
     * no second if + while pair => hash errors (tester.exe catches them) */

    if(c->size % 64 == 0)
    {
        while(datalen >= 64)
        {
            blasha1_priv_dochunk(bytes, c->h);
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
            blasha1_priv_dochunk(c->data, c->h);
            break;
        }
    }

    if(c->size % 64 == 0)
    {
        while(datalen >= 64)
        {
            blasha1_priv_dochunk(bytes, c->h);
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
            blasha1_priv_dochunk(c->data, c->h);
    }
}

void blasha1_finish_binary(blasha1_t * c, blasha1_byte_t * digest20)
{
    blasha1_u32_t h[5];
    blasha1_byte_t tmp[2 * 64];
    int lastchunklen, i;

    for(i = 0; i < 5; ++i)
        h[i] = c->h[i];

    for(i = 0; i < 2 * 64; ++i)
        tmp[i] = 0x0;

    lastchunklen = (int)(c->size % 64);
    for(i = 0; i < lastchunklen; ++i)
        tmp[i] = c->data[i];

    tmp[lastchunklen] = 0x80u;
    if(lastchunklen < 56)
    {
        blasha1_priv_writeBigU64(tmp + 56, c->size * 8);
        blasha1_priv_dochunk(tmp, h);
    }
    else
    {
        blasha1_priv_writeBigU64(tmp + 64 + 56, c->size * 8);
        blasha1_priv_dochunk(tmp, h);
        blasha1_priv_dochunk(tmp + 64, h);
    }

    for(i = 0; i < 5; ++i)
        blasha1_priv_writeBigU32(digest20 + i * 4, h[i]);

    blasha1_init(c);
}

void blasha1_finish_text(blasha1_t * c, char * digest41)
{
    blasha1_byte_t d[20];
    blasha1_finish_binary(c, d);
    blasha1_priv_binary_digest_to_hex(d, digest41);
}

#endif /* BLASHA1_IMPLEMENTATION */
