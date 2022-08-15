//
//   Copyright © 2017 Samih Soylu.
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
//   Software.
//
//   Modified work Copyright <your_name_goes_here>
//
#include "states.h"
#include "display.h"
#include "drink.h"
#include "coins.h"
#include "settings.h"
#include "admin_handler.h"
#include <stdio.h>

// strcpy()
#include <string.h>

// access()
#include <unistd.h>

// For _kbhit()
#include <conio.h>


/* Global variable */
drinks_t Drink[3];

// Prototype
void Drink_DefineDrinks();

/*
 * Instantiation of drink.c
 * If drinks.txt file does not exist or
 * is corrupt then it is re-created and new values are assigned
 *
 * If it exists it will read and define accordingly.
 *
 * @return void
 */
void Drink_Initialise(void)
{
   // Defines Drink[]; either imports or loads defaults
   Drink_ImportLogistics();

   Display_DebugSystemInfo("Dispenser: initialised");
}

/*
* Drinks are defined in this function, these are their
* default settings. This function is only executed if
* the logistics txt file does not exist.
*
* @return void
*/
void Drink_DefineDrinks() {
    strcpy(Drink[0].title, "Espresso  ");
    Drink[0].price = 150;
    Drink[0].amount_available = 2;
    Drink[0].total_sold = 0;

    strcpy(Drink[1].title, "Coffee    ");
    Drink[1].price = 100;
    Drink[1].amount_available = 2;
    Drink[1].total_sold = 0;

    strcpy(Drink[2].title, "Hot water ");
    Drink[2].price = 50;
    Drink[2].amount_available = 2;
    Drink[2].total_sold = 0;

    Drink_StockUpdate();
}

/*
 * Drink stock is updated here, the string isInStock
 * for each drink[] is updated either to no or stays yes
 *
 * @return void
 */
void Drink_StockUpdate() {
    // Loops through all drinks and checks if in stock
    // Later used to show user if in stock.
    for(unsigned int i = 0; i < (sizeof(Drink) / sizeof(Drink[0])); i++) {
        if(Drink[i].amount_available == 0) {
            strcpy(Drink[i].isInStock, "No");
        } else {
            strcpy(Drink[i].isInStock, "Yes");
        }
    }
}


/*
 * Event handler, returns a certain state based
 * on user input on the drink selection screen
 *
 * @param int userChoice - chosend drink from userinput
 * @return state_e STATE - returned state from event
 */
state_e Drink_Dispose(const int i)
{
    Drink[i].amount_available -= 1;
    Coins_UserBalance -= Drink[i].price;

    admin_data.total_earned_money += Drink[i].price;
    admin_data.total_sold_drinks += 1;
    Drink[i].total_sold += 1;

    /*
     * @note amount change returned in payment is processed in coins.c
     */

    Drink_StockUpdate();

    // Saves to file
    Drink_ExportLogistics();
    Admin_ExportData();

    Display_DebugSystemInfo_WithDelay("Dispenser: dispensing drink");

    return STATE_THANK_CUSTOMER;
}

/*
* Function confirms that user wants the
* selected drink.
*
* @return event_e condition - either user accepts or declines
*                           the drink they choose.
*/
event_e Drink_Confirmation() {
    char userInput;
    while (!_kbhit()){;}
    userInput = getch();

    switch(userInput) {
        case 'Y':
        case 'y':
            return EVENT_USER_ACCEPTS_SELECTED_DRINK;
        case 'N':
        case 'n':
            return EVENT_USER_DECLINES_SELECTED_DRINK;
        default:
            return EVENT_READY;
    }
}

/*
* Based on given choice, the function returns
* chosen drink index, later used in drink_dispose
* externally.
*
* @param event_e - choice of drink
* @return int index - id of drink
*/
int Drink_getChoiceID(const event_e choice) {
    switch(choice) {
        case EVENT_SELECTED_DRINK_COFFEE:
            return 1;
        case EVENT_SELECTED_DRINK_HOTWATER:
            return 2;
        case EVENT_SELECTED_DRINK_EXPRESSO:
            return 0;
        default: // in this case, default will never be returned.
            return 3;
    }
}

/*
* This function is used to import the coffee
* machine logistics to the program. The text
* file that is imported stores information
* such as how many left in stock and title.
*
* @return void
*/
void Drink_ImportLogistics() {
    FILE *f;

    if(access("CoffeeMachine_Logistics.txt", F_OK) != -1 && !RESET_FILES) {
        // File exists, reads from file
        f = fopen("CoffeeMachine_Logistics.txt", "r");
        fread(&Drink[0], sizeof(drinks_t), 1, f);
        fread(&Drink[1], sizeof(drinks_t), 1, f);
        fread(&Drink[2], sizeof(drinks_t), 1, f);
        fclose(f);
        Display_DebugSystemInfo_WithDelay("Read CoffeeMachine_Logistics.txt");
    } else {
        // File does not exist, writes default data to a new file
        Drink_DefineDrinks();
        Drink_ExportLogistics();
        Display_DebugSystemInfo_WithDelay("Created CoffeeMachine_Logistics.txt");
    }
}

/*
 * This function exports the data such has how
 * many drinks are left in stock. This is then
 * stored in a safe location just in case there
 * is a system failiure.
 *
 * @return void
 */
void Drink_ExportLogistics() {
    FILE *f;

    f = fopen("CoffeeMachine_Logistics.txt", "w+");
    fwrite(&Drink[0], sizeof(drinks_t), 1, f);
    fwrite(&Drink[1], sizeof(drinks_t), 1, f);
    fwrite(&Drink[2], sizeof(drinks_t), 1, f);
    fclose(f);
}

/*
 * Loops through and finds if all stocks are
 * empty. If so then returns a value based on condition
 * Used in state waiting for input after balance screen.
 *
 * @return int true/false - StockEmpty/StockFull
 */
int Drink_CheckIfStockIsCompletelyEmpty() {
    unsigned int i;
    for(i = 0; i < (sizeof(Drink) / sizeof(Drink[0])); i++) {
        if(Drink[i].amount_available > 0) {
            return 0;
        }
    }
    return 1;
}
