/*
 * Simple routines to wrap memory allocation and facilitate error handling.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* memory_alloc(size_t size) {

    void* ptr = calloc(1, size); // memory is cleared by calloc().
    if(ptr == NULL) {
        fprintf(stderr, "cannot allocate %lu bytes\n", size);
        exit(1);
    }

    return ptr;
}

void* memory_realloc(void* ptr, size_t size) {

    void* nptr = realloc(ptr, size);
    if(nptr == NULL) {
        fprintf(stderr, "cannot re-allocate ptr %p with %lu bytes\n", ptr, size);
        exit(1);
    }

    return nptr;
}

void memory_free(void* ptr) {

    if(ptr != NULL)
        free(ptr);
    else {
        fprintf(stderr, "cannot free NULL pointer\n");
        exit(1);
    }
}

char* memory_strdup(const char* str) {

    size_t len = strlen(str) + 1;
    char* buf = malloc(len);
    if(buf == NULL) {
        fprintf(stderr, "cannot allocate string of %lu bytes\n", len);
        exit(1);
    }
    memcpy(buf, str, len);
    return buf;
}