#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NAME_LEN 51
#define MAX_INITIAL 100 

typedef struct {
    int id;
    char name[50];
    float prices;
    int quantity;
}Products;


/*--Helper functions--*/

// clearing input buffer avoiding leftover ch from breaking future scanf/fgets calls
void clearInputBuffer() {
    int c;
     while((c = getchar()) != '\n' && c != EOF);
}

// reading a full line
void readString(char *buffer, int size){
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

/*-- FUNCTION PROTOTYPES--*/

void displayMenu();
void addProduct(Products **inventory, int *productCount);
void viewProducts(const Products *inventory, int productCount);
void updateQuantity(Products *inventory, int productCount);
void searchById(const Products *inventory, int productCount);
void searchByName(const Products *inventory, int productCount);
void searchByPriceRange(const Products *inventory, int productCount);
void deleteProduct(Products *inventory, int *productCount);


int main(){
    Products *inventory = NULL;
    int productCount = 0;
    int initialSize = 0;
    int choice = 0;

    printf("Enter initial number of products: ");
    while(scanf("%d", &initialSize)!=1 || initialSize <=0 )
    {
        printf("Invalid Input Please enter a positive number : ");
        clearInputBuffer();
    }
    clearInputBuffer();// Clear the newline after scanf

    // allocating memory 
    inventory = (Products *)calloc(initialSize, sizeof(Products));

    if (inventory == NULL) {
        printf("Error: Memory allocation failed. Exiting.\n");
        return 1; // Exit with an error code
    }

    productCount = initialSize;

    // getting details for the initial products 
    for(int i = 0; i< productCount; i++){
        printf("\nEnter details for product %d:\n", i+1);

        //PRODUCT ID
        printf("Product ID: ");
        while (scanf("%d", &inventory[i].id) != 1)
        {
            printf("Invalid input. Enter an integer ID: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        // PRODUCT NAME
        printf("Product Name: ");
        readString(inventory[i].name, 51);

        // PRODUCT PRICE
        printf("Product Price: ");
        while (scanf("%d", &inventory[i].prices) != 1)
        {
            printf("Invalid input. Enter an valid price: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        //PRODUCT QUANTITY
        printf("Product Quantity: ");
        while (scanf("%d", &inventory[i].quantity) != 1) 
        {
            printf("Invalid input. Enter an integer quantity: ");
            clearInputBuffer();
        }
        clearInputBuffer();  
    } // for loop ends

    // Menu Loop 
    do
    {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            choice = 0;
        }
        clearInputBuffer(); 

        switch (choice)
        {
        case 1:
            addProduct(&inventory, &productCount);
            break;
            case 2:
                viewProducts(inventory, productCount);
                break;
            case 3:
                updateQuantity(inventory, productCount);
                break;
            case 4:
                searchById(inventory, productCount);
                break;
            case 5:
                searchByName(inventory, productCount);
                break;
            case 6:
                searchByPriceRange(inventory, productCount);
                break;
            case 7:
                deleteProduct(inventory, &productCount);
                break;
            case 8:
                printf("\nMemory released successfully. Exiting program...\n");
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 8.\n");
                break;
        }
    } while (choice != 8);

    free(inventory);
    inventory = NULL;
    return 0;
    
}


/*-- FUNCTIONS --*/
void displayMenu() {
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
    printf("==================================\n");
}

void addProduct(Products **inventory, int *productCount) {
    // Increase the size by 1 for the new product
    int newCount = *productCount + 1;

    // Use realloc() to resize the memory block.
    // *inventory holds the current memory address.
    Products *temp = (Products *)realloc(*inventory, newCount * sizeof(Products));

    if (temp == NULL) {
        printf("Error: Memory reallocation failed. Cannot add product.\n");
        // *inventory still points to the old, valid memory block
        return;
    }

    *inventory = temp;

    // Get a pointer to the newly allocated space at the end of the array
    Products *newProduct = &(*inventory)[*productCount]; // Index is the old count

    printf("\nEnter new product details:\n");
    printf("Product ID: ");
    while (scanf("%d", &newProduct->id) != 1) {
        printf("Invalid input. Enter an integer ID: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Product Name: ");
    readString(newProduct->name, 51);

    printf("Product Price: ");
    while (scanf("%f", &newProduct->prices) != 1) {
        printf("Invalid input. Enter a valid price: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Product Quantity: ");
    while (scanf("%d", &newProduct->quantity) != 1) {
        printf("Invalid input. Enter an integer quantity: ");
        clearInputBuffer();
    }
    clearInputBuffer();


    *productCount = newCount;
    printf("Product added successfully!\n");
}

void viewProducts(const Products *inventory, int productCount) {
    printf("\n========= PRODUCT LIST =========\n");
    if (productCount == 0) {
        printf("No products in inventory.\n");
    } else {
        for (int i = 0; i < productCount; i++) {
            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].prices,
                   inventory[i].quantity);
        }
    }
}

void updateQuantity(Products *inventory, int productCount) {
    int targetId;
    int newQuantity;
    int found = 0;

    printf("Enter Product ID to update quantity: ");
    while (scanf("%d", &targetId) != 1) {
        printf("Invalid input. Enter an integer ID: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == targetId) {
            printf("Enter new Quantity: ");
            while (scanf("%d", &newQuantity) != 1) {
                printf("Invalid input. Enter an integer quantity: ");
                clearInputBuffer();
            }
            clearInputBuffer();

            inventory[i].quantity = newQuantity;
            found = 1;
            printf("Quantity updated successfully!\n");
            break; // Stop searching once found
        }
    }

    if (!found) {
        printf("Product ID %d not found.\n", targetId);
    }
}


void searchById(const Products *inventory, int productCount) {
    int targetId;
    int found = 0;

    printf("Enter Product ID to search: ");
    while (scanf("%d", &targetId) != 1) {
        printf("Invalid input. Enter an integer ID: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == targetId) {
            printf("Product Found:\n");
            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].prices,
                   inventory[i].quantity);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Product ID %d not found.\n", targetId);
    }
}

void searchByName(const Products *inventory, int productCount) {
    char searchName[51];
    int found = 0;

    printf("Enter name to search (partial allowed): ");
    readString(searchName, 51);

    for (int i = 0; i < productCount; i++) {
        // strstr() checks if searchName is a substring of inventory[i].name
        if (strstr(inventory[i].name, searchName) != NULL) {
            if (found == 0) {
                // Print header only on the first match
                printf("\nProducts Found:\n");
            }
            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].prices,
                   inventory[i].quantity);
            found = 1;
        }
    }

    if (!found) {
        printf("No products found matching '%s'.\n", searchName);
    }
}

void searchByPriceRange(const Products *inventory, int productCount) {
    float minPrice, maxPrice;
    int found = 0;

    printf("Enter minimum price: ");
    while (scanf("%f", &minPrice) != 1) {
        printf("Invalid input. Enter a valid price: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Enter maximum price: ");
    while (scanf("%f", &maxPrice) != 1 || maxPrice < minPrice) {
        printf("Invalid input. Enter a valid price greater than or equal to the minimum: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    for (int i = 0; i < productCount; i++) {
        if (inventory[i].prices >= minPrice && inventory[i].prices <= maxPrice) {
            if (found == 0) {
                printf("\nProducts in price range (%.2f - %.2f):\n", minPrice, maxPrice);
            }
            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].prices,
                   inventory[i].quantity);
            found = 1;
        }
    }

    if (!found) {
        printf("No products found in that price range.\n");
    }
}

void deleteProduct(Products *inventory, int *productCount) {
    int targetId;
    int foundIndex = -1; // Index of the product to delete

    printf("Enter Product ID to delete: ");
    while (scanf("%d", &targetId) != 1) {
        printf("Invalid input. Enter an integer ID: ");
        clearInputBuffer();
    }
    clearInputBuffer();

   
    for (int i = 0; i < *productCount; i++) {
        if (inventory[i].id == targetId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Product ID %d not found. Nothing to delete.\n", targetId);
        return;
    }

    for (int i = foundIndex; i < *productCount - 1; i++) {
        inventory[i] = inventory[i + 1];
    }

   
    (*productCount)--;

    printf("Product deleted successfully!\n");

   
}