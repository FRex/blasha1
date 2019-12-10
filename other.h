#pragma once
#include <Windows.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned char byte;

static double mytime(void)
{
    LARGE_INTEGER freq, ret;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&ret);
    return ((double)ret.QuadPart) / ((double)freq.QuadPart);
}

struct MappedFile
{
    const unsigned char * ptr;
    u64 size;
    HANDLE file;
    HANDLE mapping;
    const char * name;

    /* instead of volatile, += touch returns into this var to prevent optimization */
    unsigned optimizationpreventer;
};

static void memorymapfile(const char * fname, struct MappedFile * file)
{
    HANDLE f, m;
    void * ptr = NULL;
    LARGE_INTEGER li;

    f = CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(f == INVALID_HANDLE_VALUE)
        return;

    if(GetFileSizeEx(f, &li) == 0)
    {
        CloseHandle(f);
        return;
    }

    m = CreateFileMappingW(f, NULL, PAGE_READONLY, 0, 0, NULL);
    if(m == NULL)
    {
        CloseHandle(f);
        return;
    }

    ptr = MapViewOfFile(m, FILE_MAP_READ, 0, 0, 0);
    if(!ptr)
    {
        CloseHandle(f);
        CloseHandle(m);
        return;
    }

    file->ptr = (const unsigned char*)ptr;
    file->file = f;
    file->mapping = m;
    file->name = fname;
    file->size = li.QuadPart;
}

static void unmapfile(struct MappedFile * file)
{
    if(file->ptr)
    {
        UnmapViewOfFile(file->ptr);
        CloseHandle(file->mapping);
        CloseHandle(file->file);
    }

    file->ptr = NULL;
    file->size = 0;
    file->file = NULL;
    file->mapping = NULL;
    file->name = NULL;
}
