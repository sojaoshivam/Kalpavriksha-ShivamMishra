#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100


char operatorStack[MAX];
int top = -1;


int push(char c)
{
    if (top >= MAX - 1) 
    {
        return 0; 
    }
    operatorStack[++top] = c;
    return 1; 
}


int pop(char* c)
{
    if (top == -1) 
    {
        return 0; 
    }
    *c = operatorStack[top--];
    return 1; 
}

char peek()
{
    if (top == -1) return '\0';
    return operatorStack[top];
}

// Precedence of operators
int getOperatorPrecedence(char op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Validate input expression
int validateInfix(char* expr)
{
    int parenthesis = 0; // checks last processed op was a parentheses 0=), 1=(
    int lastWasOp = 1; // checks if the last processed operator was an opening parentheses
                       // or an operator "+-*/ , ( "
    int hasDigit = 0;

    for (int i = 0; expr[i]; i++) 
    {
        char c = expr[i];

        if (isspace(c)) 
        {
            continue;
        }

        if (isdigit(c)) 
        {
            lastWasOp = 0;
            hasDigit = 1;
        } else if (strchr("+-*/", c)) 
        {
            if (lastWasOp) {
                return 0; // wrong sequence
            }
            lastWasOp = 1;
        } else if (c == '(') 
        {
            parenthesis++;
            lastWasOp = 1; // Corrected from original: an operator can follow '('
        } else if (c == ')') 
        {
            if (parenthesis == 0 || lastWasOp) 
            {
                return 0; // unbalanced or wrong placed
            }
            parenthesis--;
            lastWasOp = 0;
        } else {
            return 0; // invalid expression
        }
    }

    if (parenthesis != 0 || lastWasOp || !hasDigit) 
    {
        return 0;
    }
    return 1;
}


int convertInfixToPostfix(char* infix, char* postfix)
{
    int k = 0;
    top = -1; // reset operator stack
    char popped_char;

    for (int i = 0; infix[i]; i++) 
    {
        char c = infix[i];

        if (isspace(c)) continue;

        // Number
        if (isdigit(c)) {
            while (isdigit(infix[i])) 
            {
                postfix[k++] = infix[i++];
            }
            postfix[k++] = ' ';
            i--;
        }
        // Unary minus/plus
        else if ((c == '+' || c == '-') && (i == 0 || infix[i-1]=='(' || strchr("+-*/", infix[i-1]))) 
        {
            postfix[k++] = '0';
            postfix[k++] = ' ';
            if (!push(c)) 
            {
                printf("Error: Stack overflow.\n");
                return 0;
            }
        }
        else if (c == '(') 
        {
            if (!push(c)) {
                printf("Error: Stack overflow.\n");
                return 0;
            }
        }
        else if (c == ')') 
        {
            while (peek() != '(') 
            {
                if (!pop(&popped_char)) 
                {
                    printf("Error: Mismatched parentheses.\n");
                    return 0;
                }
                postfix[k++] = popped_char;
                postfix[k++] = ' ';
            }
            pop(&popped_char); // remove '('
        }
        else if (strchr("+-*/", c)) {
            while (getOperatorPrecedence(peek()) >= getOperatorPrecedence(c)) {
                if(!pop(&popped_char)) break; // Stack is empty
                postfix[k++] = popped_char;
                postfix[k++] = ' ';
            }
            if (!push(c)) 
            {
                printf("Error: Stack overflow.\n");
                return 0;
            }
        }
        else {
            printf("Error: Invalid character '%c' in expression.\n", c);
            return 0;
        }
    }

    while (top != -1) 
    {
        if (peek() == '(') 
        {
            printf("Error: Mismatched parentheses.\n");
            return 0;
        }
        pop(&popped_char);
        postfix[k++] = popped_char;
        postfix[k++] = ' ';
    }

    postfix[k] = '\0';
    return 1;
}


int evaluatePostfix(char* postfix, int* result)
{
    int valueStack[MAX], valueTop = -1;

    for (int i = 0; postfix[i]; i++) 
    {
        if (isspace(postfix[i])) continue;

        if (isdigit(postfix[i])) 
        {
            int num = 0;
            while (isdigit(postfix[i])) num = num * 10 + (postfix[i++] - '0');
            valueStack[++valueTop] = num;
            i--;
        }
        else if (strchr("+-*/", postfix[i])) 
        {
            if (valueTop < 1) 
            {
                printf("Error: Invalid expression (missing operand).\n");
                return 0;
            }
            int b = valueStack[valueTop--];
            int a = valueStack[valueTop--];

            switch (postfix[i]) 
            {
                case '+': valueStack[++valueTop] = a + b; break;
                case '-': valueStack[++valueTop] = a - b; break;
                case '*': valueStack[++valueTop] = a * b; break;
                case '/':
                    if (b == 0) 
                    {
                        printf("Error: Division by zero.\n");
                        return 0;
                    }
                    valueStack[++valueTop] = a / b; break;
                default:
                    printf("Error: Unsupported operator '%c'.\n", postfix[i]);
                    return 0;
            }
        }
    }

    if (valueTop != 0) 
    {
        printf("Error: Invalid expression (extra operands/operators).\n");
        return 0;
    }

    *result = valueStack[valueTop];
    return 1;
}

// Main
int main()
{
    char infix[MAX], postfix[MAX * 2];

    printf("Enter expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = 0;

    if (!validateInfix(infix)) 
    {
        printf("Error: Invalid infix expression.\n");
        return 1;
    }

    if (!convertInfixToPostfix(infix, postfix)) 
    {
        // Error message is printed inside the function
        return 1;
    }

    int result;
    if (!evaluatePostfix(postfix, &result)) 
    {
        
        return 1;
    }

    printf("Result: %d\n", result);

    return 0;
}