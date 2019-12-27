#include <stdio.h>
#include <string.h>
#include "other.h"
#include "blasha1.h"

int main(int argc, char ** argv)
{
    blasha1_t c;
    struct MappedFile file;
    char textdigest[41];
    int i;
    const int single = NULL == strstr(argv[0], "a.exe");

    if(argc == 1)
        fprintf(stderr, "single = %d\n", single);

    for(i = 1; i < argc; ++ i)
    {
        file.ptr = NULL;
        memorymapfile(argv[i], &file);
        if(!file.ptr)
        {
            fprintf(stderr, "Failed to open or map file '%s'\n", argv[i]);
            continue;
        }

        if(single)
        {
            blasha1_text(file.ptr, file.size, textdigest);
        }
        else
        {
            blasha1_init(&c);
            blasha1_update(&c, file.ptr, file.size);
            blasha1_get_text(&c, textdigest);
        }

        unmapfile(&file);
        printf("%s *%s\n", textdigest, argv[i]);
    }

    return 0;
}
