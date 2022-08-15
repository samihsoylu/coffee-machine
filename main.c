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

//#include <ctype.h>

/*
 * Prototypes
 */
void Main_Working_Machine(void);
state_e Main_CoffeeMachine_Initialise(void);

/* Initial definitions */
state_e Main_Current_State;
event_e Main_Event;

/* Later used to display returned money */
int returned_money = 0;

/* Later used to help the disposer function to determine which drink to dispose */
int SelectedDrinkID;

int admin_mode_stock_index;

int main()
{ 
   // Start of program
   Main_Current_State = STATE_START;
   Main_Event = EVENT_READY;

   while (1)
   {
      Main_Working_Machine();
   }

   return 0;
}

/*
 * Main function that processes everything
 * its in a loop within the main() because
 * we do not want the program to stop.
 *
 * @return void
 */
void Main_Working_Machine(void)
{
    state_e Updated_State;

    switch(Main_Current_State) {
        case STATE_START:
            // RETURNS _INITIALISED (loads all modules)
            Updated_State = Main_CoffeeMachine_Initialise();
            break;

        case STATE_INITIALISED:
            // Displays user balance
            switch(Main_Event) {
                // Only using default because same state is returned every time regardless of event
                default:
                    // Next state is _WAIT_FOR_COINS
                    Updated_State = Display_BalanceScreen(Coins_UserBalance);
                    break;
            }
            break;

        case STATE_WAIT_FOR_COINS:
            // User is able to interact and add coins at this state
            Main_Event = Coins_BalanceProcessor();

            // Based on user input, switch.
            switch(Main_Event) {
                case EVENT_50C_ENTERED_TO_MACHINE:

                    // Adds 50 cents to balance (returns STATE_INITIALISED)
                    // new balance is shown and then back to _WAIT_FOR_COINS
                    if(Coins_UserBalance != 150 && !Drink_CheckIfStockIsCompletelyEmpty()){
                        Updated_State = Coins_inputCoins(50);
                    } else {
                        Updated_State = Display_DeniedCoins();
                    }

                    break;
                case EVENT_USER_WANTS_ALL_MONEY_RETURNED:

                    // Used to show how much was returned later
                    returned_money = Coins_UserBalance;

                    // Resets balance to 0 (returns STATE_RETURNED_MONEY)
                    Updated_State = Coins_returnCoins(&returned_money, returned_money);

                    break;
                case EVENT_USER_WANTS_TO_SELECT_DRINK:
                    Updated_State = STATE_DRINK_SELECTION;
                    break;
                case EVENT_ADMIN_MODE:
                    Updated_State = STATE_ADMIN_MODE;
                    break;
                default:
                    Updated_State = STATE_INITIALISED;
                    Display_SystemError("STATE_WAIT_FOR_COINS received unknown event");
                    break;
            }
            break;

        case STATE_DRINK_SELECTION:

            // Shows options to user
            Display_DrinkSelectionScreen(Coins_UserBalance);

            // Waits for user input
            Main_Event = Coins_CoffeeChoice(Coins_UserBalance);

            // User choice, switch.
            switch(Main_Event) {
                case EVENT_SELECTED_DRINK_CANCEL:
                    Updated_State = STATE_INITIALISED;
                    break;
                case EVENT_SELECTED_DRINK_HOTWATER:
                case EVENT_SELECTED_DRINK_COFFEE:
                case EVENT_SELECTED_DRINK_EXPRESSO:
                    // Assigns choice to variable later used in drink_dispose()
                    SelectedDrinkID = Drink_getChoiceID(Main_Event);
                    Updated_State = STATE_DRINK_CONFIRMATION;
                    break;
                default:
                    Updated_State = STATE_DRINK_SELECTION;
                    Display_SystemError("STATE_DRINK_SELECTION received unknown event");
                    break;
            }

            break;

        case STATE_DRINK_CONFIRMATION:

           // Displays are you sure
           Display_DrinkConfirmationScreen(Coins_ChosenDrinkIndex);

           // Waits for user input
           Main_Event = Drink_Confirmation();

            // User choice, switch.
            switch(Main_Event) {
                case EVENT_USER_ACCEPTS_SELECTED_DRINK:
                    Updated_State = Drink_Dispose(SelectedDrinkID);
                    Display_DrinkDisposing();
                    Updated_State = STATE_THANK_CUSTOMER;
                    break;

                case EVENT_USER_DECLINES_SELECTED_DRINK:
                    Updated_State = STATE_DRINK_SELECTION;
                    break;

                default:
                    Updated_State = STATE_DRINK_CONFIRMATION;
                    Display_SystemError("STATE_DRINK_CONFIRMATION received unknown event");
                    break;
            }

            break;

        case STATE_THANK_CUSTOMER:
            // Used in STATE_RETURNED_MONEY to display how much money was returned
            returned_money = Coins_UserBalance;

            // Displays thanks and returns change
            Updated_State = Display_ThanksScreen();
            if(returned_money > 0) {
                Updated_State = Coins_returnCoins(&returned_money, returned_money);
            }
            break;

        case STATE_RETURNED_MONEY:
            // Displays how much is returned, and then state goes back to balance screen
            Updated_State = Display_CancellationScreen(returned_money);
            break;

        case STATE_ADMIN_MODE:
            // Displays admin options
            Display_AdminScreen();

            // Waits for user input
            Main_Event = Admin_MainScreen_WaitForInput();

            // Based on user input shows different screen
            switch(Main_Event) {
                case EVENT_ADMIN_MODE_INSERTCHANGE:
                    Updated_State = STATE_ADMIN_MODE_INSERTCHANGE;
                    break;
                case EVENT_ADMIN_MODE_UPDATESTOCKS:
                    Updated_State = STATE_ADMIN_MODE_STOCKS;
                    break;
                case EVENT_ADMIN_MODE_RETURN_TO_CUSTOMER_SCREEN:
                    Updated_State = STATE_INITIALISED;
                    break;
                default:
                    Updated_State = STATE_ADMIN_MODE;
            }
            break;

        case STATE_ADMIN_MODE_STOCKS:
            // Displays the stock screen
            // Asks for which stock to edit
            Display_AdminScreen_UpdateStocks();

            // Expects user input
            int index;
            index = Admin_UpdateStock_WaitForInput();
            Main_Event = Admin_UpdateStock_Input(index);

            switch(Main_Event) {
                case EVENT_ADMIN_MODE_STOCK_SELECTED:
                    Updated_State = STATE_ADMIN_MODE_STOCKS_EDIT;
                    admin_mode_stock_index = index;
                    break;
                case EVENT_ADMIN_MODE_STOCK_EXIT:
                    Updated_State = STATE_ADMIN_MODE;
                    break;
                default:
                    Updated_State = STATE_ADMIN_MODE_STOCKS;
            }
            break;

        case STATE_ADMIN_MODE_STOCKS_EDIT:
            // Asks admin to enter new stock detail
            Display_AdminScreen_UpdateStocks_Edit(admin_mode_stock_index);

            // Expects user input
            Main_Event = Admin_UpdateStock_Edit_WaitForInput(admin_mode_stock_index);

            switch(Main_Event) {
                case EVENT_ADMIN_MODE_STOCK_UPDATED:
                    Updated_State = STATE_ADMIN_MODE_STOCKS_UPDATED;
                    break;
                default:
                    Updated_State = STATE_ADMIN_MODE_STOCKS_EDIT;
            }
            break;

        case STATE_ADMIN_MODE_STOCKS_UPDATED:
            // Displays good news and retruns to choices screen
            Display_AdminScreen_UpdateStocks_Edit_Updated();
            Updated_State = STATE_ADMIN_MODE_STOCKS;
            break;

        case STATE_ADMIN_MODE_INSERTCHANGE:
            Display_AdminScreen_InsertCoins();
            Main_Event = Admin_InsertCoin();
            switch(Main_Event) {
                case EVENT_ADMIN_MODE_INSERTCHANGE_EXIT:
                    Updated_State = STATE_ADMIN_MODE;
                    break;
                default:
                    Updated_State = STATE_ADMIN_MODE_INSERTCHANGE;
            }

            break;

        default:
            Display_SystemError("System received unknown state");
            break;

    }

    Main_Current_State = Updated_State;
}

/*
* Initilisation of all modules, starts importing data
* and initialising all required variables.
*
* @return void
*/
state_e Main_CoffeeMachine_Initialise(void)
{

   // These functions currently, only display a message
   Display_Initialise();
   Coins_Initialise();

   // Drinks read from file
   Drink_Initialise();

   // Imports admin_data.txt file
   Admin_Initialise();

   Display_DebugSystemInfo_WithDelay("Initialisation complete!");

   return STATE_INITIALISED;
}
