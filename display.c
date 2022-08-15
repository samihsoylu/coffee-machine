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
#include "settings.h"
#include "coins.h"
#include "admin_handler.h"

// Standard I/O library
#include <stdio.h>

// For system();
#include <stdlib.h>

#define DISPLAY_SIZE_STR "60"
#define DISPLAY_SIZE 60

// Gets rid of warning
#ifdef _WIN32
    void sleep(int ms);
#endif

void delay(int ms);

/*
* Debug message, displays for the programmers
* convenience that the module is working.
*
* @return void
*/
void Display_Initialise(void)
{
    Display_DebugSystemInfo("Display module: initialised");
}

/*
* Displays message for the programmers convenience.
* This only works in debug mode.
*
* @return void
*/
void Display_DebugSystemInfo(const char *text)
{
   if(DEBUG_MODE) {
    printf("\n-- DEBUG %-"DISPLAY_SIZE_STR"s\n", text);
   }
}

/*
* We manipulate the original function and add
* a delay here.
*
* @return void
*/
void Display_DebugSystemInfo_WithDelay(const char *text) {
    Display_DebugSystemInfo(text);
    if(DEBUG_MODE){
        Display_Delay(3000);
    }
}
/*
* Only in debug mode, system error is shown. This is again
* for the programmers convenience. This function is implemented
* in to various locations within this project.
*
* @return void
*/
void Display_SystemError(const char *text)
{
   if(DEBUG_MODE) {
    printf("\n## SYSTEM ERROR %-"DISPLAY_SIZE_STR"s ##\n", text);
    Display_Delay(3000);
   }
}

/*
 * Displays balance screen to user, the function clears
 * screen first and then divides int balance by 100 to turn
 * it in to double.
 *
 * If admin mode is enabled extra information is displayed/enabled
 *
 * @return state_e WAIT - user is able to input a letter after this
 *                      function is executed
 */
state_e Display_BalanceScreen(const int balance) {

    double newBalance = (double)balance / 100;

    // Clear Screen
    Display_ClearScreen();

    if(newBalance > 0) {
        printf("\n\t-------------");
        printf("\n\tProcessing...");
        printf("\n\t-------------");
        Display_Delay(1000);
    }

    // Clear Screen
    Display_ClearScreen();

    if(ADMIN_MODE) {
        printf("\n\t------------------------------------------------------");
        printf("\n\tADMIN_MODE: ON\t\t Press 'e' to go to admin menu");
        printf("\n\t------------------------------------------------------\n");
    }

    if(admin_data.change_available < 200) {
        printf("\n\t------------------------------------------------------");
        printf("\n\tWARNING: Machine cannot return any change");
        printf("\n\t------------------------------------------------------\n");
    }

    printf("\n\t------------------------------");
    printf("\n\tCurrent balance:\t $%.2f", newBalance);
    printf("\n\t------------------------------");

    printf("\n\n\t--------------------------------------------------");
    printf("\n\tDrink\t\t\t Price \t\tIn stock?");
    if(ADMIN_MODE) {
        printf("\t\tStock");
    }
    printf("\n\t--------------------------------------------------");
    for(unsigned int i = 0; i < (sizeof(Drink) / sizeof(Drink[0])); i++) {
        printf("\n\t%s:\t\t $%.2f \t\t%s", Drink[i].title, (double)Drink[i].price / 100, Drink[i].isInStock);
        if(ADMIN_MODE) {
            printf("\t\t\t%d", Drink[i].amount_available);
        }
    }
    printf("\n\t--------------------------------------------------");

    printf("\n\n\tPress: 'Y' to insert 50 cents | 'Q' to return all cash | 'P' go to coffee selection screen\n\n\t");

    return STATE_WAIT_FOR_COINS;
}

/*
 * Drink select shows available balance and list of choices
 * if balance is lower than price then choice is not displayerd
 *
 * @return void
 */
void Display_DrinkSelectionScreen(const int userBalance) {

    Display_ClearScreen();

    printf("\n\t------------------------------");
    printf("\n\tBalance:\t $%.2f", (double)userBalance / 100);
    printf("\n\t------------------------------");

    printf("\n\n\t------------------------------");
    printf("\n\tDrink\t\t Key to select");
    printf("\n\t------------------------------");

    if(userBalance >= Drink[2].price && Drink[2].amount_available > 0) { // H-Water
        printf("\n\t%s:\t\t G", Drink[2].title);
    }
    if(userBalance >= Drink[1].price && Drink[1].amount_available > 0) { // B-Coffee
        printf("\n\t%s:\t\t Z", Drink[1].title);
    }
    if(userBalance >= Drink[0].price && Drink[0].amount_available > 0) { // Expresso
        printf("\n\t%s:\t\t X", Drink[0].title);
    }
    printf("\n\t[Go back] :\t\t P");
    printf("\n\t------------------------------\n\t");

}

/*
 * Cancel screen, shows money has been returned
 * and returns to official balance screen.
 *
 * @return state_e INITIAL - balance screen.
 */
state_e Display_CancellationScreen(const int coins) {

    Display_ClearScreen();

    double returnedCoins = (double)coins / 100;

    // if change is more than 200
    if(returnedCoins > 0) {
        printf("\n\t------------------------------");
        printf("\n\tReturned change:\t $%.2f", returnedCoins);
        printf("\n\t------------------------------");
    } else {

        printf("\n\t------------------------------");
        printf("\n\tSorry could not give change: $%.2f", returnedCoins);
        printf("\n\t------------------------------");
    }

    Display_Delay(5000);

    // go back to balance screen.
    return STATE_INITIALISED;
}

/*
 * Confirmation screen, displays name of product
 * asks for user confirmation
 *
 * @return void
 */
void Display_DrinkConfirmationScreen(const int i) {

    Display_ClearScreen();

    printf("\n\t--------------------------------------");
    printf("\n\tYou have chosen: %s $%.2f.", Drink[i].title, (double)Drink[i].price / 100);
    printf("\n\t--------------------------------------");
    printf("\n\n\tPress 'Y' to proceed or 'N' to go back. \n\t");

}

/*
 * Executed when state is thank you. This is after the choice of
 * purchase has been made from the customers.
 *
 * @return state_e STATE_INITIALISED - back to the start (for new customer)
 */
state_e Display_ThanksScreen() {

    Display_ClearScreen();

    printf("\n\t------------------------------");
    printf("\n\t Thank you for your purchase!");
    printf("\n\t------------------------------");

    Display_Delay(3000);

    return STATE_INITIALISED;
}

/*
 * Welcomes admin, shows different information regarding
 * the machine. Shows three options at the end for the
 * administrator to be able to perform maintenance tasks.
 * The options are processed in admin_handler.c
 *
 * @return void
 */
void Display_AdminScreen() {
    Display_ClearScreen();
    printf("\n\t---------------------------------------");
    printf("\n\tWelcome to the administration screen");
    printf("\n\t---------------------------------------");

    printf("\n\n\tMachine status");
    printf("\n\t---------------------------------------");
    printf("\n\tAvailable change \t\t$%.2f", (double)admin_data.change_available / 100);
    printf("\n\t---------------------------------------");

    printf("\n\n\tMachine statistics");
    printf("\n\t---------------------------------------");
    printf("\n\tTotal earned money \t\t$%.2f", (double)admin_data.total_earned_money / 100);
    printf("\n\tTotal drinks sold \t\t%d", admin_data.total_sold_drinks);
    printf("\n\t---------------------------------------");

    printf("\n\n\tDrink statistics");
    printf("\n\t---------------------------------------");
    for(unsigned int i = 0; i < (sizeof(Drink) / sizeof(Drink[0])); i++) {
        printf("\n\t%s sold \t\t%d", Drink[i].title, Drink[i].total_sold);
    }
    printf("\n\t---------------------------------------");

    printf("\n\n\tDrinks \t\t\t\tStock");
    printf("\n\t---------------------------------------");
    for(unsigned int i = 0; i < (sizeof(Drink) / sizeof(Drink[0])); i++) {
        printf("\n\t%s \t\t\t%d", Drink[i].title, Drink[i].amount_available);
    }
    printf("\n\t---------------------------------------");
    printf("\n\n\tPress 'Q' to return to main screen | 'N' to update stocks | 'C' to insert change\n\t");
}

/*
 * Displays stocks when admin selects update stocks
 * option within the admin screen. Options are processed
 * in admin_handler.c
 *
 * @return void
 */
void Display_AdminScreen_UpdateStocks() {

    Display_ClearScreen();

    printf("\n\n\tDrinks \t\t\tStock\tKey");
    printf("\n\t---------------------------------------");
    for(unsigned int i = 0; i < (sizeof(Drink) / sizeof(Drink[0])); i++) {
        printf("\n\t%s \t\t%d\t%d", Drink[i].title, Drink[i].amount_available, i);
    }
    printf("\n\t---------------------------------------");
    printf("\n\n\tPress any of the keys listed above appointed to edit the stock, press X to go back\n\t");
}

/*
 * Displays edit screen when selection of which stock
 * to edit has been made. Input for this is processed in
 * admin_handler.c
 *
 * @param int choice - the index drink of users choice
 * @return void
 */
void Display_AdminScreen_UpdateStocks_Edit(int choice) {

    Display_ClearScreen();

    printf("\n\n\tDrinks \t\t\tStock");
    printf("\n\t--------------------------------");
    printf("\n\t%s \t\t%d", Drink[choice].title, Drink[choice].amount_available);
    printf("\n\t--------------------------------");
    printf("\n\tEnter the added stock value (enter a number):\n\t");
}

/*
 * Once stocks are successfully updated, this message
 * is shown on screen to let the admin know
 *
 * @return void
 */
void Display_AdminScreen_UpdateStocks_Edit_Updated() {

    Display_ClearScreen();

    printf("\n\t--------------------------------");
    printf("\n\tUpdated stock successfully.");
    printf("\n\t--------------------------------");
    Display_Delay(3000);
}

/*
 * This function is called when there are 1.50 coins
 * in the machine. Stops extra change from being given
 * back in long term, so less maintenance for the owner
 *
 * @return state_e initial - goes back to balance screen
 */
state_e Display_DeniedCoins() {

    Display_ClearScreen();

    printf("\n\t---------------------------------------------");
    printf("\n\tThe coin you've just entered has been denied.");
    printf("\n\t---------------------------------------------");
    Display_Delay(3000);
    return STATE_INITIALISED;
}

/*
 * Shows coins to user, and displays options on how to add coins
 * this is displayed in the admin screen.
 *
 * @return void
 */
void Display_AdminScreen_InsertCoins() {
    Display_ClearScreen();

    printf("\n\t------------------------------------------");
    printf("\n\tCurrent amount of coins in the machine: $%.2f", (double)admin_data.change_available / 100);
    printf("\n\t------------------------------------------");
    printf("\n\n\tPress 'W' to enter $0.50 | 'E' to enter $10.00 | 'R' to enter $50.00 | 'X' to go back\n\t");
}

/*
 * Simply displays that drink is being disposed
 * executed in STATE_DRINK_CONFIRMATION within
 * the main.c file.
 *
 * @return void
 */
void Display_DrinkDisposing() {

    for(int i = 0; i < 6; i++) {
        Display_ClearScreen();

        printf("\n\t------------------------------");
        printf("\n\t Dispensing drink");
        for(int x = 0; x < i; x++) {
            printf(".");
        }
        printf("\n\t------------------------------");

        Display_Delay(1000);
    }
}

/*
 * Clears console screen, this was added because development was
 * peferred to be done on a Mac halfway through this project.
 *
 * Since I wanted to keep this cross-platform it was necessary
 * to implement this function.
 *
 * @return void
 */
void Display_ClearScreen(void)
{
    #ifdef _WIN32
        system("cls");
    #elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
        system("clear");

    #else
        #error "OS not supported."

    #endif
}


/*
 * Delays console screen, this was added because development was
 * peferred to be done on a Mac halfway through this project.
 *
 * Since I wanted to keep this cross-platform it was necessary
 * to implement this function.
 * 
 * @param int ms - mili seconds of delay.
 * @return void
 */
void Display_Delay(unsigned int ms) {

    #ifdef _WIN32
        sleep(ms / 1000);
    #elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
        usleep(ms * 1000);

    #else
        #error "OS not supported."

    #endif

}
