/*
 * The AST data structure has one type for all nodes. The data for the node is
 * kept in an internal union.
 */
#ifndef __AST_H__
#define __AST_H__

#include <stdbool.h>

typedef enum {
    PLUS_OP,
    MINUS_OP,
    STAR_OP,
    SLASH_OP,
    PERCENT_OP,
    ASSIGN_OP,
    PRINT_OP,
} op_type_t;

#define OP_TOSTR(o) (\
    ((o) == PLUS_OP)? "PLUS_OP" : \
    ((o) == MINUS_OP)? "MINUS_OP" : \
    ((o) == STAR_OP)? "STAR_OP" : \
    ((o) == SLASH_OP)? "SLASH_OP" : \
    ((o) == PERCENT_OP)? "PERCENT_OP" : \
    ((o) == ASSIGN_OP)? "ASSIGN_OP" : \
    ((o) == PRINT_OP)? "PRINT_OP" : "UNKNOWN")

typedef enum {
    LITERAL_NODE,   // val
    VARIABLE_NODE,  // var
    UNARY_NODE,     // op
    BINARY_NODE,    // op
    ASSIGN_NODE,    // op
    PRINT_NODE,     // op
} node_type_t;

#define NT_TOSTR(n) (\
    ((n) == VARIABLE_NODE)? "VARIABLE_NODE" : \
    ((n) == UNARY_NODE)? "UNARY_NODE" : \
    ((n) == LITERAL_NODE)? "LITERAL_NODE" : \
    ((n) == BINARY_NODE)? "BINARY_NODE" : \
    ((n) == ASSIGN_NODE)? "ASSIGN_NODE" : \
    ((n) == PRINT_NODE)? "PRINT_NODE" : "UNKNOWN" )

typedef struct {
    double val;
} literal_node_t;

typedef struct {
    bool is_assigned;
    const char* name;
    double val;
} variable_node_t;

typedef struct {
    op_type_t op;
    double val;
} operator_node_t;

typedef struct _ast_t_ {
    node_type_t type;
    struct _ast_t_* left;
    struct _ast_t_* right;
    double (*visit)(struct _ast_t_*);
    int node_number; // for dot generation
    union {
        literal_node_t* val;
        variable_node_t* var;
        operator_node_t* op;
    } value;
} ast_t;

ast_t* ast_literal(double val);
ast_t* ast_variable(const char* name);
ast_t* ast_unary(op_type_t op, ast_t* node);
ast_t* ast_binary(op_type_t op, ast_t* left, ast_t* right);
ast_t* ast_assign(const char* name, ast_t* tree);
ast_t* ast_print(ast_t* tree);

void destroy_ast(ast_t* root);
double traverse_ast(ast_t* root);
void ast_to_dot(const char* fname);
void dump_ast(ast_t* root);

#endif
