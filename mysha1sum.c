#include <stdlib.h>
#include <stdio.h>

#define BLASHA1_IMPLEMENTATION
#include "blasha1.h"

#define mybufsize (1 * 1024 * 1024)

int main(int argc, char ** argv)
{
    blasha1_t c;
    char * data;
    char textdigest[41];
    int i;

    data = (char*)malloc(mybufsize);
    if(!data)
        return 1;

    for(i = 1; i < argc; ++i)
    {
        FILE * f = fopen(argv[i], "rb");

        if(!f)
        {
            fprintf(stderr, "Failed to fopen file '%s'\n", argv[i]);
            continue;
        }

        blasha1_init(&c);

        while(1)
        {
            const size_t readc = fread(data, 1, mybufsize, f);
            blasha1_update(&c, data, readc);
            if(feof(f))
                break;
        }

        blasha1_finish_text(&c, textdigest);
        fclose(f);
        printf("%s *%s\n", textdigest, argv[i]);
    }

    free(data);
    return 0;
}
