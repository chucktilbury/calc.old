/*
 * When the AST tree is evaluated, each node has code called in the "visitor"
 * pattern. These functions implement the node visits.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ast.h"
#include "error.h"
#include "symbols.h"

/*
 * Visit node with literal number.
 */
double visit_literal(ast_t* node) {

    msg(2, "Visit %s AST node", NT_TOSTR(node->type));
    if(node != NULL)
        return node->value.val->val;
    else {
        error("invalid literal node");
        return NAN; // Not A Number
    }

    return NAN; // unreachable
}

/*
 * Visit node with a variable from the symbol table.
 */
double visit_variable(ast_t* node) {

    msg(2, "Visit %s AST node", NT_TOSTR(node->type));
    if(node != NULL) {
        double val;
        const char* name = node->value.var->name;
        if(SYM_NO_ERROR == find_symbol(name, &val))
            return val;
        else {
            error("symbol \"%s\" is not defined", name);
            return NAN; // not a number
        }
    }
    else {
        error("invalid symbol node");
        return NAN; // not a number
    }

    return NAN; // unreachable
}

/*
 * Perform a unary operation.
 */
double visit_unary(ast_t* node) {

    msg(2, "Visit %s AST node", NT_TOSTR(node->type));
    if(node != NULL) {
        //node_type_t type = node->type;
        double val;
        if(node->left != NULL)
            val = node->left->visit(node->left);
        else {
            error("invalid unary child node");
            return NAN;
        }
        switch(node->value.op->op) {
            case PLUS_OP:
                return fabs(val);
            case MINUS_OP:
                return - val;
            default:
                error("invalid unary node type: %s", OP_TOSTR(node->value.op->op));
                return NAN;
        }
    }
    else {
        error("invalid unary node");
        return NAN; // not a number
    }

    return NAN; // unreachable
}

/*
 * Perform a binary operation.
 */
double visit_binary(ast_t* node) {

    msg(2, "Visit %s AST node", NT_TOSTR(node->type));
    if(node != NULL) {
        //node_type_t type = node->type;
        double left;
        double right;

        if(node->left != NULL)
            left = node->left->visit(node->left);
        else {
            error("invalid left binary child node");
            return NAN;
        }

        if(node->right != NULL)
            right = node->right->visit(node->right);
        else {
            error("invalid right binary child node");
            return NAN;
        }

        switch(node->value.op->op) {
            case PLUS_OP:  return  left + right;
            case MINUS_OP: return  left - right;
            case STAR_OP:  return  left * right;
            case SLASH_OP:
                if(right == 0.0) {
                    error("divide by zero");
                    return NAN;
                }
                return left / right;
            case PERCENT_OP:
                if(right == 0.0) {
                    error("divide by zero");
                    return NAN;
                }
                return fmod(left, right);
            default:
                error("invalid binary node type: %s", OP_TOSTR(node->value.op->op));
                return NAN;
        }
    }
    else {
        error("invalid binary node");
        return NAN; // not a number
    }

    return NAN; // unreachable
}

/*
 * Perform binary operation where the left item is the assign target and the
 * right item is the value to assign. This function causes the AST to be
 * traversed so that the assignment can be made.
 */
double visit_assign(ast_t* node) {

    msg(2, "Visit %s AST node", NT_TOSTR(node->type));
    if(node != NULL) {
        //node_type_t type = node->type;
        double left;    // identifier node
        double right;   // expression node
        const char* name;

        if(node->left != NULL) {
            name = node->left->value.var->name;
            if(SYM_NO_ERROR != find_symbol(name, &left)) {
                error("symbol \"%s\" is not defined", name);
                return NAN;
            }
        }
        else {
            error("invalid left assign child node");
            return NAN;
        }

        if(node->right != NULL)
            right = node->right->visit(node->right);
        else {
            error("invalid right assign child node");
            return NAN;
        }

        switch(node->value.op->op) {
            case ASSIGN_OP:
                if(SYM_NO_ERROR != assign_symbol(name, right)) {
                    error("symbol \"%s\" is not found", name);
                    return NAN;
                }
                else
                    return right;
            default:
                error("invalid assign node type: %s", OP_TOSTR(node->value.op->op));
                return NAN;
        }
    }
    else {
        error("invalid assign node");
        return NAN; // not a number
    }

    return NAN; // unreachable
}

/*
 * Perform a unary operation where the operation is "print". This traverses the
 * tree to obtain the value to print out.
 */
double visit_print(ast_t* node) {

    msg(2, "Visit %s AST node", NT_TOSTR(node->type));
    if(node != NULL) {
        //node_type_t type = node->type;
        double val;
        if(node->left != NULL)
            val = node->left->visit(node->left);
        else {
            error("invalid print child node");
            return NAN;
        }
        switch(node->value.op->op) {
            case PRINT_OP:
                //printf("%0.3f\n", val);
                return val;
            default:
                error("invalid print node type: %s", OP_TOSTR(node->value.op->op));
                return NAN;
        }
    }
    else {
        error("invalid print node");
        return NAN; // not a number
    }

    return NAN; // unreachable
}

