#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// --- Operator stack ---
char opStack[MAX];
int opTop = -1;
void pushOp(char c) { opStack[++opTop] = c; }
char popOp() { return (opTop == -1) ? '\0' : opStack[opTop--]; }
char peekOp() { return (opTop == -1) ? '\0' : opStack[opTop]; }

// --- Value stack (for evaluation) ---
int valStack[MAX];
int valTop = -1;
void pushVal(int v) { valStack[++valTop] = v; }
int popVal() { return valStack[valTop--]; }

// Precedence
int prec(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Apply operator
int apply(int a, int b, char op) {
    switch (op) {
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': return a/b;
    }
    return 0;
}

// Evaluate infix directly via shunting-yard + eval
int evaluate(char* expr) {
    for (int i = 0; expr[i]; i++) {
        if (isspace(expr[i])) continue;

        if (isdigit(expr[i])) {  // multi-digit number
            int num = 0;
            while (isdigit(expr[i]))
                num = num*10 + (expr[i++]-'0');
            pushVal(num);
            i--;
        }
        else if (expr[i] == '(') pushOp(expr[i]);
        else if (expr[i] == ')') {
            while (peekOp() != '(') {
                int b = popVal(), a = popVal();
                pushVal(apply(a,b,popOp()));
            }
            popOp(); // remove '('
        }
        else { // operator
            while (prec(peekOp()) >= prec(expr[i])) {
                int b = popVal(), a = popVal();
                pushVal(apply(a,b,popOp()));
            }
            pushOp(expr[i]);
        }
    }

    while (opTop != -1) {
        int b = popVal(), a = popVal();
        pushVal(apply(a,b,popOp()));
    }

    return popVal();
}

// --- Main ---
int main() {
    char expr[MAX];
    printf("Enter expression: ");
    fgets(expr, MAX, stdin);
    expr[strcspn(expr, "\n")] = 0;

    printf("Result: %d\n", evaluate(expr));
    return 0;
}
