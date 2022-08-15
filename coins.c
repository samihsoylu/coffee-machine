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
#include "coins.h"
#include "drink.h"
#include "settings.h"
#include "admin_handler.h"

// Standard I/O library
#include <stdio.h>

// For strcpy();
#include <string.h>

// For getch();
#include <conio.h>

// Global user balance (global defined in header file)
int Coins_UserBalance;
int Coins_ChosenDrinkIndex;

/*
 * Only executed at the start
 * of program. Lets developer
 * know it is initialised.
 *
 * @return void
 */
void Coins_Initialise()
{
    Display_DebugSystemInfo("Coin module: initialised");
}

/*
 * Executed in main when user is on balance screen
 * and has chosen which action to take. In this case
 * they have three options, enter coin, return coin
 * and select drink.
 *
 * @return event_e EVENT - choice of action
 */
event_e Coins_BalanceProcessor() {

    char userInput;

    while (!_kbhit()){;}

    userInput = getch();

    switch(userInput) {
        case 'Y':
        case 'y':
            return EVENT_50C_ENTERED_TO_MACHINE;
        case 'Q':
        case 'q':
            return EVENT_USER_WANTS_ALL_MONEY_RETURNED;
        case 'P':
        case 'p':
            return EVENT_USER_WANTS_TO_SELECT_DRINK;
        case 'E':
        case 'e':
            if(ADMIN_MODE) {
                return EVENT_ADMIN_MODE;
            }
        default: // should be never true
            return EVENT_READY;
    }
}

/*
 * Executed when user is asked to make a choice of drink
 * the choice options are displayed based on balance.
 *
 * @param int balance - Sum value of coins inserted in to machine
 * @return event_e EVENT - choice of drink
 */
event_e Coins_CoffeeChoice(const int balance) {
    char userInput;
    while (!_kbhit()){;}
    userInput = getch();

    if(balance >= Drink[2].price && Drink[2].amount_available > 0) {
        if(userInput == 'G' || userInput == 'g') {
            Coins_ChosenDrinkIndex = 2;
            return EVENT_SELECTED_DRINK_HOTWATER;
        }
    }

    if(balance >= Drink[1].price && Drink[1].amount_available > 0) {
        if(userInput == 'Z' || userInput == 'z') {
            Coins_ChosenDrinkIndex = 1;
            return EVENT_SELECTED_DRINK_COFFEE;
        }
    }

    if(balance >= Drink[0].price && Drink[0].amount_available > 0) {
        if(userInput == 'X' || userInput == 'x') {
            Coins_ChosenDrinkIndex = 0;
            return EVENT_SELECTED_DRINK_EXPRESSO;
        }
    }

    if(userInput == 'P' || userInput == 'p') {
        return EVENT_SELECTED_DRINK_CANCEL;
    }

    // In this case state doesn't change
    // either a undefined key was pressed or
    // the customer did not have enough money
    return EVENT_READY;
}

/*
 * Executed when user enters coins
 *
 * @param int enteredCoins - value of coin the user entered
 * @return state_e STATE_INITIALISED - program returns to balance screen
 */
state_e Coins_inputCoins(const int enteredCoins) {

    Coins_UserBalance += enteredCoins;
    return STATE_INITIALISED;
}

/*
 * Executed when user demands return
 * resets the balance. Two of the same parameters are passed because
 * one is a pointer, the other is not. Pointer is in binary and was causing
 * subtracting problems, using two parameters was a the quick solution.
 *
 * @param int amountToReturn            - simply set to 0 later if no change is available
 * @param int amount_returned           - returned cash, used to subtract to change left in machine
 * @return state_e STATE_RETURNED_MONEY - takes user to the screen that
 *                                        shows the amount returned
 */
state_e Coins_returnCoins(int *amountToReturn, int amount_returned) {

    // Resets coins
    Coins_UserBalance = 0;

    // Only subtracts if coins are more than 2.00
    if(admin_data.change_available >= 200) {
        admin_data.change_available -= amount_returned;
    } else {
        // sets amount returned to 0. Used later to display if any is returned
        *amountToReturn = (int) 0;
    }

    Admin_ExportData();

    // Says display returned money screen
    return STATE_RETURNED_MONEY;
}
