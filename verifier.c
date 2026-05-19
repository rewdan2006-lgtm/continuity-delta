// verifier.c
#include <math.h>
#include <stdbool.h>
#include "expression.h"

#define NUM_SAMPLES 1000 // How closely we scan the interval

// Helper function to check if a specific delta satisfies epsilon
bool check_delta(Node* expression, double c, double L, double epsilon, double delta) {
    // Calculate the step size to scan across the interval (c - delta, c + delta)
    double step = (2.0 * delta) / NUM_SAMPLES;
    
    for (int i = 0; i <= NUM_SAMPLES; i++) {
        double x = (c - delta) + (i * step);
        
        // The limit definition says: 0 < |x - c|
        // So we skip evaluating exactly at x = c
        if (fabs(x - c) < 1e-9) continue; 
        
        double f_x = evaluate(expression, x);
        
        // If |f(x) - L| >= epsilon, this delta fails!
        if (fabs(f_x - L) >= epsilon) {
            return false; 
        }
    }
    return true; // All sampled points passed
}

// Use binary search to find the maximum possible delta
double find_max_delta(Node* expression, double c, double L, double epsilon) {
    double low = 0.0;
    double high = 2.0; // Initial maximum bound guess
    double max_delta = 0.0;
    
    // Binary search loop (precision tracking up to 7 decimal places)
    while ((high - low) > 1e-7) {
        double mid = low + (high - low) / 2.0;
        
        if (check_delta(expression, c, L, epsilon, mid)) {
            max_delta = mid; // This mid works! Let's see if we can go larger.
            low = mid;
        } else {
            high = mid; // Fails, must look at smaller deltas
        }
    }
    
    return max_delta;
}