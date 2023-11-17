#ifndef MYMALLOC_HEADER_H_
    #define MYMALLOC_HEADER_H_

    #include <stddef.h>

void *mymalloc(size_t size);
void myfree(void *ptr);

#endif
