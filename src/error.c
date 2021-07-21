
#include <stdio.h>
#include <stdarg.h>

static int errors = 0;

int get_errors() {
    return errors;
}

void reset_errors() {
    errors = 0;
}

void error(const char* fmt, ...) {

    printf("syntax error: ");
    va_list(args);

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
    errors++;
}

extern int verbose;
void msg(int level, const char* fmt, ...) {

    if(verbose >= level) {
        printf("msg: ");
        va_list(args);

        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);

        printf("\n");
    }
}
