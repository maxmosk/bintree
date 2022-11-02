#include "hash.h"



hash_t hashFAQ6 (const void *mem, size_t n)
{
    if (NULL == mem)
    {
        return 0;
    }

    const uint8_t *memStart = mem;
    hash_t hash = 0;

    for (size_t i = 0; i < n; i++)
    {
        hash += (uint8_t) memStart[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);


    return hash;
}

