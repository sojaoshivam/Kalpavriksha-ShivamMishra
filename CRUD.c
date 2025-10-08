#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "user.txt"

struct user{
    int id;
    char name[50];
    int age;
};


//helping function to identify if the numeric id 
int isNumeric(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// CREATE USER
void createUser(){
    FILE *fp = fopen(FILE_NAME, "a");
     if(!fp){
        printf("Error : Unable to open file '%s'\n", FILE_NAME);
        return;
    }

    struct user u;
    char id[20], age[20];

    printf("Enter User Id : ");
    scanf("%s", id);
    if(!isNumeric(id)) // now it checks the whole id if its numeric or not
    { 
        printf("Invalid user ID. It must contain only digits \n");
        fclose(fp);
        return;
    }
    u.id = atoi(id);
    
    getchar(); // clear newline from input buffer
    
    printf("Enter User Name : ");
    if(!fgets(u.name, sizeof(u.name), stdin)){
        printf("Error reading the name. \n");
        fclose(fp);
        return;
    }
    u.name[strcspn(u.name, "\n")] = 0; //remove trailing newline

    if(strlen(u.name) == 0){
        printf("Error: Name cannot be empty .\n");
        fclose(fp);
        return;
    }

    printf("Enter User Age: ");
    scanf("%s", age);
    if (!isNumeric(age)) {
        printf("Invalid Age. Must contain only digits.\n");
        fclose(fp);
        return;
    }
    u.age = atoi(age);
    if (u.age < 0) {
        printf("Error: Age cannot be negative.\n");
        fclose(fp);
        return;
    }

    // quoted name to support spaces 
    fprintf(fp, "%d \"%s\" %d\n", u.id, u.name, u.age);
    fclose(fp);
    printf("User Addded to the file '%s' \n", FILE_NAME);
}

// DISPLAY USER
void displayUser() {
    FILE *fp = fopen( FILE_NAME, "r");
    
    if(!fp)
    {
        printf("Error : Unable to open file '%s'\n", FILE_NAME);
        return;
    }

    struct user u;
    int isFound = 0;

    printf("\n--- User Records ---\n");
   while (fscanf(fp, "%d \"%49[^\"]\" %d", &u.id, u.name, &u.age) == 3) 
   {
        printf("| ID: %d | Name: %s | Age: %d |\n", u.id, u.name, u.age);
        isFound = 1;
    }
    if(!isFound){
        printf("No users found. \n");
    }
    fclose(fp);
}


// UPDATE USER
void updateUser(){

    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if(!fp || !temp)
    {
        printf("cant open file");
        if(fp) fclose(fp);
        if(temp) fclose(temp);
        return;
    }

    char id[20], age[20];
    int targetId , isFound = 0;
    struct user u;

    printf("Enter the user Id to update : ");
    scanf("%s", &id);
    if(!isNumeric(id))
    {
         printf("Invalid ID. Must be numeric.\n");
        fclose(fp);
        fclose(temp);
        return;
    }

    targetId = atoi(id);
    
    getchar();

  
while (fscanf(fp, "%d \"%49[^\"]\" %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == targetId) {
            isFound = 1;

            printf("Enter new name: ");
            fgets(u.name, sizeof(u.name), stdin);
            u.name[strcspn(u.name, "\n")] = 0;
            if (strlen(u.name) == 0) {
                printf("Error: Name cannot be empty. Keeping old name.\n");
            }

            printf("Enter new age: ");
            scanf("%s", age);
            if (!isNumeric(age)) {
                printf("Invalid age. Keeping old age.\n");
            } else {
                u.age = atoi(age);
            }
        }
        fprintf(temp, "%d \"%s\" %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    
    if(isFound){
        printf("User ID %d successfully updated. \n", targetId);
    }else{
        printf("There is no user with Id %d. \n", targetId);
    }
}

//DELETE USER 
void deleteUser(){
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if(!fp || !temp)
    {
        printf("Error: Unable to open files for deletion.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    int targetId , isFound = 0;

    struct user u;

    printf("Enter the id of the user you wanna DELETE : ");
    scanf("%d", &targetId);

    while (fscanf(fp, "%d \"%49[^\"]\" %d", &u.id, u.name, &u.age) == 3){
        if(u.id == targetId){
            isFound = 1;
           continue; // SKIP THIS RECORD SO IT DOSENT COPIES TO THE NEW FILE

        }
        fprintf(temp, "%d \"%s\" %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    
    if(isFound){
        printf("User with ID %d deleted successfully.\n", targetId);
    }else{
        printf("No user found with ID %d.\n",targetId);
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
