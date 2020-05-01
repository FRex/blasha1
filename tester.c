#include <string.h>
#include <stdio.h>

#define BLASHA1_IMPLEMENTATION
#include "blasha1.h"

const char * kTestData = "test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test";

const char * kTestSha1 = "f9730d2e153614e2ba058c21e59285f44d8ac109";
const char * kEmptySha1 = "da39a3ee5e6b4b0d3255bfef95601890afd80709";

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

    if(errors)
        printf("ERRORS in checkEmpty, %d total\n", errors);
    else
        printf("OK, no errors in checkEmpty\n");
}

static void checkTestOneCall(void)
{
    int i, errors;
    char text[41];

    errors = 0;

    for(i = 0; i < 1234; ++i)
    {
        blasha1_text(kTestData, strlen(kTestData), text);
        if(0 != strcmp(kTestSha1, text))
        {
            ++errors;
            printf("wrong hash for test data from one call api: %s\n", text);
        }
    }

    if(errors)
        printf("ERRORS in checkTestOneCall, %d total\n", errors);
    else
        printf("OK, no errors in checkTestOneCall\n");
}

int main(void)
{
    checkEmpty();
    checkTestOneCall();
    return 0;
}
