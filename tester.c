#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BLASHA1_IMPLEMENTATION
#include "blasha1.h"

#define REPETITIONS 1

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

    for(i = 0; i < REPETITIONS; ++i)
    {
        trashMemory(&sha1, sizeof(blasha1_t), i);

        blasha1_init(&sha1);
        blasha1_get_text(&sha1, text);
        if(0 != strcmp(kEmptySha1, text))
        {
            ++errors;
            printf("wrong empty sha1 after init: %s (wanted %s)\n", text, kEmptySha1);
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

    for(i = 0; i < REPETITIONS; ++i)
    {
        blasha1_text(data, len, text);
        if(0 != strcmp(goodtext41, text))
        {
            ++errors;
            printf("wrong hash for test data from one call api: %s (wanted %s)\n", text, goodtext41);
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

    for(i = 0; i < REPETITIONS; ++i)
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
            printf("wrong hash for test data after feeding fixed %d chunks: %s (wanted %s)\n",
                amount, text, goodtext41);
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

    for(i = 0; i < REPETITIONS; ++i)
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
            printf("wrong hash for test data after random mixed sizes: %s (wanted %s)\n",
                text, goodtext41);
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

static void * reallocOrFree(void * ptr, size_t newsize)
{
    void * ret = realloc(ptr, newsize);
    if(!ret)
        free(ptr);

    return ret;
}

static void * loadFile(FILE * f, size_t * outfsize)
{
    size_t bufsize = 0u;
    size_t sofar = 0u;
    void * ret = NULL;

    if(!f)
        return NULL;

    while(1)
    {
        if(bufsize - sofar == 0)
        {
            bufsize = bufsize + bufsize / 2 + 20;
            ret = reallocOrFree(ret, bufsize);
            if(!ret)
                return NULL;
        }

        size_t readc = fread(((char*)ret) + sofar, 1, bufsize - sofar, f);
        sofar += readc;
        if((readc == 0) && feof(f))
            break;
    } /* while 1 */

    /* ret = attemptShrink(ret, sofar); */
    /* ret = forceShrink(ret, sofar); */

    *outfsize = sofar;
    return ret;
}

static void * openAndLoadFile(const char * fname, size_t * outfsize)
{
    void * ret;
    FILE * f = fopen(fname, "rb");

    if(!f)
    {
        printf("failed to open %s\n", fname);
        return NULL;
    }

    ret = loadFile(f, outfsize);
    fclose(f);
    return ret;
}

#define MYMAXLINE 200

static int isHexDigit(char c)
{
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f');
}

static int checkAndAdjustLine(char * line)
{
    int i;

    /* even empty line that isn't last has 1 \n in it so this is eof? */
    if(strlen(line) == 0)
        return 0;

    if(strlen(line) == MYMAXLINE)
    {
        printf("line was too long\n");
        return 0;
    }

    for(i = 0; i < 40; ++i)
    {
        if(!isHexDigit(line[i]))
        {
            printf("char %d is not hex digit\n", i);
            return 0;
        }
    }

    /* remove both linux \n and windows \r\n line endings: */
    if(strchr(line, '\n'))
        *strchr(line, '\n') = '\0';

    if(strchr(line, '\r'))
        *strchr(line, '\r') = '\0';

    return 1;
}

static void doitOnFile(FILE * f)
{
    const char * fname;
    char line[MYMAXLINE + 1];
    void * ptr;
    size_t len;

    while(1)
    {
        line[0] = '\0';
        fgets(line, MYMAXLINE + 1, f);

        if(!checkAndAdjustLine(line))
            break;

        fname = strstr(line, " *") + 2;
        *strstr(line, " *") = '\0';

        ptr = openAndLoadFile(fname, &len);
        if(ptr)
            checkData(fname, ptr, len, line);

        free(ptr);
    }
}

static void doit(const char * fname)
{
    /* special case, - = stdin and don't close it either? */
    if(0 == strcmp(fname, "-"))
    {
        doitOnFile(stdin);
        return;
    }

    FILE * f = fopen(fname, "rb");
    if(!f)
    {
        printf("failed to open %s\n", fname);
        return;
    }

    doitOnFile(f);
    fclose(f);
}

int main(int argc, char ** argv)
{
    int i;
    unsigned seed;
    char c = 'x';

    seed = (unsigned)time(NULL);
    srand(seed);
    printf("seed = %u\n", seed);

    checkEmpty();
    checkData("kTestData", kTestData, strlen(kTestData), kTestSha1);
    checkData("oneLowercaseX", &c, 1, kSha1OfLowercaseX);

    for(i = 1; i < argc; ++i)
        doit(argv[i]);

    return 0;
}
