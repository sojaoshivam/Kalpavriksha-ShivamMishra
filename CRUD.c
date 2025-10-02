#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "user.txt"

struct user{
    int id;
    char name[40];
    int age;
};


// CREATE USER
void createUser(){
    FILE *fp = fopen(FILE_NAME, "a");
    if(!fp){
        printf("error opening file");
        return ;
    }

    struct user u;
    printf("Enter User Id : ");
    scanf("%d", &u.id);
    printf("Enter User Name : ");
    scanf("%s", &u.name);
    printf("Enter User Age : ");
    scanf("%d", &u.age);

    fprintf(fp, "%d %s %d\n" , u.id, u.name, u.age);
    fclose(fp);
    printf("User Addded to the FILE user.txt\n");
}

// DISPLAY USER
void displayUser() {
    FILE *fp = fopen( FILE_NAME, "r");
    
    struct user u;
    printf("\n--- User Records ---\n");
    while(fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3){
        printf("| ID : %d | Name : %s | Age : %d |\n", u.id, u.name, u.age);
    }
    fclose(fp);
}


// UPDATE USER
void updateUser(){
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if(!fp || !temp){
        printf("cant open file");
        return;
    }

    int targetId , isFound = 0;

    struct user u;

    printf("Enter the id of the user you wanna update : ");
    scanf("%d \n", &targetId);

    while(fscanf(fp, "%d %s %d\n", &u.id, u.name, &u.age) == 3){
        if(u.id == targetId){
            isFound = 1;
            printf("Enter new name : ");
            scanf("%s\n", &u.name);
            printf("Enter new Age : ");
            scanf("%d\n", &u.age);

        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    
    if(isFound){
        printf("User successfully updtad");
    }else{
        printf("There is no user with that id");
    }
}

//DELETE USER 
void deleteUser(){
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if(!fp || !temp){
        printf("cant open file");
        return;
    }

    int targetId , isFound = 0;

    struct user u;

    printf("Enter the id of the user you wanna DELETE : ");
    scanf("%d \n", &targetId);

    while(fscanf(fp, "%d %s %d\n", &u.id, u.name, &u.age) == 3){
        if(u.id == targetId){
            isFound = 1;
           continue; // SKIP THIS RECORE SO IT DOSENT COPIES TO THE NEW FILE

        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    
    if(isFound){
        printf("User successfully DELETED");
    }else{
        printf("There is no user with that id");
    }
}




int main() {
    int choice;
    do {
        printf("\n--- User Management ---\n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: displayUser(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}