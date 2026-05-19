// main.c
#include <stdio.h>
#include "expression.h"

// Declaration of our verifier function
double find_max_delta(Node* expression, double c, double L, double epsilon);

int main() {
    // Manually build the AST for f(x) = x * x
    Node* x1 = create_variable();
    Node* x2 = create_variable();
    Node* fx = create_operation(NODE_MULTIPLY, x1, x2);
    
    // Set up our Limit Problem parameters
    double c = 2.0;       // Target x point
    double L = 4.0;       // Expected limit value
    double epsilon = 0.01; // Error tolerance
    
    printf("Verifying: lim_{x -> %.2f} (x * x) = %.2f with epsilon = %f\n", c, L, epsilon);
    
    // Run the verifier
    double delta = find_max_delta(fx, c, L, epsilon);
    
    if (delta == 0.0) {
        printf("Verification failed or limit does not exist.\n");
    } else {
        printf("Maximum allowed Delta found: %f\n", delta);
        printf("Verification Successful! If |x - %.2f| < %f, then |f(x) - %.2f| < %f\n", 
               c, delta, L, epsilon);
    }
    
    // Free memory to prevent leaks
    free_tree(fx);
    
    return 0;
}