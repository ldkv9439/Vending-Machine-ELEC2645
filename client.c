#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "client.h"
#include "owner.h"
#include "structs.h"

int itemsSelected = 0; // starts with 0 items selected
float totalCost = 0.0; // total cost is 0 in the start
int count; // initialize count for struct items
struct cartItem *cartItems; // intialize stock item struct pointer
struct item *items; // intialize cart items struct pointer
int displayedIndexes[50]; // initialize array for displayed indices

void client_menu(void)
{   // display the client menu

    printf("\n----- Client Menu -----\n");
    printf("1. Select Item\n");
    printf("2. Back to Main Menu\n");
    printf("-----------------------\n");

    int input; 
    scanf("%d", &input);

    getchar(); // to consume leftover newline character

    cartItems = malloc(50 * sizeof(struct cartItem)); // dynamically allocate cart items

    items = filetostruct(&count); // call file to struct function and store it in items pointer

    if (input == 1)
    {
        select_item(items, displayedIndexes);
    }
    else
    {
        return;
    }
}

/* Function takes in items struct pointer as input
    It displays only the AVAILABLE items (quantity>0)
    Goes to select quantity of the same item
 */
void select_item(struct item *items, int *displayedIndexes)
{
    printf("Available Items:\n\n");
    printf("Item Name - Price\n");
    int displayCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (items[i].quantity < 1)
        {
            continue; // Not displaying out of stock items
        }
        else
        {
            printf("%d. %s - %.2f\n", displayCount + 1, items[i].name, items[i].price);
            displayedIndexes[displayCount] = i; 
            displayCount++;
        }
    }
    printf("\nEnter the item number you wish to select: ");
    int input;
    scanf("%d", &input);
    if (input < 1 || input > displayCount)
    {
        printf("Invalid item number. Please try again.\n");
        return;
    }
    else{
      int realIndex = displayedIndexes[input - 1];
        printf("You selected: %s\n", items[realIndex].name);
        select_quantity(cartItems, realIndex, displayedIndexes);}
    }
/* Function takes in the input number of item and cartItems pointer as input
    Asks user to enter quantity they want
    Updates cart Items with attributes(item index, quantity, price)
    Updates number of items selected
    Asks if user would like to add more items
*/
void select_quantity(struct cartItem *cartItems, int input, int *displayedIndexes)
{
    printf("Enter quantity: ");
    int qty;
    scanf("%d", &qty);
    cartItems[itemsSelected].index = input;
    cartItems[itemsSelected].price = items[input].price;
    cartItems[itemsSelected].quantity = qty;
    itemsSelected++;

    printf("\nWould you like to add more items? (y/n): ");
    char choice;
    getchar(); // consume newline
    scanf("%c", &choice);
    choice = tolower(choice);
    if (choice == 'y')
    {
        add_more_items(items, cartItems, displayedIndexes);
    }
    else
    {
        payment(&totalCost);
    }
}

/* Function adds more items to the cart
    If an item already exists in the cart, generates an error and exits function
    else allows to add quantity of the new item added
*/
void add_more_items(struct item *items, struct cartItem *cartItems, int *displayedIndexes)
{
    printf("\nEnter the item number you wish to select: ");
    int input;
    scanf("%d", &input);
    // make checks if items is in cart or no
    for (int i = 0; i < itemsSelected; i++)
    {
        if (input - 1 == cartItems[i].index)
        {
            printf("\nItem already exists in cart.\n");
            return;
        }
    }

    if (input < 1 || input > count)
    {
        printf("Invalid item number. Please try again.\n");
    }
    else
    {   int realIndex = displayedIndexes[input - 1];
        printf("You selected: %s\n", items[realIndex].name);
        select_quantity(cartItems, realIndex, displayedIndexes);
    }
}

/* Function calculates the total cost of items in the cart
    Uses pointer to totalCost to update it
    Is only called in Payment Function
*/
void calculate_total(struct cartItem *cartItems, float *totalCost)
{
    float sum = 0.0;
    for (int i = 0; i < itemsSelected; i++)
    {
        sum += cartItems[i].price * cartItems[i].quantity;
    }
    *totalCost = sum;
}

/*  Function to generate an animation for dispensing item
    Is called after payment is successful
*/
void dispensing_animation(void)
{   printf("\e[?25l"); // disables terminal cursor
    for (int i = 0; i < 6; i++)
    {
        clock_t start_time = clock();
        while(clock() < (start_time + 200000)){;} // time delay
        printf("\033c"); // clear terminal every frame
        printf("\n Dispensing Item ＼(￣▽￣)／\n");
        printf("\n_________________");
        for (int j = 0; j < 10; j++)
        {
            if (j == i * 2 && i < 6)
            {
                printf("\n|\t▓  \t|");
            }
            else
            {
                printf("\n|\t\t|");
            }
        }
        if (i == 5)
        {
            printf("\n________▓________\n\n");
        }
        else
        {
            printf("\n_________________\n\n\n");
        }
        
    }
    printf("\n Item Dispensed. Please Collect It!! ByeBye (◠ ‿ ◠)\n\n");
    printf("\e[?25h"); // enable terminal cursor again
}


/*  Payment Function
    After calculating total cost, it asks users if they want to continue with payment
    if Y then dispensing item animation is generated and program exits
    if N then transaction is cancelled and program exits
*/
void payment(float *totalCost)
{
    calculate_total(cartItems, totalCost);
    printf("\nProceeding to payment...\n");
    printf("Total amount due: %.2f\n", *totalCost);
    printf("\nWould you like to confirm payment? (y/n): ");

    char choice;
    getchar(); // consume newline
    scanf("%c", &choice);

    choice = tolower(choice);
    if (choice == 'y')
    {
        float prevSum = 0.0;
        printf("\nPayment successful. Thank you for your purchase!\n\n");

        dispensing_animation();

        // append existing money.txt file
        FILE *in = fopen("money.txt", "r+");
        fscanf(in, "%f", &prevSum); // take previous total from file
        *totalCost += prevSum; // add the new total to the previous amount
        rewind(in);
        fprintf(in, "%f", *totalCost); // rewrite the new value in file
        fclose(in);

        // check lines of sales record to keep only 100 recent sales
        FILE *sales = fopen("sales.txt", "r");
        int lines = 0;
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), sales))
        {
            lines++;
        }
        fclose(sales);

        if (lines <= 100) // if less than 100
        {
            // add sales
            FILE *output = fopen("sales.txt", "a+");
            for (int i = 0; i < itemsSelected; i++)
            {
                int index = cartItems[i].index;
                fprintf(output, "%s %d\n", items[index].name, cartItems[i].quantity);
                items[index].quantity -= cartItems[i].quantity;
            }
            fclose(output);
        }
        else
        { // clear the file to add new 100
            FILE *sales = fopen("sales.txt", "w");
            fclose(sales);
        }

        update_record(items, count); // update the record after client buys items

        // free pointers and exit
        free(items); 
        free(cartItems);
        exit(1);
    }
    else
    {   // if cancelled free pointers and exit
        free(items);
        free(cartItems);
        cancel_transaction();
    }
}

void cancel_transaction(void)
{
    printf("Transaction cancelled. Returning to main menu.\n");
}
