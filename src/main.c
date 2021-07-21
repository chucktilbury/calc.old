
/*
 * Simple calculator using flex and bison.
 *
 * This is intended to be a simple demonstrator that has all of the basic
 * pieces needed to build a complete language. The grammar is designed in such
 * a way as to make it easier to translate it to a hand written R/D parser.
 */
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "scan.h"
#include "error.h"

int quit_flag = 0;

int main() {

    char* buf;

    rl_bind_key('\t', rl_insert);

    printf("CALC v0.1\n'help' for commands.\n");
    while ((buf = readline("calc> ")) != NULL) {
        if (strlen(buf) > 0) {
            add_history(buf);

            void* s = yy_scan_string(buf);
            yyparse();
            yy_delete_buffer(s);
            reset_errors();

        }
        // readline mallocs a new buffer every time.
        free(buf);

        if(quit_flag)
            break;
    }

    return 0;
}
