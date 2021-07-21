/*
 * This is the Abstract Syntax Tree that is generated by the parser as the
 * input is parsed. When the parser finishes, then this data structure is
 * returned in a global variable for further processing.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h> // for NAN definition

#include "ast.h"
#include "visit.h"
#include "memory.h"
#include "error.h"
#include "symbols.h"

int serial_number = 0;

static void print_node(ast_t* node) {

    switch(node->type) {
        case LITERAL_NODE:
            printf("literal value: %0.3f\n", node->value.val->val);
            break;
        case VARIABLE_NODE:
            find_symbol(node->value.var->name, &node->value.var->val);
            printf("variable name: %s, value: %0.3f\n", node->value.var->name, node->value.var->val);
            break;
        case UNARY_NODE:
            printf("unary node op: %s\n", OP_TOSTR(node->value.op->op));
            break;
        case BINARY_NODE:
            printf("binary node op: %s\n", OP_TOSTR(node->value.op->op));
            break;
        case ASSIGN_NODE:
            printf("assign node\n");
            break;
        case PRINT_NODE:
            printf("print node\n");
            break;
        default:
            error("unknown node type in print_node()");
    }
}

static void destroy_ast_node(ast_t* node) {

    switch(node->type) {
        case LITERAL_NODE:
            FREE(node->value.val);
            break;
        case VARIABLE_NODE:
            FREE((void*)node->value.var->name);
            FREE(node->value.var);
            break;
        case UNARY_NODE:
        case BINARY_NODE:
        case ASSIGN_NODE:
        case PRINT_NODE:
            FREE(node->value.op);
            break;
        default:
            error("unknown node type in destroy_ast_node()");
    }
    FREE(node);
}

/*
 * Create a literal number.
 */
ast_t* ast_literal(double val) {

    msg(2, "Create a literal AST node");
    ast_t* node = ALLOC_DS(ast_t);
    node->type = LITERAL_NODE;
    node->node_number = serial_number++;
    node->visit = visit_literal;
    node->left = NULL;
    node->right = NULL;

    node->value.val = ALLOC_DS(literal_node_t);
    node->value.val->val = val;

    return node;
}

/*
 * Create a variable reference in the tree.
 */
ast_t* ast_variable(const char* name) {

    msg(2, "Create a varible AST node");
    ast_t* node = ALLOC_DS(ast_t);
    node->type = VARIABLE_NODE;
    node->node_number = serial_number++;
    node->visit = visit_variable;
    node->left = NULL;
    node->right = NULL;

    node->value.var = ALLOC_DS(variable_node_t);
    node->value.var->name = name;
    // note that value and other fields are completed when the tree is
    // evaluated.

    return node;
}

/*
 * Create a unary operatlr node in the tree.
 */
ast_t* ast_unary(op_type_t op, ast_t* n) {

    msg(2, "Create a unary AST node");
    ast_t* node = ALLOC_DS(ast_t);
    node->type = UNARY_NODE;
    node->node_number = serial_number++;
    node->visit = visit_unary;
    node->left = n;
    node->right = NULL;

    node->value.op = ALLOC_DS(operator_node_t);
    node->value.op->op = op;

    return node;
}

/*
 * Create a binary operation node in the tree.
 */
ast_t* ast_binary(op_type_t op, ast_t* left, ast_t* right) {

    msg(2, "Create a binary AST node");
    ast_t* node = ALLOC_DS(ast_t);
    node->type = BINARY_NODE;
    node->node_number = serial_number++;
    node->visit = visit_binary;
    node->left = left;
    node->right = right;

    node->value.op = ALLOC_DS(operator_node_t);
    node->value.op->op = op;

    return node;
}

/*
 * Create an assignment node, which is a binary node that assigns to a variable
 * node in the tree.
 */
ast_t* ast_assign(const char* name, ast_t* tree) {

    msg(2, "Create a assign AST node");
    ast_t* node = ALLOC_DS(ast_t);
    node->type = BINARY_NODE;
    node->node_number = serial_number++;
    node->visit = visit_assign;
    node->left = ast_variable(name);
    node->right = tree;

    node->value.op = ALLOC_DS(operator_node_t);
    node->value.op->op = ASSIGN_OP;

    return node;
}

/*
 * Create a print node in the tree. This is a unary operation that uses the
 * print operator.
 */
ast_t* ast_print(ast_t* tree) {

    msg(2, "Create a print AST node");
    ast_t* node = ALLOC_DS(ast_t);
    node->type = UNARY_NODE;
    node->node_number = serial_number++;
    node->visit = visit_print;
    node->left = tree;
    node->right = NULL;

    node->value.op = ALLOC_DS(operator_node_t);
    node->value.op->op = PRINT_OP;

    return node;
}

/*
 * Free all of the memory associated with the AST.
 */
void destroy_ast(ast_t* root) {

    msg(1, "Destroy the AST");
    if(root->left != NULL)
        destroy_ast(root->left);

    if(root->right != NULL)
        destroy_ast(root->right);

    destroy_ast_node(root);
}

/*
 * Traverse the tree and return the numerical result of the expression.
 */
double traverse_ast(ast_t* root) {

    msg(1, "Traverse the AST");
    int errs = get_errors();
    if(errs == 0)
        return root->visit(root);
    else {
        printf("Errors: %d\n", errs);
        return NAN;
    }
}

/*
 * Save the expression to a dot file for use with Graphviz.
 */
void ast_to_dot(const char* fname) {
    // not implemented
    (void) fname;
}

/*
 * Show every node in the AST.
 */
void dump_ast(ast_t* root) {

    msg(0, "Dump the AST");
    if(root->left != NULL)
        dump_ast(root->left);

    if(root->right != NULL)
        dump_ast(root->right);

    print_node(root);
}
