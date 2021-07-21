/*
 * This is intended to be a demonstrator that has all of the basic pieces needed
 * to build a complete language.
 */
%{

#include "ast.h"
#include "scan.h"
#include "symbols.h"
#include "error.h"

extern int quit_flag;
extern ast_t* root;

int verbose = 0;

%}
%define parse.error verbose
%debug
%defines
%locations

%union {
    const char* ident;
    double number;
    ast_t* node;
};

%token PRINT SYMT HELP QUIT VERBO
%token <ident> IDENT
%token <number> NUMBER

%type <node> line assignment print term factor unary primary

%%
    /* top level rule */
line
    : /* empty */ { $$ = NULL; }
    | assignment {
        //$$ = $1;
        traverse_ast($$);
        destroy_ast($$);
    }
    | print {
        //$$ = $1;
        printf("Result: %0.3f\n", traverse_ast($$));
        destroy_ast($$);
    }
    | SYMT  {
        //msg(2, "show symbols:");
        dump_symbols();
    }
    | HELP { printf("\nCommands:\n"
                    "quit|q    = end the calculator\n"
                    "help|?|h  = show this text\n"
                    "print|p   = print the value of a variable or expression\n"
                    "symt|s    = show the symbol table\n"
                    "verbose|v = show what's happening in the program\n\n"); }
    | QUIT {
        //msg(3, "quit");
        quit_flag = 1;
    }
    | VERBO {
        verbose++;
        msg(3, "verbose set to %d", verbose);
    }
    | VERBO NUMBER {
        verbose = (int)$2;
        msg(3, "verbose set to %d", verbose);
    }
    ;

    /* assignemnt statment */
assignment
    : IDENT '=' term {
        msg(3, "rule assignment to %s", $1);
        add_symbol($1);
        $$ = ast_assign($1, $3);
    }
    ;

    /* print statement */
print
    : PRINT term {
        msg(3, "print rule");
        $$ = ast_print($2);
    }
    ;

    /* lowest precidence element */
term
    : factor
    | term '+' factor {
        msg(3, "term + rule");
        $$ = ast_binary(PLUS_OP, $1, $3);
    }
    | term '-' factor {
        msg(3, "term - rule");
        $$ = ast_binary(MINUS_OP, $1, $3);
    }
    ;

    /* factor higher precidence than term */
factor
    : unary
    | factor '*' unary {
        msg(3, "factor * rule");
        $$ = ast_binary(STAR_OP, $1, $3);
    }
    | factor '/' unary {
        msg(3, "factor / rule");
        $$ = ast_binary(SLASH_OP, $1, $3);
    }
    | factor '%' unary {
        msg(3, "factor %% rule");
        $$ = ast_binary(PERCENT_OP, $1, $3);
    }
    ;

    /* unary expression */
unary
    : primary
    | '+' unary {
        msg(3, "unary + rule");
        $$ = ast_unary(PLUS_OP, $2);
    }
    | '-' unary {
        msg(3, "unary - rule");
        $$ = ast_unary(MINUS_OP, $2);
    }
    ;

    /* primary expression element */
primary
    : IDENT {
        msg(3, "identifier: \"%s\"", $1);
        if(SYM_NOT_FOUND == find_symbol($1, NULL))
            error("symbol \"%s\" is not found", $1);
        else
            $$ = ast_variable($1);
    }
    | NUMBER {
        msg(3, "literal number: %0.3f rule", $1);
        $$ = ast_literal($1);
    }
    | '(' term ')' {
        msg(3, "(term) rule");
    }
    ;


%%

#include <stdio.h>

extern char yytext[];

void yyerror(const char* s)
{
    fflush(stderr);
    //fprintf(stderr, "Syntax Error: %s: line %d: at %d: %s\n", get_file_name(),
    //                  get_line_number(), get_col_number(), s);
    //inc_error_count();
    fprintf(stderr, "%s\n", s);
}
