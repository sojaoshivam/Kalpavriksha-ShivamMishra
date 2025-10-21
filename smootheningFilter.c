#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10


void printMatrix(int *mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", *(mat + i * n + j));
        }
        printf("\n");
    }
}


void rotateMatrix(int *mat, int n) {
    for (int layer = 0; layer < n / 2; layer++) {
        int first = layer;
        int last = n - 1 - layer;

        for (int j = first; j < last; j++) {
            int offset = j - first;

            
            int *top = mat + first * n + j;
            int *left = mat + (last - offset) * n + first;
            int *bottom = mat + last * n + (last - offset);
            int *right = mat + (first + offset) * n + last;

            
            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}


void smoothMatrix(int *mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0, count = 0;

           
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    int ni = i + x;
                    int nj = j + y;

                    
                    if (ni < 0 || nj < 0 || ni >= n || nj >= n)
                        continue;

                    int *p = mat + ni * n + nj;
                    sum += (*p & 0xFFFF); 
                    count++;
                }
            }

            int avg = sum / count;
            int *cur = mat + i * n + j;
            int oldVal = *cur & 0xFFFF;
            *cur = (avg << 16) | oldVal;
        }
    }

  
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int *p = mat + i * n + j;
            *p = (*p >> 16) & 0xFFFF;
        }
    }
}

int main() {
    int n;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &n);

    if (n < 2 || n > MAX) {
        printf("Invalid size!\n");
        return 1;
    }

    int *mat = (int *)malloc(n * n * sizeof(int));
    if (!mat) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    srand(1); 

   
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(mat + i * n + j) = rand() % 256;
        }
    }

    printf("\nOriginal Matrix:\n");
    printMatrix(mat, n);

    rotateMatrix(mat, n);
    printf("\nAfter 90° Clockwise Rotation:\n");
    printMatrix(mat, n);

    smoothMatrix(mat, n);
    printf("\nAfter 3×3 Smoothing Filter:\n");
    printMatrix(mat, n);

    free(mat);
    return 0;
}
