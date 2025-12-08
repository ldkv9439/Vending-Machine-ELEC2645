#ifndef CLIENT_H
#define CLIENT_H
#include "structs.h"

// client functions declaration

void client_menu(void); // displays menu 
void select_item(struct item *items, int *displayIndexes); // select items to dispense
void select_quantity(struct cartItem *cartItems, int input, int *displayIndexes); // asks for quantity of the item selected
void add_more_items(struct item *items, struct cartItem *cartItems, int *displayIndexes); // adds more items if needed
void calculate_total(struct cartItem *cartItems, float *totalCost); // calculates the total cost of the order
void payment(float *totalCost); // goes to payment
void dispensing_animation(void); // dispensing item animation
void cancel_transaction(void); // cancel transaction if asked

#endif