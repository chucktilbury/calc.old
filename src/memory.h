

#ifndef __MEMORY_H__
#define __MEMORY_H__

#define ALLOC(s)        memory_alloc(s)
#define ALLOC_DS(t)     memory_alloc(sizeof(t))
#define ALLOC_LST(n,t)  memory_alloc((n)*sizeof(t))
#define REALLOC(p, s)   memory_realloc((p), (s))
#define REALLOC_LST(p,n,t) memory_realloc((p), ((n)*sizeof(t)))
#define STRDUP(s)       memory_strdup(s)
#define FREE(p)         memory_free(p)

void* memory_alloc(size_t size);
void* memory_realloc(void* ptr, size_t size);
char* memory_strdup(const char* str);
void memory_free(void* ptr);

#endif