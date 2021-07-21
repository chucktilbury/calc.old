

#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <stdbool.h>

typedef enum {
    SYM_NO_ERROR,
    SYM_NOT_FOUND,
    SYM_NOT_ASSIGNED,
    SYM_EXISTS,
} symbols_error_t;

symbols_error_t add_symbol(const char* name); //, double val, bool flag);
symbols_error_t assign_symbol(const char* name, double val);
symbols_error_t find_symbol(const char* name, double* val);
symbols_error_t symbol_is_assigned(const char* name);
void dump_symbols();

#endif
