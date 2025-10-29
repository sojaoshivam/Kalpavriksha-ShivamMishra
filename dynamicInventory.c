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
void flushInput() {
    int tempChar;
    while ((tempChar = getchar()) != '\n' && tempChar != EOF);
}

// reading a full line
void readString(char *buffer, int size){
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

/*-- FUNCTION PROTOTYPES--*/

void displayMenu();
void inputProductDetails(Products *p);
void addProduct(Products **inventory, int *productCount);
void viewProducts(const Products *inventory, int productCount);
void updateQuantity(Products *inventory, int productCount);
void searchById(const Products *inventory, int productCount);
void searchByName(const Products *inventory, int productCount);
void searchByPriceRange(const Products *inventory, int productCount);
void deleteProduct(Products **inventory, int *productCount);



int main(){
    Products *inventory = NULL;
    int productCount = 0;
    int initialSize = 0;
    int choice = 0;

    printf("Enter initial number of products: ");
    while(scanf("%d", &initialSize)!=1 || initialSize <=0 )
    {
        printf("Invalid Input Please enter a positive number : ");
        flushInput();
    }
    flushInput();// Clear the newline after scanf

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
        inputProductDetails(&inventory[i]);// made a function to avoid repetetive input 
    }

    // Menu Loop 
    do
    {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            choice = 0;
        }
        flushInput(); 

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
                deleteProduct(&inventory, &productCount);
                break;
            case 8:
                free(inventory);
                inventory = NULL;
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

void inputProductDetails(Products *p) {
    // Product ID
    printf("Enter Product ID: ");
    while (scanf("%d", &(p->id)) != 1 || p->id <= 0) {
        printf("Invalid input. Enter a positive integer ID: ");
        flushInput();
    }
    flushInput();

    // Product Name
    printf("Enter Product Name: ");
    readString(p->name, NAME_LEN);
    

    // Product Price
    printf("Enter Product Price: ");
    
    while (scanf("%f", &(p->prices)) != 1 || p->prices < 0 || p->prices > 100000) {
        printf("Invalid input. Enter a valid price: ");
        flushInput();
    }
    flushInput();

    // Product Quantity
    printf("Enter Product Quantity: ");
    while (scanf("%d", &(p->quantity)) != 1 || p->quantity < 0) {
        printf("Invalid input. Enter a valid quantity: ");
        flushInput();
    }
    flushInput();
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
    inputProductDetails(newProduct);
    *productCount = newCount;
    printf("Product added successfully!\n");
}

void viewProducts(const Products *inventory, int productCount) {
    printf("\n========= PRODUCT LIST =========\n");
    if (productCount == 0) {
        printf("No products in inventory.\n");
    } else {
        const Products *ptr = inventory; // pointer iterator
        for (int i = 0; i < productCount; i++, ptr++) {
            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   ptr->id,
                   ptr->name,
                   ptr->prices,
                   ptr->quantity);
        }
    }
}


void updateQuantity(Products *inventory, int productCount) {
    int targetId, newQuantity, found = 0;

    printf("Enter Product ID to update quantity: ");
    while (scanf("%d", &targetId) != 1) {
        printf("Invalid input. Enter an integer ID: ");
        flushInput();
    }
    flushInput();

    Products *ptr = inventory;
    for (int i = 0; i < productCount; i++, ptr++) {
        if (ptr->id == targetId) {
            printf("Enter new Quantity: ");
            while (scanf("%d", &newQuantity) != 1) {
                printf("Invalid input. Enter an integer quantity: ");
                flushInput();
            }
            flushInput();

            ptr->quantity = newQuantity;
            found = 1;
            printf("Quantity updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Product ID %d not found.\n", targetId);
    }
}



void searchById(const Products *inventory, int productCount) {
    int targetId, found = 0;

    printf("Enter Product ID to search: ");
    while (scanf("%d", &targetId) != 1) {
        printf("Invalid input. Enter an integer ID: ");
        flushInput();
    }
    flushInput();

    const Products *ptr = inventory;
    for (int i = 0; i < productCount; i++, ptr++) {
        if (ptr->id == targetId) {
            printf("Product Found:\n");
            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   ptr->id,
                   ptr->name,
                   ptr->prices, 
                   ptr->quantity
                );
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Product ID %d not found.\n", targetId);
    }
}
void searchByName(const Products *inventory, int productCount) {
    char searchName[NAME_LEN];
    int found = 0;

    printf("Enter name to search (partial allowed): ");
    readString(searchName, NAME_LEN);

    const Products *ptr = inventory;
    for (int i = 0; i < productCount; i++, ptr++) {
        if (strstr(ptr->name, searchName) != NULL) {
            if (!found)
                printf("\nProducts Found:\n");

            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   ptr->id, 
                   ptr->name, 
                   ptr->prices, 
                   ptr->quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No products found matching '%s'.\n", searchName);
}


void searchByPriceRange(const Products *inventory, int productCount) {
    float minPrice, maxPrice;
    int found = 0;

    printf("Enter minimum price: ");
    while (scanf("%f", &minPrice) != 1) {
        printf("Invalid input. Enter a valid price: ");
        flushInput();
    }
    flushInput();

    printf("Enter maximum price: ");
    while (scanf("%f", &maxPrice) != 1 || maxPrice < minPrice) {
        printf("Invalid input. Enter a valid price greater than or equal to the minimum: ");
        flushInput();
    }
    flushInput();

    const Products *ptr = inventory;
    for (int i = 0; i < productCount; i++, ptr++) {
        if (ptr->prices >= minPrice && ptr->prices <= maxPrice) {
            if (!found)
                printf("\nProducts in price range (%.2f - %.2f):\n", minPrice, maxPrice);

            printf("Product ID: %-5d | Name: %-20s | Price: %-8.2f | Quantity: %d\n",
                   ptr->id, ptr->name, ptr->prices, ptr->quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No products found in that price range.\n");
}


void deleteProduct(Products **inventory, int *productCount) {
    if (inventory == NULL || *productCount == 0 || *inventory == NULL) {
        printf("No products available to delete.\n");
        return;
    }

    int targetId;
    printf("Enter Product ID to delete: ");
    while (scanf("%d", &targetId) != 1) {
        printf("Invalid input. Enter an integer ID: ");
        flushInput();
    }
    flushInput();

    int foundIndex = -1;
    for (int i = 0; i < *productCount; ++i) {
        if ((*inventory)[i].id == targetId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Product ID %d not found. Nothing to delete.\n", targetId);
        return;
    }

    // Shift elements left to overwrite the deleted product
    for (int i = foundIndex; i < *productCount - 1; ++i) {
        (*inventory)[i] = (*inventory)[i + 1];
    }

    // Decrease the count
    (*productCount)--;

    // If no products left, free and set pointer to NULL
    if (*productCount == 0) {
        free(*inventory);
        *inventory = NULL;
        printf("Product deleted successfully! Inventory is now empty.\n");
        return;
    }

    // Otherwise shrink the array
    Products *tmp = (Products *)realloc(*inventory, (*productCount) * sizeof(Products));
    if (tmp == NULL) {
        // If realloc fails, the original block is still valid; keep it but warn user.
        printf("Warning: memory reallocation failed after deletion. Inventory remains allocated.\n");
        // Do not set *inventory to NULL here because realloc failed.
    } else {
        *inventory = tmp;
    }
    printf("Product deleted successfully!\n");
}

