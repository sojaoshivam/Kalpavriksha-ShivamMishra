#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NUM_SUBJECTS 3
#define MAX_NAME_LEN 50

enum Grade { A, B, C, D, F };

struct Student {
    int roll;
    char name[MAX_NAME_LEN];
    float marks[NUM_SUBJECTS];
    float total;
    float average;
    enum Grade grade;
};


int getNumberOfStudents();
void inputAllStudents(struct Student students[], int count);
void printAllReports(const struct Student students[], int count);
void printAllRollNumbersRecursive(const struct Student students[], int count);

void getStudentData(struct Student *s);
void calculateResults(struct Student *s);
enum Grade assignGrade(float average); 
void printStudentReport(const struct Student *s);


int getNumberOfStudents() 
{
    int num;
    while (1) 
    {
        printf("Enter the number of students (1-%d): ", MAX_STUDENTS);

        // Make sure the input is a valid number.
        if (scanf("%d", &num) != 1) 
        {
            printf("Oops! That's not a number. Please try again.\n");
            while (getchar() != '\n'); // Clear the bad input.
        }
        // Make sure the number is within our defined limits.
        else if (num < 1 || num > MAX_STUDENTS) 
        {
            printf("Please enter a number between 1 and %d.\n", MAX_STUDENTS);
            while (getchar() != '\n'); // Clear any extra characters.
        }
       
        else 
        {
            while (getchar() != '\n'); 
            return num;
        }
    }
}


void getStudentData(struct Student *s) 
{
    // --- Get Roll Number ---
    while (1) 
    {
        printf("Enter Roll No: ");
        if (scanf("%d", &s->roll) != 1) 
        {
            printf("Invalid input! Please enter a numeric roll number.\n");
            while (getchar() != '\n'); // Clear buffer.
        } else if (s->roll <= 0) 
        {
            printf("Roll number must be a positive number.\n");
        } else 
        {
            while (getchar() != '\n'); // Clear trailing newline for the next input.
            break;
        }
    }

    // --- Get Name ---
    printf("Enter Full Name: ");
    fgets(s->name, MAX_NAME_LEN, stdin);
    s->name[strcspn(s->name, "\n")] = '\0'; 

    // --- Get Marks ---
    printf("Enter Marks for %d subjects (0-100):\n", NUM_SUBJECTS);
    for (int i = 0; i < NUM_SUBJECTS; i++) 
    {
        while (1) 
        {
            printf("  Subject %d: ", i + 1);
            if (scanf("%f", &s->marks[i]) != 1) 
            {
                printf("  Invalid input! Please enter numeric marks.\n");
                while (getchar() != '\n');
            } else if (s->marks[i] < 0 || s->marks[i] > 100) 
            {
                printf("  Marks must be between 0 and 100.\n");
            } else 
            {
                break; // Input is valid.
            }
        }
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


enum Grade assignGrade(float average) 
{
    if (average >= 85) return A;
    if (average >= 70) return B;
    if (average >= 50) return C;
    if (average >= 35) return D;
    return F;
}



void inputAllStudents(struct Student students[], int count) 
{
    for (int i = 0; i < count; i++) 
    {
        printf("\n--- Enter Details for Student %d ---\n", i + 1);
        getStudentData(&students[i]);
        calculateResults(&students[i]); // Calculate results right after getting data.
    }
}


void printAllReports(const struct Student students[], int count) 
{
    printf("\n\n--- Student Performance Reports ---\n");
    for (int i = 0; i < count; i++) {
        printStudentReport(&students[i]);
    }
}


void printStudentReport(const struct Student *s) 
{
    printf("------------------------------------\n");
    printf(" Roll       : %d\n", s->roll);
    printf(" Name       : %s\n", s->name);
    printf(" Total      : %.0f / %d\n", s->total, NUM_SUBJECTS * 100);
    printf(" Average    : %.2f%%\n", s->average);
    
   
    printf(" Grade      : ");
    switch (s->grade) {
        case A: printf("A"); break;
        case B: printf("B"); break;
        case C: printf("C"); break;
        case D: printf("D"); break;
        case F: printf("F"); break;
    }
    printf("\n");


    printf(" Performance: ");
    switch(s->grade) {
        case A: printf("***** (Excellent)"); break;
        case B: printf("**** (Good)");      break;
        case C: printf("*** (Average)");   break;
        case D: printf("** (Needs Work)");break;
        case F: printf("* (Fail)");      break;
    }
    printf("\n");
    printf("------------------------------------\n");
}


void printAllRollNumbersRecursive(const struct Student students[], int count) 
{
   
    if (count <= 0) 
    {
        return;
    }
   
    printAllRollNumbers(students, count - 1);
   
    printf("%d ", students[count - 1].roll);
}

int main() 
{
    struct Student students[MAX_STUDENTS];

    printf("Welcome to the Student Performance analyzer \n");
    printf("=========================================\n");

    int numStudents = getNumberOfStudents();

    // --- Data Entry and Processing ---
    inputAllStudents(students, numStudents);

  
    printAllReports(students, numStudents);

   
    printf("\nAll Student Roll Numbers: ");
    printAllRollNumbers(students, numStudents);
    printf("\n"); 

    return 0;
}