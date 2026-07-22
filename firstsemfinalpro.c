#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_PRODUCTS 1000
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_PRODUCT_NAME 100
#define USER_FILE "users.txt"
#define PRODUCT_FILE "products.txt"

// User structure
typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} User;

// Product structure
typedef struct {
    int id;
    char name[MAX_PRODUCT_NAME];
    int quantity;
    float price;
} Product;

// Global variables
User users[MAX_USERS];
Product products[MAX_PRODUCTS];
int user_count = 0;
int product_count = 0;
int current_user_index = -1;

// Function prototypes
void clear_screen();
void pause_screen();
int load_users();
int save_users();
int load_products();
int save_products();
void signup();
int login();
void main_menu();
void add_product();
void view_products();
void update_product();
void delete_product();
void search_product();
int is_valid_username(const char* username);
int is_valid_password(const char* password);
int find_user(const char* username);
int find_product(int id);
void trim_whitespace(char* str);

// Clear screen function
void clear_screen() {
    #ifdef _WIN32
        system("cls");
        system("color 1F");        
    #else
        system("clear");
    #endif
}

// Pause screen function
void pause_screen() {
    printf("\nPress Enter to continue...");
    getchar();
}

// Trim whitespace from string
void trim_whitespace(char* str) {
    char* end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    // All spaces?
    if(*str == 0) return;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    end[1] = '\0';
}

// Load users from file
int load_users() {
    FILE* file = fopen(USER_FILE, "r");
    if (file == NULL) {
        return 0; // File doesn't exist yet
    }
    
    user_count = 0;
    while (fscanf(file, "%s %s", users[user_count].username, users[user_count].password) == 2) {
        user_count++;
        if (user_count >= MAX_USERS) break;
    }
    
    fclose(file);
    return 1;
}

// Save users to file
int save_users() {
    FILE* file = fopen(USER_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save users!\n");
        return 0;
    }
    
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    
    fclose(file);
    return 1;
}

// Load products from file
int load_products() {
    FILE* file = fopen(PRODUCT_FILE, "r");
    if (file == NULL) {
        return 0; // File doesn't exist yet
    }
    
    product_count = 0;
    while (fscanf(file, "%d %s %d %f", &products[product_count].id, 
                  products[product_count].name, &products[product_count].quantity, 
                  &products[product_count].price) == 4) {
        product_count++;
        if (product_count >= MAX_PRODUCTS) break;
    }
    
    fclose(file);
    return 1;
}

// Save products to file
int save_products() {
    FILE* file = fopen(PRODUCT_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save products!\n");
        return 0;
    }
    
    for (int i = 0; i < product_count; i++) {
        fprintf(file, "%d %s %d %.2f\n", products[i].id, products[i].name, 
                products[i].quantity, products[i].price);
    }
    
    fclose(file);
    return 1;
}

// Validate username
int is_valid_username(const char* username) {
    if (strlen(username) < 3 || strlen(username) > MAX_USERNAME - 1) {
        return 0;
    }
    
    for (int i = 0; username[i]; i++) {
        if (!isalnum(username[i]) && username[i] != '_') {
            return 0;
        }
    }
    
    return 1;
}

// Validate password
int is_valid_password(const char* password) {
    if (strlen(password) < 4 || strlen(password) > MAX_PASSWORD - 1) {
        return 0;
    }
    return 1;
}

// Find user by username
int find_user(const char* username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Find product by ID
int find_product(int id) {
    for (int i = 0; i < product_count; i++) {
        if (products[i].id == id) {
            return i;
        }
    }
    return -1;
}

// User signup
void signup() {
    clear_screen();
    printf("=== SIGN UP ===\n");
    
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    
    printf("Enter username (3-49 characters, alphanumeric and underscore only): ");
    fgets(username, sizeof(username), stdin);
    trim_whitespace(username);
    
    if (!is_valid_username(username)) {
        printf("Invalid username! Must be 3-49 characters, alphanumeric and underscore only.\n");
        pause_screen();
        return;
    }
    
    if (find_user(username) != -1) {
        printf("Username already exists!\n");
        pause_screen();
        return;
    }
    
    printf("Enter password (4-49 characters): ");
    fgets(password, sizeof(password), stdin);
    trim_whitespace(password);
    
    if (!is_valid_password(password)) {
        printf("Invalid password! Must be 4-49 characters.\n");
        pause_screen();
        return;
    }
    
    if (user_count >= MAX_USERS) {
        printf("Maximum users reached!\n");
        pause_screen();
        return;
    }
    
    // Add new user
    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    user_count++;
    
    if (save_users()) {
        printf("User registered successfully!\n");
    } else {
        printf("Error saving user data!\n");
    }
    
    pause_screen();
}

// User login
int login() {
    clear_screen();
    printf("=== LOGIN ===\n");
    
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    trim_whitespace(username);
    
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    trim_whitespace(password);
    
    int user_index = find_user(username);
    
    if (user_index == -1) {
        printf("User not found!\n");
        pause_screen();
        return 0;
    }
    
    if (strcmp(users[user_index].password, password) != 0) {
        printf("Invalid password!\n");
        pause_screen();
        return 0;
    }
    
    current_user_index = user_index;
    printf("Login successful! Welcome, %s!\n", username);
    pause_screen();
    return 1;
}

// Add product
void add_product() {
    clear_screen();
    printf("=== ADD PRODUCT ===\n");
    
    if (product_count >= MAX_PRODUCTS) {
        printf("Maximum products reached!\n");
        pause_screen();
        return;
    }
    
    int id;
    char name[MAX_PRODUCT_NAME];
    int quantity;
    float price;
    
    printf("Enter product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID format!\n");
        pause_screen();
        return;
    }
    getchar(); // consume newline
    
    if (find_product(id) != -1) {
        printf("Product ID already exists!\n");
        pause_screen();
        return;
    }
    
    printf("Enter product name: ");
    fgets(name, sizeof(name), stdin);
    trim_whitespace(name);
    
    if (strlen(name) == 0) {
        printf("Product name cannot be empty!\n");
        pause_screen();
        return;
    }
    
    printf("Enter quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity < 0) {
        printf("Invalid quantity!\n");
        pause_screen();
        return;
    }
    
    printf("Enter price: ");
    if (scanf("%f", &price) != 1 || price < 0) {
        printf("Invalid price!\n");
        pause_screen();
        return;
    }
    getchar(); // consume newline
    
    // Add product
    products[product_count].id = id;
    strcpy(products[product_count].name, name);
    products[product_count].quantity = quantity;
    products[product_count].price = price;
    product_count++;
    
    if (save_products()) {
        printf("Product added successfully!\n");
    } else {
        printf("Error saving product data!\n");
    }
    
    pause_screen();
}

// View all products
void view_products() {
    clear_screen();
    printf("=== PRODUCT INVENTORY ===\n");
    
    if (product_count == 0) {
        printf("No products in inventory!\n");
        pause_screen();
        return;
    }
    
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Quantity", "Price");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < product_count; i++) {
        printf("%-5d %-20s %-10d $%-9.2f\n", 
               products[i].id, products[i].name, 
               products[i].quantity, products[i].price);
    }
    
    pause_screen();
}

// Update product
void update_product() {
    clear_screen();
    printf("=== UPDATE PRODUCT ===\n");
    
    if (product_count == 0) {
        printf("No products to update!\n");
        pause_screen();
        return;
    }
    
    int id;
    printf("Enter product ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID format!\n");
        pause_screen();
        return;
    }
    getchar(); // consume newline
    
    int index = find_product(id);
    if (index == -1) {
        printf("Product not found!\n");
        pause_screen();
        return;
    }
    
    printf("Current product details:\n");
    printf("ID: %d\n", products[index].id);
    printf("Name: %s\n", products[index].name);
    printf("Quantity: %d\n", products[index].quantity);
    printf("Price: $%.2f\n", products[index].price);
    
    char name[MAX_PRODUCT_NAME];
    int quantity;
    float price;
    
    printf("\nEnter new name (or press Enter to keep current): ");
    fgets(name, sizeof(name), stdin);
    trim_whitespace(name);
    
    if (strlen(name) > 0) {
        strcpy(products[index].name, name);
    }
    
    printf("Enter new quantity (current: %d): ", products[index].quantity);
    if (scanf("%d", &quantity) == 1 && quantity >= 0) {
        products[index].quantity = quantity;
    }
    
    printf("Enter new price (current: $%.2f): ", products[index].price);
    if (scanf("%f", &price) == 1 && price >= 0) {
        products[index].price = price;
    }
    getchar(); // consume newline
    
    if (save_products()) {
        printf("Product updated successfully!\n");
    } else {
        printf("Error saving product data!\n");
    }
    
    pause_screen();
}

// Delete product
void delete_product() {
    clear_screen();
    printf("=== DELETE PRODUCT ===\n");
    
    if (product_count == 0) {
        printf("No products to delete!\n");
        pause_screen();
        return;
    }
    
    int id;
    printf("Enter product ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID format!\n");
        pause_screen();
        return;
    }
    getchar(); // consume newline
    
    int index = find_product(id);
    if (index == -1) {
        printf("Product not found!\n");
        pause_screen();
        return;
    }
    
    printf("Product to delete:\n");
    printf("ID: %d\n", products[index].id);
    printf("Name: %s\n", products[index].name);
    printf("Quantity: %d\n", products[index].quantity);
    printf("Price: $%.2f\n", products[index].price);
    
    char confirm;
    printf("\nAre you sure you want to delete this product? (y/n): ");
    scanf("%c", &confirm);
    getchar(); // consume newline
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift products to fill the gap
        for (int i = index; i < product_count - 1; i++) {
            products[i] = products[i + 1];
        }
        product_count--;
        
        if (save_products()) {
            printf("Product deleted successfully!\n");
        } else {
            printf("Error saving product data!\n");
        }
    } else {
        printf("Delete cancelled.\n");
    }
    
    pause_screen();
}

// Search product
void search_product() {
    clear_screen();
    printf("=== SEARCH PRODUCT ===\n");
    
    if (product_count == 0) {
        printf("No products to search!\n");
        pause_screen();
        return;
    }
    
    printf("Search by:\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("Enter choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice!\n");
        pause_screen();
        return;
    }
    getchar(); // consume newline
    
    if (choice == 1) {
        int id;
        printf("Enter product ID: ");
        if (scanf("%d", &id) != 1) {
            printf("Invalid ID format!\n");
            pause_screen();
            return;
        }
        getchar(); // consume newline
        
        int index = find_product(id);
        if (index == -1) {
            printf("Product not found!\n");
        } else {
            printf("\nProduct found:\n");
            printf("ID: %d\n", products[index].id);
            printf("Name: %s\n", products[index].name);
            printf("Quantity: %d\n", products[index].quantity);
            printf("Price: $%.2f\n", products[index].price);
        }
    } else if (choice == 2) {
        char search_name[MAX_PRODUCT_NAME];
        printf("Enter product name (or part of it): ");
        fgets(search_name, sizeof(search_name), stdin);
        trim_whitespace(search_name);
        
        int found = 0;
        printf("\nSearch results:\n");
        printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Quantity", "Price");
        printf("---------------------------------------------------\n");
        
        for (int i = 0; i < product_count; i++) {
            if (strstr(products[i].name, search_name) != NULL) {
                printf("%-5d %-20s %-10d $%-9.2f\n", 
                       products[i].id, products[i].name, 
                       products[i].quantity, products[i].price);
                found = 1;
            }
        }
        
        if (!found) {
            printf("No products found matching '%s'\n", search_name);
        }
    } else {
        printf("Invalid choice!\n");
    }
    
    pause_screen();
}

// Main menu
void main_menu() {
    int choice;
    
    while (1) {
        clear_screen();
        printf("=== INVENTORY MANAGEMENT SYSTEM ===\n");
        printf("Welcome, %s!\n\n", users[current_user_index].username);
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Search Product\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            pause_screen();
            continue;
        }
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                add_product();
                break;
            case 2:
                view_products();
                break;
            case 3:
                update_product();
                break;
            case 4:
                delete_product();
                break;
            case 5:
                search_product();
                break;
            case 6:
                printf("Logged out successfully!\n");
                current_user_index = -1;
                return;
            default:
                printf("Invalid choice!\n");
                pause_screen();
        }
    }
}

// Main function
int main() {
    // Load existing data
    load_users();
    load_products();
    
    int choice;
    
    while (1) {
        clear_screen();
        printf("=== INVENTORY MANAGEMENT SYSTEM ===\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            pause_screen();
            continue;
        }
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                signup();
                break;
            case 2:
                if (login()) {
                    main_menu();
                }
                break;
            case 3:
                printf("Thank you for using Inventory Management System!\n");
                return 0;
            default:
                printf("Invalid choice!\n");
                pause_screen();
        }
    }
    
    return 0;
}