#ifndef INTERNAL_MYMALLOC_HEADER_H_
    #define INTERNAL_MYMALLOC_HEADER_H_

    #include <stddef.h>

    #define ALIGN_S(align_to, n) ((n + align_to - 1) & ~ (align_to - 1))

struct page_list_s {
    struct page_list_s *next;
    size_t size;
    struct block_s *free;
};

struct block_s {
    struct block_s *next;
};

void fill_page(struct page_list_s *page);
struct page_list_s *create_page(size_t size);
struct page_list_s *find_or_create_page(size_t size);
struct page_list_s *find_page_with_ptr(void *ptr);

#endif
