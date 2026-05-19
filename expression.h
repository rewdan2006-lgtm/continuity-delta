// expression.h
#ifndef EXPRESSION_H
#define EXPRESSION_H

// 1. Identify what kind of node we are looking at
typedef enum {
    NODE_CONSTANT, // e.g., 2.0, 5.5
    NODE_VARIABLE, // x
    NODE_ADD,      // +
    NODE_SUBTRACT, // -
    NODE_MULTIPLY, // *
    NODE_DIVIDE    // /
} NodeType;

// 2. The Node struct. Each node can have a left and right child.
typedef struct Node {
    NodeType type;
    double value;          // Only used if type == NODE_CONSTANT
    struct Node* left;     // Left child pointer
    struct Node* right;    // Right child pointer
} Node;

// Helper functions to build nodes
Node* create_constant(double val);
Node* create_variable();
Node* create_operation(NodeType type, Node* left, Node* right);

// Core functions
double evaluate(Node* node, double x_val);
void free_tree(Node* node);

#endif