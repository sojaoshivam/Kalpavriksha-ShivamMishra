#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

// Global operator stack
char operatorStack[MAX];
int top = -1;

void push(char c)
{
    if (top >= MAX - 1) {
        printf("Error: Stack overflow.\n");
        exit(1);
    }
    operatorStack[++top] = c;
}

char pop()
{
    if (top == -1) {
        printf("Error: Stack underflow.\n");
        exit(1);
    }
    return operatorStack[top--];
}

char peek()
{
    if (top == -1) return '\0';
    return operatorStack[top];
}

// Precedence of operators
int getPrecedence(char op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Validate input expression
int validateInfix(char* expr)
{
    int parenthesis = 0; // checks last processed op was a parentheses 0=), 1=(

    int lastWasOp=1; // checks if the last processed operator was a opening parentheses
                     // or a operator "+-*/ , ( "
    int hasDigit=0;

    for( int i=0; expr[i]; i++){
        char c = expr[i];

        if(isspace(c))
        {
            continue;
        }

        if(isdigit(c))
        {
            lastWasOp = 0;
            hasDigit = 1;
        }

        else if(strchr("+-*/", c))
        {
            if(lastWasOp)
            {
                return 0; // wrong sequence
            }
            lastWasOp = 1;
        }

        else if( c == '(')
        {
            parenthesis++;
            lastWasOp = 0;
        }

        else if( c == ')')
        {
            if(parenthesis == 0 || lastWasOp)
            {
                return 0; // unbalanced or wrong placed 
            }
            parenthesis--;
            lastWasOp=0;
        }

        else {
            return 0;// invalid expression
        }

    }

    if(parenthesis != 0 || lastWasOp || !hasDigit)
    {
        return 0;
    }
    return 1;
}

// Convert infix to postfix 
void convertInfixToPostfix(char* infix, char* postfix)
{
    int k = 0;
    top = -1; // reset operator stack

    for (int i = 0; infix[i]; i++) 
    {
        char c = infix[i];

        if (isspace(c)) continue;

        // Number
        if (isdigit(c))
        {
            while (isdigit(infix[i])) 
            {
                postfix[k++] = infix[i++];
            }
            postfix[k++] = ' ';
            i--;
        }
        // Unary minus
        else if ((c == '+' || c == '-') && (i == 0 || infix[i-1]=='(' || strchr("+-*/", infix[i-1]))) 
        {
            postfix[k++] = '0';
            push(c);
        }
        else if (c == '(') push(c);
        else if (c == ')') 
        {
            while (peek() != '(') {
                postfix[k++] = pop();
                postfix[k++] = ' ';
                if (top == -1) {
                    printf("Error: Mismatched parentheses.\n");
                    exit(1);
                }
            }
            pop(); // remove '('
        }
        else if (strchr("+-*/", c)) 
        {
            while (getPrecedence(peek()) >= getPrecedence(c)) {
                postfix[k++] = pop();
                postfix[k++] = ' ';
            }
            push(c);
        }
        else 
        {
            printf("Error: Invalid character '%c' in expression.\n", c);
            exit(1);
        }
    }

    while (top != -1) 
    {
        if (peek() == '(') 
        {
            printf("Error: Mismatched parentheses.\n");
            exit(1);
        }
        postfix[k++] = pop();
        postfix[k++] = ' ';
    }

    postfix[k] = '\0';
}

// Evaluate postfix
int evaluatePostfix(char* postfix)
{
    int valueStack[MAX], valueTop = -1;

    for (int i = 0; postfix[i]; i++) 
    {
        if (isspace(postfix[i])) continue;

        if (isdigit(postfix[i])) 
        {         
            int num = 0;
            while (isdigit(postfix[i])) num = num*10 + (postfix[i++]-'0');
            valueStack[++valueTop] = num;
            i--;
        }
        else if (strchr("+-*/", postfix[i])) 
        {
            if (valueTop < 1) 
            {
                printf("Error: Invalid expression (missing operand).\n");
                exit(1);
            }
            int b = valueStack[valueTop--];
            int a = valueStack[valueTop--];

            switch(postfix[i]) 
            {
                case '+': valueStack[++valueTop] = a + b; break;
                case '-': valueStack[++valueTop] = a - b; break;
                case '*': valueStack[++valueTop] = a * b; break;
                case '/':
                    if (b == 0) 
                    {
                        printf("Error: Division by zero.\n");
                        exit(1);
                    }
                    valueStack[++valueTop] = a / b; break;
                default:
                    printf("Error: Unsupported operator '%c'.\n", postfix[i]);
                    exit(1);
            }
        }
    }

    if (valueTop != 0) 
    {
        printf("Error: Invalid expression (extra operands/operators).\n");
        exit(1);
    }

    return valueStack[valueTop];
}

// Main
int main()
{
    char infix[MAX], postfix[MAX*2];

    printf("Enter expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = 0;

    if (!validateInfix(infix)) 
    {
        printf("Error: Invalid expression.\n");
        return 1;
    }

    convertInfixToPostfix(infix, postfix);
    

    int result = evaluatePostfix(postfix);
    printf("Result: %d\n", result);

    return 0;
}
