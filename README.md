# Automated Delta-Epsilon ($\delta$-$\epsilon$) Limit Verifier

An algorithmic bridge between pure real analysis and low-level system software. This project replaces the traditional, human-centric algebraic approximations of limits with a rigorous, computer-driven numerical verification engine written in pure C. 

By representing mathematical functions as dynamic **Abstract Syntax Trees (ASTs)** and applying a bounded **Binary Search** optimization, this tool calculates the maximum allowable input boundary ($\delta$) for any given output tolerance ($\epsilon$) down to seven decimal places of precision.

---

## 1. Theoretical Framework & Mathematical Rigor

In real analysis, the formal definition of a limit ($\lim_{x \to c} f(x) = L$) states that for every error tolerance $\epsilon > 0$, there exists a corresponding boundary $\delta > 0$ such that:

$$\text{If } 0 < |x - c| < \delta, \text{ then } |f(x) - L| < \epsilon$$

Geometrically, this represents a game of mathematical containment. If an adversary challenges you with a horizontal bounding box of height $2\epsilon$ centered around the target limit $L$, you must find a vertical bounding box of width $2\delta$ centered around $c$ such that the function never escapes through the top or bottom of the horizontal corridor.

### The Analytical vs. Algorithmic Gap
When evaluating a limit like $\lim_{x \to 2} x^2 = 4$ by hand, a student typically uses algebra to establish a coarse, linear upper bound—such as choosing $\delta = \min(1, \frac{\epsilon}{5fixed})$. While mathematically valid, this analytical approach heavily underestimates the true geometric capacity of the curve.

This program approaches the problem from a **numerical scanning perspective**. Instead of guessing conservative linear constraints, it samples the actual curve behavior at the microscopic level, locating the *exact non-linear geometric threshold* where the function breaks the $\epsilon$-barrier.

---

## 2. Requirements & Installation

To compile and run this multi-file C project, you need a C compiler (GCC) and standard development utilities. 

### Windows (via MSYS2)
1. Download and install **MSYS2** from [msys2.org](https://www.msys2.org/).
2. Open the **MSYS2 UCRT64** terminal and install the GCC toolchain, GDB debugger, and development utilities by running:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb base-devel
   

```

3. Add the compiler binary folder to your Windows **System Environment Variables (Path)**:
```text
C:\msys64\ucrt64\bin


```



```

### macOS / Linux
* **macOS:** Install the Xcode command-line tools via terminal: `xcode-select --install`
* **Linux (Ubuntu/Debian):** Install the build tools via terminal: `sudo apt update && sudo apt install build-essential`

---

## 3. Compilation & Execution

Clone this repository, navigate to the project root directory, and compile using the following command in your terminal:

```bash
gcc main.c parser.c verifier.c -o verifier -lm

```

*Note: The `-lm` flag links the math library (`<math.h>`) required for absolute precision tracking.*

Run the compiled binary:

```bash
./verifier

```

---

## 4. Software Architecture & Implementation Process

Because the C programming language lacks native object-oriented capabilities, the project is structured around low-level memory layout control, strict pointer manipulation, and multi-file compilation.

```text
delta_epsilon_verifier/
├── expression.h   # Data structures and core function signatures
├── parser.c       # AST memory allocation, tracking, and evaluation
├── verifier.c     # Numerical binary search engine
└── main.c         # Interface and problem execution context

```

### Module I: Abstract Syntax Tree (AST) & Evaluation

A computer cannot naturally interpret a raw string like $x \cdot x$. To evaluate functions dynamically across thousands of sample points without hardcoding equations, the software builds an expression tree in memory using nested structures and self-referential pointers.

```c
typedef enum {
    NODE_CONSTANT, 
    NODE_VARIABLE, 
    NODE_ADD,      
    NODE_SUBTRACT, 
    NODE_MULTIPLY, 
    NODE_DIVIDE    
} NodeType;

typedef struct Node {
    NodeType type;
    double value;          
    struct Node* left;     
    struct Node* right;    
} Node;

```

* **Recursive Post-Order Evaluation:** Evaluating the tree requires a depth-first traversal. To determine the value of a parent operation node (like `NODE_ADD`), the program recursively dives to the bottom of the left branch, evaluates it, dives to the right branch, evaluates it, and then combines them at the parent level.
* **Heap Allocation & Post-Order Cleanup:** Every single mathematical node is dynamically allocated on the heap via `malloc`. To prevent critical memory leaks, a recursive `free_tree` function acts from the bottom up—ensuring child nodes are completely freed before their parent addresses are erased from memory.

### Module II: Bounded Binary Search Engine

Finding the maximum possible $\delta$ presents a search optimization problem. A naive linear search would be far too computationally expensive to achieve high precision. Instead, the engine treats $\delta$ as a bounded optimization variable between $0.0$ and a safe maximum ceiling ($2.0$).

1. **The Core Guard Condition ($x \neq c$):** The rigorous limit definition specifies $0 < |x - c|$. Therefore, when the scanner evaluates points in the interval $(c - \delta, c + \delta)$, it explicitly skips checking exactly at $x = c$. This ensures the verifier handles removable discontinuities (holes in graphs) with perfect mathematical accuracy.
2. **Binary Search Convergence:** The engine tests the midpoint of its $\delta$ bounds. If every point inside that midpoint's interval satisfies $|f(x) - L| < \epsilon$, the current guess is saved as working, and the lower bound is moved upward. If even a single point breaches the tolerance, the upper bound shrinks. This cuts the search space in half with every iteration, converging at $O(\log N)$ speed.

---

## 5. Results & Empirical Verification

To test the engine, we evaluated the classic quadratic limit problem:

$$\lim_{x \to 2} x^2 = 4 \quad \text{with an assigned error tolerance of } \epsilon = 0.01$$

### Runtime Output

```text
Verifying: lim_{x -> 2.00} (x * x) = 4.00 with epsilon = 0.010000
Maximum allowed Delta found: 0.002498
Verification Successful! If |x - 2.00| < 0.002498, then |f(x) - 4.00| < 0.010000

```

### Analytical Proof Validation

Let's verify the software's output against exact calculus. We want to find the exact point where the right-hand side of our interval breaches the upper epsilon ceiling:

$$f(c + \delta) = L + \epsilon$$

$$(2 + \delta)^2 = 4 + 0.01$$

$$2 + \delta = \sqrt{4.01}$$

$$\delta = \sqrt{4.01} - 2 \approx 0.002498437$$

The automated verifier converged on **$0.002498$**, matching the true analytical threshold up to **six decimal places of accuracy**.

---

## 6. Key Engineering Insights

* **Memory Safety & Control:** Through structural engineering of self-referential pointers and rigorous post-order traversal logic, every byte of heap-allocated memory utilized during the generation of the AST is systematically recovered upon completion, resulting in a zero-leak footprint.
* **Floating-Point Awareness:** Floating-point operations inherently introduce precision limitations. By implementing a custom absolute difference threshold (`1e-9`) rather than strict equality evaluations (`== 0`), the logic completely bypasses standard machine epsilon inaccuracies when dealing with coordinates infinitely close to $c$.

```

