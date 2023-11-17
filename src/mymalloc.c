#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.h"
#include "internal.h"

static struct page_list_s *PAGES = NULL;
static long PAGE_SIZE = 0;

void fill_page(struct page_list_s *page)
{
    struct block_s *cursor;

    for (cursor = page->free;
            ((long) cursor) >
                ((long) page) + ((long) sizeof(struct page_list_s));
            cursor = (struct block_s *) (((long) cursor) - ((long) page->size))) {
        cursor->next = (struct block_s *) (((long) cursor) - ((long) page->size));
    }

    cursor = (struct block_s *) (((long) cursor) + (long) page->size);
    cursor->next = NULL;
}

struct page_list_s *create_page(size_t size)
{
    void *last_allocd_memory = sbrk(0);
    long last_allocd_memory_char = (long) last_allocd_memory;
    long page_size_char = 0;
    struct page_list_s *true_page = NULL;

    if (last_allocd_memory == NULL) {
        return NULL;
    }
    if (PAGE_SIZE == 0) {
        PAGE_SIZE = sysconf(_SC_PAGESIZE);
    }
    page_size_char = PAGE_SIZE;
    brk((void *) last_allocd_memory_char + page_size_char);
    true_page = last_allocd_memory;
    true_page->next = NULL;
    true_page->size = size;
    true_page->free = (void *) last_allocd_memory_char + page_size_char - size;
    fill_page(true_page);
    return true_page;
}

struct page_list_s *find_or_create_page(size_t size)
{
    struct page_list_s *true_page = NULL;
    size_t true_size = ALIGN_S(16, size);

    for (struct page_list_s *cursor = PAGES;
            cursor != NULL && true_page == NULL;
            cursor = cursor->next) {
        if (cursor->size == true_size && cursor->free != NULL) {
            true_page = cursor;
        }
    }
    if (true_page != NULL) {
        return true_page;
    }
    true_page = create_page(true_size);
    if (PAGES == NULL) {
        PAGES = true_page;
    } else {
        true_page->next = PAGES;
        PAGES = true_page;
    }
    return true_page;
}

struct page_list_s *find_page_with_ptr(void *ptr)
{
    struct page_list_s *true_page = NULL;

    for (struct page_list_s *cursor = PAGES;
            cursor != NULL && true_page != NULL;
            cursor = cursor->next) {
        if (((long) ptr) > (long) cursor &&
                ((long) ptr) < ((long) cursor) + (long) PAGE_SIZE) {
            true_page = cursor;
        }
    }
    return true_page;
}

void *mymalloc(size_t size)
{
    struct page_list_s *true_page = NULL;
    void *user_data = NULL;

    true_page = find_or_create_page(size);
    if (true_page == NULL) {
        return NULL;
    }
    user_data = true_page->free;
    true_page->free = true_page->free->next;
    return user_data;
}

void myfree(void *ptr)
{
    struct page_list_s *true_page = NULL;
    struct block_s *block = ptr;

    if (ptr == NULL) {
        return;
    }
    true_page = find_page_with_ptr(ptr);
    if (true_page == NULL) {
        return;
    }
    block->next = true_page->free;
    true_page->free = block;
}
