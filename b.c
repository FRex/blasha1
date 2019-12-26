#include <stdio.h>
#include <string.h>
#include "other.h"
#include "blasha1.h"

int main(int argc, char ** argv)
{
    struct MappedFile file;
    int i;

    for(i = 1; i < argc; ++ i)
    {
        file.ptr = NULL;
        memorymapfile(argv[i], &file);
        if(!file.ptr)
        {
            fprintf(stderr, "Failed to open or map file '%s'\n", argv[i]);
            continue;
        }

        char textdigest[41];
        blasha1_text(file.ptr, file.size, textdigest);
        printf("%s *%s\n", textdigest, argv[i]);
        unmapfile(&file);
    }

    return 0;
}
