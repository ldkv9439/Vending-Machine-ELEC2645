#ifndef OWNER_H
#define OWNER_H

#include "structs.h"

// owner functions declarations 

struct item *filetostruct(int *count); // convert stock file 
int authorize(void); // authorize 
void owner_menu();  // displays the owner menu
void add_stock(void); // allows owner to add/edit/remove stock
void update_record(struct item *items, int count); // updates the new stock into file
void check_inventory(void); // inventory information
void collect_money(void); // collecting total sales
void change_password(void); // change owner password
void check_trends(void); // check trends of pruchase


#endif
