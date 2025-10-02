#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

// Operator stack
char stack[MAX];
int top = -1;
void push(char c) { stack[++top] = c; }
char pop() { return (top == -1) ? '\0' : stack[top--]; }
char peek() { return (top == -1) ? '\0' : stack[top]; }

// Precedence function
int prec(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Convert Infix → Postfix
void infixToPostfix(char* infix, char* postfix) {
    int k = 0;
    for (int i = 0; infix[i]; i++) {
        if (isdigit(infix[i])) { // numbers
            while (isdigit(infix[i])) 
                postfix[k++] = infix[i++];
            postfix[k++] = ' ';
            i--; // adjust
        }
        else if (infix[i] == '(') push(infix[i]);
        else if (infix[i] == ')') {
            while (peek() != '(') postfix[k++] = pop(), postfix[k++] = ' ';
            pop(); // remove '('
        }
        else { // operator
            while (prec(peek()) >= prec(infix[i]))
                postfix[k++] = pop(), postfix[k++] = ' ';
            push(infix[i]);
        }
    }
    while (top != -1) postfix[k++] = pop(), postfix[k++] = ' ';
    postfix[k] = '\0';
}

// Evaluate Postfix
int evalPostfix(char* postfix) {
    int vals[MAX], vtop = -1;
    for (int i = 0; postfix[i]; i++) {
        if (isdigit(postfix[i])) {
            int num = 0;
            while (isdigit(postfix[i])) num = num*10 + (postfix[i++]-'0');
            vals[++vtop] = num;
        }
        else if (strchr("+-*/", postfix[i])) {
            int b = vals[vtop--], a = vals[vtop--];
            switch (postfix[i]) {
                case '+': vals[++vtop] = a+b; break;
                case '-': vals[++vtop] = a-b; break;
                case '*': vals[++vtop] = a*b; break;
                case '/': vals[++vtop] = a/b; break;
            }
        }
    }
    return vals[vtop];
}

// Main
int main() {
    char infix[MAX], postfix[MAX*2];
    printf("Enter expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = 0;

    infixToPostfix(infix, postfix);
    printf("Postfix: %s\n", postfix);

    printf("Result: %d\n", evalPostfix(postfix));
    return 0;
}
