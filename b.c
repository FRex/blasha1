#include <stdio.h>
#include <string.h>
#include "other.h"
#include "bsha1.h"

int main(int argc, char ** argv)
{
    struct MappedFile file;
    file.ptr = NULL;
    memorymapfile(argv[1], &file);
    if(!file.ptr)
    {
        fprintf(stderr, "Failed to open or map file '%s'\n", argv[1]);
        return 1;
    }

    char textdigest[41];
    muhsha1_text(file.ptr, file.size, textdigest);
    printf("%s *%s\n", textdigest, argv[1]);
    unmapfile(&file);
}
