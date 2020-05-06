#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BLASHA1_IMPLEMENTATION
#include "blasha1.h"

const char * kTestData = "test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test";

const char * kTestSha1 = "f9730d2e153614e2ba058c21e59285f44d8ac109";
const char * kEmptySha1 = "da39a3ee5e6b4b0d3255bfef95601890afd80709";

const char * kSha1OfLowercaseX = "11f6ad8ec52a2984abaafd7c3b516503785c2072";

static void trashMemory(void * ptr, int size, int extra)
{
    while(size > 0)
    {
        --size;
        ((unsigned char*)ptr)[size] = (unsigned char)((extra * size) & 0xff);
    }
}

static void checkEmpty(void)
{
    int i, errors;
    char text[41];
    blasha1_t sha1;

    errors = 0;

    for(i = 0; i < 1234; ++i)
    {
        trashMemory(&sha1, sizeof(blasha1_t), i);

        blasha1_init(&sha1);
        blasha1_get_text(&sha1, text);
        if(0 != strcmp(kEmptySha1, text))
        {
            ++errors;
            printf("wrong empty sha1 after init: %s\n", text);
        }

        blasha1_update(&sha1, NULL, 123);
        blasha1_update(&sha1, &sha1, 0);
        blasha1_update(&sha1, NULL, 0);
        blasha1_get_text(&sha1, text);
        if(0 != strcmp(kEmptySha1, text))
        {
            ++errors;
            printf("wrong empty sha1 after feeding ptr=NULL len=0 data: %s\n", text);
        }

        blasha1_text(&sha1, 0, text);
        if(0 != strcmp(kEmptySha1, text))
        {
            ++errors;
            printf("wrong empty sha1 after one call api len=0: %s\n", text);
        }

        blasha1_text(NULL, 123, text);
        if(0 != strcmp(kEmptySha1, text))
        {
            ++errors;
            printf("wrong empty sha1 after one call api ptr=NULL: %s\n", text);
        }

        blasha1_text(NULL, 0, text);
        if(0 != strcmp(kEmptySha1, text))
        {
            ++errors;
            printf("wrong empty sha1 after one call api ptr=NULL len=0: %s\n", text);
        }
    } /* for */

    printf("Errors in checkEmpty: %d\n", errors);
}

static int checkOneCall(const void * data, size_t len, const char * goodtext41)
{
    int i, errors;
    char text[41];

    errors = 0;

    for(i = 0; i < 1234; ++i)
    {
        blasha1_text(data, len, text);
        if(0 != strcmp(goodtext41, text))
        {
            ++errors;
            printf("wrong hash for test data from one call api: %s\n", text);
        }
    }

    return errors;
}

static int checkIncremental(int amount, const void * data, size_t len, const char * goodtext41)
{
    int i, errors;
    char text[41];
    blasha1_t sha1;
    size_t j;
    const unsigned char * cdata = (const unsigned char*)data;

    errors = 0;

    for(i = 0; i < 1234; ++i)
    {
        trashMemory(&sha1, sizeof(blasha1_t), i);
        blasha1_init(&sha1);

        for(j = 0u; j < len; j += amount)
        {
            if((len - j) < (unsigned)amount)
                blasha1_update(&sha1, cdata + j, len - j);
            else
                blasha1_update(&sha1, cdata + j, amount);
        }

        blasha1_get_text(&sha1, text);
        if(0 != strcmp(goodtext41, text))
        {
            ++errors;
            printf("wrong hash for test data after feeding fixed %d chunks: %s\n", amount, text);
        }
    }

    return errors;
}

static int checkRandomMixedSizes(const void * data, size_t len, const char * goodtext41)
{
    int i, errors;
    char text[41];
    blasha1_t sha1;
    size_t j;
    const unsigned char * cdata = (const unsigned char*)data;

    errors = 0;

    for(i = 0; i < 1234; ++i)
    {
        trashMemory(&sha1, sizeof(blasha1_t), i);
        blasha1_init(&sha1);

        for(j = 0u; j < len;)
        {
            const int amount = rand() % 130;
            if((len - j) < (unsigned)amount)
                blasha1_update(&sha1, cdata + j, len - j);
            else
                blasha1_update(&sha1, cdata + j, amount);

            j += amount;
        }

        blasha1_get_text(&sha1, text);
        if(0 != strcmp(goodtext41, text))
        {
            ++errors;
            printf("wrong hash for test data after random mixed sizes: %s\n", text);
        }
    }

    return errors;
}

const int kPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83};
const int kPrimeCount = sizeof(kPrimes) / sizeof(kPrimes[0]);

static void checkData(const char * name, const void * data, size_t len, const char * goodtext41)
{
    int i, errtotal;

    errtotal = 0;

    errtotal += checkOneCall(data, len, goodtext41);
    errtotal += checkIncremental(1, data, len, goodtext41);
    errtotal += checkIncremental(len, data, len, goodtext41);

    if(len > 1)
        errtotal += checkIncremental(len - 1, data, len, goodtext41);

    for(i = 0; i < kPrimeCount; ++i)
        errtotal += checkIncremental(kPrimes[i], data, len, goodtext41);

    errtotal += checkRandomMixedSizes(data, len, goodtext41);

    printf("Errors in %s: %d\n", name, errtotal);
}

int main(void)
{
    unsigned seed;
    char c = 'x';

    seed = (unsigned)time(NULL);
    srand(seed);
    printf("seed = %u\n", seed);

    checkEmpty();
    checkData("kTestData", kTestData, strlen(kTestData), kTestSha1);
    checkData("oneLowercaseX", &c, 1, kSha1OfLowercaseX);
    return 0;
}
