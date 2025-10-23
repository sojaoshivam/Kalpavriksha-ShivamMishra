#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_MATRIX_SIZE 10
#define MIN_MATRIX_SIZE 2
#define MAX_PIXEL_VALUE 255
#define RANDOM_SEED 1


void displayMatrix(int *matrix, int size);
void rotateMatrixClockwise(int *matrix, int size);
void applySmoothingFilter(int *matrix, int size);
void generateRandomMatrix(int *matrix, int size);
int getValidatedMatrixSize(void);



// Prints the NxN matrix
void displayMatrix(int *matrix, int size) 
{
    for (int row = 0; row < size; row++) 
    {
        for (int col = 0; col < size; col++) 
        {
            printf("%4d ", *(matrix + row * size + col));
        }
        printf("\n");
    }
}

// Rotates the matrix 90° clockwise
void rotateMatrixClockwise(int *matrix, int size) 
{
    for (int layer = 0; layer < size / 2; layer++) 
    {
        int firstIndex = layer;
        int lastIndex = size - 1 - layer;

        for (int col = firstIndex; col < lastIndex; col++) 
        {
            int offset = col - firstIndex;

            int *topCell = matrix + firstIndex * size + col;
            int *leftCell = matrix + (lastIndex - offset) * size + firstIndex;
            int *bottomCell = matrix + lastIndex * size + (lastIndex - offset);
            int *rightCell = matrix + (firstIndex + offset) * size + lastIndex;

            int tempValue = *topCell;
            *topCell = *leftCell;
            *leftCell = *bottomCell;
            *bottomCell = *rightCell;
            *rightCell = tempValue;
        }
    }
}

// Applies a 3×3 smoothing filter
void applySmoothingFilter(int *matrix, int size) 
{
    // Encode new average in higher 16 bits
    for (int row = 0; row < size; row++) 
    {
        for (int col = 0; col < size; col++) 
        {
            int sum = 0, neighborCount = 0;

            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) 
            {
                for (int deltaCol = -1; deltaCol <= 1; deltaCol++) 
                {
                    int neighborRow = row + deltaRow;
                    int neighborCol = col + deltaCol;

                    if (neighborRow < 0 || neighborCol < 0 || 
                        neighborRow >= size || neighborCol >= size)
                        continue;

                    int *neighborPtr = matrix + neighborRow * size + neighborCol;
                    sum += (*neighborPtr & 0xFFFF);
                    neighborCount++;
                }
            }

            // Use rounded average instead of integer division
            double average = (double)sum / neighborCount;
            int roundedAvg = (int)(average + 0.5); // rounding fix
            int *currentPtr = matrix + row * size + col;
            int oldValue = *currentPtr & 0xFFFF;
            *currentPtr = (roundedAvg << 16) | oldValue;
        }
    }

    // Decode new averaged values
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            int *cellPtr = matrix + row * size + col;
            *cellPtr = (*cellPtr >> 16) & 0xFFFF;
        }
    }
}

// Generate random NxN matrix
void generateRandomMatrix(int *matrix, int size) {
    srand(RANDOM_SEED);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            *(matrix + row * size + col) = rand() % (MAX_PIXEL_VALUE + 1);
        }
    }
}

// Validate numeric input and range
int getValidatedMatrixSize(void) 
{
    int matrixSize;
    printf("Enter matrix size (2-10): ");

    // Validate direct integer input
    if (scanf("%d", &matrixSize) != 1) 
    {
        printf("Invalid input! Please enter a numeric value.\n");
        while (getchar() != '\n'); // clear input buffer
        return -1;
    }

    if (matrixSize < 2 || matrixSize > MAX_MATRIX_SIZE) 
    {
        printf("Invalid size! Please enter a value between 2 and 10.\n");
        return -1;
    }

    return matrixSize;
}

// ---------- Main ----------
int main(void) {
    int matrixSize = getValidatedMatrixSize();

    int *matrix = (int *)malloc(matrixSize * matrixSize * sizeof(int));
    if (!matrix) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    generateRandomMatrix(matrix, matrixSize);

    printf("\nOriginal Matrix:\n");
    displayMatrix(matrix, matrixSize);

    rotateMatrixClockwise(matrix, matrixSize);
    printf("\nAfter 90° Clockwise Rotation:\n");
    displayMatrix(matrix, matrixSize);

    applySmoothingFilter(matrix, matrixSize);
    printf("\nAfter 3×3 Smoothing Filter:\n");
    displayMatrix(matrix, matrixSize);

    free(matrix);
    return 0;
}
