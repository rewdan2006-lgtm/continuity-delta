// parser.c
#include <stdio.h>
#include <stdlib.h>
#include "expression.h"

// Allocate memory for a constant node
Node* create_constant(double val) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_CONSTANT;
    node->value = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Allocate memory for the variable 'x'
Node* create_variable() {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_VARIABLE;
    node->value = 0.0; // Placeholder
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Allocate memory for an operation (+, -, *, /)
Node* create_operation(NodeType type, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->value = 0.0;
    node->left = left;
    node->right = right;
    return node;
}

// Recursively evaluate the tree for a given value of x
double evaluate(Node* node, double x_val) {
    if (node == NULL) return 0.0;

    switch (node->type) {
        case NODE_CONSTANT: 
            return node->value;
        case NODE_VARIABLE: 
            return x_val; // Swap 'x' with the actual number we are testing
        case NODE_ADD: 
            return evaluate(node->left, x_val) + evaluate(node->right, x_val);
        case NODE_SUBTRACT: 
            return evaluate(node->left, x_val) - evaluate(node->right, x_val);
        case NODE_MULTIPLY: 
            return evaluate(node->left, x_val) * evaluate(node->right, x_val);
        case NODE_DIVIDE: 
            return evaluate(node->left, x_val) / evaluate(node->right, x_val);
    }
    return 0.0;
}

// Crucial for Valgrind! Clean up all allocated memory recursively.
void free_tree(Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}