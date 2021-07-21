/**
 * Symbols are stored in a binary tree because it's quick and easy. There
 * will never more than a couple of hundred symbols, one would think.
 * Different storage and retrieval techniques are not going to make much
 * difference.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "memory.h"
#include "error.h"
#include "symbols.h"

typedef struct _ste_t_ {
    const char* name;
    double value;
    bool is_assigned;
    struct _ste_t_* left;
    struct _ste_t_* right;
} symbol_table_t;

// global symbol table
static symbol_table_t* root = NULL;

/*
 * Duplicate the string.
 */
static const char* dupstr(const char* str) {

    size_t len = strlen(str) + 1;
    const char* buf = ALLOC(len);
    memcpy((void*)buf, (void*)str, len);
    return buf;
}

/**
 * Recursively add the symbol to the tree.
 */
static symbols_error_t recursive_add(symbol_table_t* root, symbol_table_t* node) {

    if(strcmp(root->name, node->name) < 0) {
        if(root->left == NULL) {
            root->left = node;
            return SYM_NO_ERROR;
        }
        else
            return recursive_add(root->left, node);
    }
    else if(strcmp(root->name, node->name) > 0) {
        if(root->right == NULL) {
            root->right = node;
            return SYM_NO_ERROR;
        }
        else
            return recursive_add(root->right, node);
    }
    else {
        return SYM_EXISTS;  // node exists.
    }
}

/**
 * Recursively find a symbol in the tree.
 */
static symbol_table_t* recursive_find(symbol_table_t* root, const char* name) {

    if(root != NULL) {
        if(strcmp(root->name, name) < 0) {
            if(root->left == NULL)
                return NULL;
            else
                return recursive_find(root->left, name);

        }
        else if(strcmp(root->name, name) > 0) {
            if(root->right == NULL)
                return NULL;
            else
                return recursive_find(root->right, name);
        }
        else
            return root;
    }

    return NULL;
}

/**
 * Recursively dump the symbol table.
 */
static void recursive_dump(symbol_table_t* node) {

    if(node != NULL) {
        if(node->left != NULL) {
            recursive_dump(node->left);
        }

        if(node->right != NULL) {
            recursive_dump(node->right);
        }

        if(node->is_assigned)
            printf("name: %s value = %0.3f\n", node->name, node->value);
        else
            printf("name: %s value = not assigned\n", node->name);
    }
}

/**
 * Add a symbol to the tree. If it already exists, return !0, else
 * return 0. If the value is valid, then set the flag to true.
 */
symbols_error_t add_symbol(const char* name) { //, double val, bool flag) {

    symbol_table_t* node = ALLOC_DS(symbol_table_t);
    node->name = dupstr(name);
    //node->value = val;
    //node->is_assigned = flag;

    if(root == NULL) {
        root = node;
        return SYM_NO_ERROR;
    }
    else
        return recursive_add(root, node);
}

/**
 * Assign a value to the symbol. If it is not found then return !0. Else
 * return 0.
 */
symbols_error_t assign_symbol(const char* name, double val) {

    symbol_table_t* sym = recursive_find(root, name);
    if(sym != NULL) {
        sym->value = val;
        sym->is_assigned = true;
        return SYM_NO_ERROR;
    }

    return SYM_NOT_FOUND;
}

/**
 * Find a symbol in the tree. If it exists and has been assigned, then return
 * a pointer to the value associated with it. Otherwise, return NULL.
 */
symbols_error_t find_symbol(const char* name, double* val) {

    symbol_table_t* sym = recursive_find(root, name);
    if(sym != NULL) {
        if(val != NULL)
            *val = sym->value;
        return SYM_NO_ERROR;
    }

    return SYM_NOT_FOUND;
}

symbols_error_t symbol_is_assigned(const char* name) {

    symbol_table_t* sym = recursive_find(root, name);
    if(sym != NULL) {
        if(sym->is_assigned)
            return SYM_NO_ERROR;
        else
            return SYM_NOT_ASSIGNED;
    }

    return SYM_NOT_FOUND;
}

/**
 * Traverse the tree and print each symbol with it's current value.
 */
void dump_symbols() {

    printf("Dump symbol table\n");
    recursive_dump(root);
}
