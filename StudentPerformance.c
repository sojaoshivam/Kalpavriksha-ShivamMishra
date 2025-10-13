#include <stdio.h>
#include <string.h> 

#define MAX_STUDENTS 100
#define NUM_SUBJECTS 3
#define MAX_NAME_LEN 50


struct Student {
    int roll;
    char name[MAX_NAME_LEN];
    float marks[NUM_SUBJECTS];
    float total;
    float average;
    char grade;
};

// function prototypes (provides the compiler with enough information about the function so it can verify correctness)
void getStudentData(struct Student *s);
void calculateResults(struct Student *s);
char assignGrade(float average);
void printStudentReport(const struct Student *s);
void printAllRollNumbersRecursive(const struct Student students[], int count);



void getStudentData(struct Student *s) 
{
    printf("Enter Roll No: ");
    scanf("%d", &s->roll);

    printf("Enter Full Name: ");
    while (getchar() != '\n'); 
    fgets(s->name, MAX_NAME_LEN, stdin);
    s->name[strcspn(s->name, "\n")] = 0;

    printf("Enter Marks for %d subjects: ", NUM_SUBJECTS);
    for (int i = 0; i < NUM_SUBJECTS; i++) 
    {
        scanf("%f", &s->marks[i]);
    }
}

void calculateResults(struct Student *s)
{
    s->total = 0;
    for (int i = 0; i < NUM_SUBJECTS; i++) 
    {
        s->total += s->marks[i];
    }
    s->average = s->total / (float)NUM_SUBJECTS;
    s->grade = assignGrade(s->average);
}


char assignGrade(float average) 
{
    if (average >= 85) return 'A';
    if (average >= 70) return 'B';
    if (average >= 50) return 'C';
    if (average >= 35) return 'D';
    return 'F';
}


void printStudentReport(const struct Student *s) 
{
    printf("------------------------------------\n");
    printf("Roll       : %d\n", s->roll);
    printf("Name       : %s\n", s->name);
    printf("Total      : %.0f / %d\n", s->total, NUM_SUBJECTS * 100);
    printf("Average    : %.2f%%\n", s->average);
    printf("Grade      : %c\n", s->grade);

    if (s->grade != 'F') 
    {
        printf("Performance: ");
        for (int i = 0; i <= ('F' - s->grade - 1); i++) 
        {
            printf("*");
        }
        printf("\n");
    }
    printf("------------------------------------\n");

}

void printAllRollNumbersRecursive(const struct Student students[], int count) 
{
    if (count <= 0) 
    {
        return;
    }
    printAllRollNumbersRecursive(students, count - 1);
    printf("%d ", students[count - 1].roll);
}



int main() {
    struct Student students[MAX_STUDENTS];
    int numStudents = 0;

    printf("Enter the number of students (1-%d): ", MAX_STUDENTS);
    scanf("%d", &numStudents);


    while (getchar() != '\n');

    // Input validation
    if (numStudents < 1 || numStudents > MAX_STUDENTS) 
    {
        printf("Error: Invalid number of students. Please enter a value between 1 and %d.\n", MAX_STUDENTS);
        return 1; 
    }

    // --- Data Input & Processing Phase ---
    for (int i = 0; i < numStudents; i++) 
    {
        printf("\n--- Enter Details for Student %d ---\n", i + 1);
        getStudentData(&students[i]);
        calculateResults(&students[i]);
    }

    // --- Output Phase ---
    printf("\n\n--- Student Performance Report ---\n");
    for (int i = 0; i < numStudents; i++) 
    {
        printStudentReport(&students[i]);
    }
    
    // Recursive display of roll numbers
    printf("\nList of All Roll Numbers (via recursion): ");
    printAllRollNumbersRecursive(students, numStudents);
    printf("\n");

    return 0; // Success
}