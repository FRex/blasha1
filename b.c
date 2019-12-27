#include <stdio.h>
#include <string.h>
#include "other.h"
#include "blasha1.h"

int main(int argc, char ** argv)
{
    struct MappedFile file;
    int i;
    blasha1_t c;

    for(i = 1; i < argc; ++ i)
    {
        file.ptr = NULL;
        memorymapfile(argv[i], &file);
        if(!file.ptr)
        {
            fprintf(stderr, "Failed to open or map file '%s'\n", argv[i]);
            continue;
        }

        blasha1_init(&c);
        blasha1_update(&c, file.ptr, file.size);
        char textdigest[41];
        blasha1_get_text(&c, textdigest);
        unmapfile(&file);
        printf("%s *%s\n", textdigest, argv[i]);
    }

    return 0;
}
