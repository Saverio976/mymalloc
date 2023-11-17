#include <stdio.h>
#include "mymalloc.h"

int main(void)
{
    int *a[10] = {0};

    // IN ORDER
    for (int i = 0; i < 10; i++) {
        a[i] = mymalloc(sizeof(int));
        *(a[i]) = i;
    }
    for (int i = 0; i < 10; i++) {
        printf("a[%d]: %d | %p\n", i, *(a[i]), a[i]);
    }
    for (int i = 0; i < 10; i++) {
        myfree(a[i]);
    }

    // IN DISORDER
    for (int i = 0; i < 5; i++) {
        a[i] = mymalloc(sizeof(int));
        *(a[i]) = i;
    }
    for (int i = 0; i < 5; i++) {
        printf("a[%d]: %d | %p\n", i, *(a[i]), a[i]);
    }
    for (int i = 0; i < 2; i++) {
        myfree(a[i]);
    }
    for (int i = 3; i < 10; i++) {
        a[i] = mymalloc(sizeof(int));
        *(a[i]) = i;
    }
    for (int i = 3; i < 10; i++) {
        printf("a[%d]: %d | %p\n", i, *(a[i]), a[i]);
    }
    for (int i = 0; i < 10; i++) {
        myfree(a[i]);
    }

    return 0;
}
