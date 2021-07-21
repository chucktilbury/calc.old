/*
 * Function prototypes for the visit functions.
 */
#ifndef __VISIT_H__
#define __VISIT_H__

#include "ast.h"

double visit_literal(ast_t* node);
double visit_variable(ast_t* node);
double visit_unary(ast_t* node);
double visit_binary(ast_t* node);
double visit_assign(ast_t* node);
double visit_print(ast_t* node);

#endif
