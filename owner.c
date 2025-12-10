#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "owner.h"
#include "structs.h"

const int MAX = 100; // MAX 

/* Function to convert the stock file to struct
    Converts to make easy changes to inventory
    Returns pointer to the struct created
*/
struct item *filetostruct(int *count)
{
    FILE *fp = fopen("stock.txt", "r");
    if (!fp)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    // dynamic allocation of pointer

    struct item *items = malloc(50 * sizeof(struct item));

    if (!items)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    *count = 0; 

    char line[50];
    fgets(line, sizeof(line), fp); // ignore header
    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        char name[20];
        int quantity;
        float price;

        if (sscanf(line, "%19s %d %f", name, &quantity, &price) == 3)
        {
            // Save to struct array
            strcpy(items[*count].name, name);
            items[*count].quantity = quantity;
            items[*count].price = price;
            (*count)++;
        }
        else
        {
            printf("Invalid line format: %s\n", line);
        }
    }

    fclose(fp);
    return items;
}
/* Function is used to authorize the owner
    Asks user to input password (max 3 tries given)
    Reads the password file to compare
    If successful, goes to the owner menu
    Else goes to main menu
*/
int authorize(void)
{
    // check if entered password matches data
    char inputString[MAX] = {};
    int retry = 3;

    while (retry > 0)
    {   FILE *pass = fopen("password.txt", "r");
        if (!pass){
        printf("Error opening file.\n");
        return 0;}
    
        char password[MAX];
        fgets(password, MAX, pass);
        fclose(pass);

        // strip newline from password
        int len = strlen(password);
        if (password[len - 1] == '\n')
        password[len - 1] = '\0';
    
        int match = 1;
        int count = 0;
        printf("Enter your password: \nTries Remaining = %d \n", retry);
        fgets(inputString, sizeof(inputString), stdin);

        len = strlen(inputString);
        if (inputString[len - 1] == '\n')
            inputString[len - 1] = '\0';

        while (password[count] != '\0' || inputString[count] != '\0')
        {
            if (password[count] != inputString[count])
            {
                match = 0;
                break;
            }
            count++;
        }
        if (match)
        {
            printf("\n\tAuthorization Successful ^-^! \n\n");
            owner_menu();
            return 1;
        }
        else
        {   
            printf("Try Again! :( \n\n");
            retry = retry - 1;
        }
    }
    printf("Max tries exceeded :(\n");
    return 0;
}
void owner_menu()
{
    printf("Choose your service:");
    printf("\n----------- Owner menu ----------\n");
    printf("|\t\t\t\t|\n"
           "|\t1. Add/Edit Stock \t|\n"
           "|\t2. Check Inventory \t|\n"
           "|\t3. Collect Money \t|\n"
           "|\t4. Change Password \t|\n"
           "|\t5. Check Purchase Trend |\n"
           "|\t6. Main Menu \t\t|\n");
    printf("---------------------------------\n");
    int input;
    scanf("%d", &input);
    switch (input)
    {
    case 1:
    {
        add_stock();
        printf("\nEnter 1 to return to Owner Menu: ");
        int val;
        scanf("%d", &val);
        if (val == 1)
        {
            owner_menu();
        }
        break;
    }
    case 2:
    {
        check_inventory();
        printf("\nEnter 1 to return to Owner Menu: ");
        int val;
        scanf("%d", &val);
        if (val == 1)
        {
            owner_menu();
        }
        break;
    }
    case 3:
    {
        collect_money();
        printf("\nEnter 1 to return to Owner Menu: ");
        int val;
        scanf("%d", &val);
        if (val == 1)
        {
            owner_menu();
        }
        break;
    }
    case 4:
    {
        change_password();
        printf("\nEnter 1 to return to Owner Menu: ");
        int val;
        scanf("%d", &val);
        if (val == 1)
        {
            owner_menu();
        }
        break;
    }
    case 5:
    {
        check_trends();
        printf("\nEnter 1 to return to Owner Menu: ");
        int val;
        scanf("%d", &val);
        if (val == 1)
        {
            owner_menu();
        }
        break;
    }
    case 6:
        break;
    default:
        break;
    }
}

void add_stock()
{
    int count;
    struct item *items = filetostruct(&count);
    // Print stored data
    printf("\nCurrent Stock:\n");
    printf("Item - Quantity - Price\n\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d. %s - %d - %.2f\n", i + 1, items[i].name, items[i].quantity, items[i].price);
    }
    printf("\nEnter 'e' to edit an item or 'a' to add a new item or 'r' to remove item: ");
    char choice;
    scanf(" %c", &choice);
    choice = tolower(choice);
    if (choice == 'e')
    {
        int itemNumber;
        printf("Enter item number to edit: ");
        scanf("%d", &itemNumber);
        if (itemNumber < 1 || itemNumber > count)
        {
            printf("Invalid item number.\n");
        }
        else
        {
            int index = itemNumber - 1;
            printf("Editing item: %s - %d - %.2f\n", items[index].name, items[index].quantity, items[index].price);
            struct item *selectedItem = &items[index];
            printf("Enter new quantity (or -1 to keep %d): ", selectedItem->quantity);
            int newQuantity;
            scanf("%d", &newQuantity);
            if (newQuantity != -1)
            {
                selectedItem->quantity = newQuantity;
            }
            printf("Enter new price (or -1 to keep %.2f): ", selectedItem->price);
            float newPrice;
            scanf("%f", &newPrice);
            if (newPrice != -1)
            {
                selectedItem->price = newPrice;
            }
            printf("Item updated to: %s - %d - %.2f\n", selectedItem->name, selectedItem->quantity, selectedItem->price);
        }
    }
    else if (choice == 'a')
    {
        struct item newItem;
        printf("Enter name of new item: ");
        scanf("%s", newItem.name);
        printf("Enter quantity of new item: ");
        scanf("%d", &newItem.quantity);
        printf("Enter price of new item: ");
        scanf("%f", &newItem.price);
        items[count] = newItem;
        count++;
        printf("New item added: %s - %d - %.2f\n", newItem.name, newItem.quantity, newItem.price);
    }
    else if (choice == 'r')
    {
        int itemNumber;
        printf("Enter item number to remove: ");
        scanf("%d", &itemNumber);
        if (itemNumber < 1 || itemNumber > count)
        {
            printf("Invalid item number.\n");
        }
        else
        {
            for (int i = itemNumber - 1; i < count - 1; i++)
            {
                items[i] = items[i + 1]; // Overwrite the item to be removed
            }
            count--;
            printf("Item removed successfully.\n");
        }
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }
    update_record(items, count);
    printf("Stock updated successfully.\n");
    free(items);
}

void check_inventory(void)
{
    FILE *fptr;
    char stock[50];
    fptr = fopen("stock.txt", "r");

    if (fptr == NULL)
    {
        printf("File failed to open.");
    }
    else
    {
        while (fgets(stock, 50, fptr) != NULL)
        {
            printf("%s", stock);
        }
        fclose(fptr);
    }
}

void collect_money(void)
{
    float totalSales = 0.0;
    char line[10];
    FILE *input = fopen("money.txt", "r");
    if (input == NULL)
    {
        printf("Error opening file.\n");
    }
    else
    {
        if (fgets(line, sizeof(line), input) != NULL)
        {
            totalSales = atof(line);
            fclose(input);
        }
        if (totalSales == 0.0)
        {
            printf("No sales to collect.\n");
            return;
        }
        printf("Collecting money...\n");
        printf("Total sales collected: %.2f\n", totalSales);
        totalSales = 0.0;

        FILE *output = fopen("money.txt", "w");
        fprintf(output, "%.2f\n", totalSales);
        fclose(output);
    }
}

void change_password(void)
{
    char newPassword[MAX];
    printf("Enter new password:\n");
    getchar(); // to consume leftover newline character
    fgets(newPassword, MAX, stdin);
    // strip newline from newPassword
    int len = strlen(newPassword);
    if (newPassword[len - 1] == '\n')
        newPassword[len - 1] = '\0';
    FILE *pass = fopen("password.txt", "w");
    if (!pass)
    {
        printf("Error opening file.\n");
        return;
    }
    fputs(newPassword, pass);
    fclose(pass);
    printf("Password changed successfully!\n");
}
void check_trends(void)
{
    struct trend
    {
        char name[20];
        int quantity;
    };
    FILE *fp = fopen("sales.txt", "r");
    if (!fp)
    {
        printf("Error opening file.\n");
        exit(1);
    }

    struct trend trend_data[100];
    int count = 0;
    char line[50];

    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        char name[20];
        int quantity;

        if (sscanf(line, "%s %d", name, &quantity) == 2)
        {
            strcpy(trend_data[count].name, name);
            trend_data[count].quantity = quantity;
            count++;
        }
    }

    fclose(fp);
    struct trend unique_trends[50];
    int unique_count = 0;

    for (int i = 0; i < count; i++)
    {
        int found = 0;
        for (int j = 0; j < unique_count; j++)
        {
            if (strcmp(trend_data[i].name, unique_trends[j].name) == 0)
            {
                unique_trends[j].quantity += trend_data[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(unique_trends[unique_count].name, trend_data[i].name);
            unique_trends[unique_count].quantity = trend_data[i].quantity;
            unique_count++;
        }
    }
    printf("\nPURCHASE TRENDS\n\n");

    for (int i = 0; i < unique_count; i++)
    {
        printf("%s | ", unique_trends[i].name);

        for (int j = 0; j < unique_trends[i].quantity; j++)
        {
            printf("*");
        }

        printf(" (%d)", unique_trends[i].quantity);
        printf("\n");
    }
}

void update_record(struct item *items, int count)
{
    // Write back to file
    FILE *output = fopen("stock.txt", "w");
    fprintf(output, "item quantity price\n");
    for (int i = 0; i < count; i++)
    {
        fprintf(output, "%s %d %.2f\n", items[i].name, items[i].quantity, items[i].price);
    }
    fclose(output);
}
