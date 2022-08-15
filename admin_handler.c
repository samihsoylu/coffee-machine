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
#include "admin_handler.h"
#include "states.h"
#include "display.h"
#include "settings.h"
#include "drink.h"

#include <stdio.h>

// access()
#include <unistd.h>

// For kbhit()
#include <conio.h>

// atoi()
#include <stdlib.h>
#include <string.h>

//prototypes
void Admin_DefineDefaults();
int isNumeric(const char *str);

void Admin_Initialise() {
    Admin_ImportData();
    Display_DebugSystemInfo("Admin module ready");
}

/*
 * Defines default values of admin_data struct
 * this is only executed if admin_data.txt data file
 * does not exist.
 *
 * @return void
 */
void Admin_DefineDefaults() {
    admin_data.change_available    = 500; // $5

    // Statistics
    admin_data.total_earned_money  = 0;
    admin_data.total_sold_drinks   = 0;
}

/*
* This function is used to import the coffee
* machine administration to the program. The text
* file that is imported stores information
* such as how many drinks sold and how much change remains.
*
* @return void
*/
void Admin_ImportData() {
    FILE *f;

    if(access("Admin_Data.txt", F_OK) != -1 && !RESET_FILES) {
        // File exists, reads from file
        f = fopen("Admin_Data.txt", "r");
        fread(&admin_data, sizeof(admin_t), 1, f);
        fclose(f);
        Display_DebugSystemInfo_WithDelay("Read Admin_Data.txt");
    } else {
        // File does not exist, writes default data to a new file
        Admin_DefineDefaults();
        Admin_ExportData();
        Display_DebugSystemInfo_WithDelay("Created Admin_Data.txt");
    }
}

/*
 * This function exports the data such has how
 * many drinks sold or how much change remains. This is then
 * stored in a safe location just in case there
 * is a system failiure.
 *
 * @return void
 */
void Admin_ExportData() {
    FILE *f;

    f = fopen("Admin_Data.txt", "w+");
    fwrite(&admin_data, sizeof(admin_t), 1, f);
    fclose(f);
}

/*
 * Main screen of admin mode. User is able
 * to press various buttons. Based on input
 * an event is returned.
 *
 * @return event_e EVENT_ - next screen
 */
event_e Admin_MainScreen_WaitForInput() {
    char userInput;

    while (!_kbhit()){;}

    userInput = getch();

    switch(userInput) {
        case 'Q':
        case 'q':
            return EVENT_ADMIN_MODE_RETURN_TO_CUSTOMER_SCREEN;

        case 'C':
        case 'c':
            return EVENT_ADMIN_MODE_INSERTCHANGE;

        case 'N':
        case 'n':
            return EVENT_ADMIN_MODE_UPDATESTOCKS;

        default:
            return EVENT_READY;
    }

}

/*
 * User is asked to choose which drink
 * they want to "add" stock. Based on their
 * input an index is returned.
 *
 * @return int index - Drink[index].
 */
int Admin_UpdateStock_WaitForInput() {
    char userInput;

    while (!_kbhit()){;}

    userInput = getch();

    switch(userInput) {
        case 'X':
        case 'x': // exits state
            return 99;
        default:
            return userInput - '0';
    }
}

/*
 * After has chosen which stock to update, they
 * are asked to enter and "add" value to the current
 * stock.
 *
 * @return event_e EVENT - either UPDATED or SELECTED based
 *                         on the fact if a number is entered
 *
 * @note: there was a check to verify if a number is entered,
 *        I removed it. so now it will always return updated even if
 *        a letter is entered
 */
event_e Admin_UpdateStock_Edit_WaitForInput(int index) {
    unsigned int userInput;

    scanf("%u", &userInput);

    Drink[index].amount_available += userInput;
    Drink_StockUpdate();
    Drink_ExportLogistics();

    Admin_ExportData();
    return EVENT_ADMIN_MODE_STOCK_UPDATED;

/*
 * else {
 *      Main_Event = EVENT_ADMIN_MODE_STOCK_SELECTED;
 *  }
 */
}

/*
 * Based on chosen drink index, function determines
 * which event to trigger.
 *
 * @return event_e EVENT - chosen event based on index
 *                       for later to be used in a switch
 *                       to determine next state.
 */
event_e Admin_UpdateStock_Input(int index) {
    if(index != 99) {
        return EVENT_ADMIN_MODE_STOCK_SELECTED;
    } else if (index == 99) {
        return EVENT_ADMIN_MODE_STOCK_EXIT;
    } else {
        return EVENT_READY;
    }
}

/*
 * Waits for user input, once a key is pressed based on
 * user choice, adds value to total change available of
 * the coffe machine
 *
 * @return event_e OK / EXIT - either change is inserted
 *                           or menu is exited
 */
event_e Admin_InsertCoin() {
    char userInput;

    while (!_kbhit()){;}

    userInput = getch();

    switch(userInput) {
        case 'W':
        case 'w':
            admin_data.change_available += 50; // $0.50
            break;
        case 'E':
        case 'e':
            admin_data.change_available += 1000; // $10.00
            break;
        case 'R':
        case 'r':
            admin_data.change_available += 5000; // $50.00
            break;
        case 'X':
        case 'x':
            return EVENT_ADMIN_MODE_INSERTCHANGE_EXIT;
        default: // stays in state
            break;
    }

    Admin_ExportData();
    return EVENT_ADMIN_MODE_INSERTCHANGE_OK;
}

/*
* Function checks if passed parameter is
* a number. This function was used in the
* Admin_UpdateStock_Edit_WaitForInput(int index)
* function.
*
* @param char str - user input string
* @return true/false
*/
int isNumeric(const char *str)
{
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}
