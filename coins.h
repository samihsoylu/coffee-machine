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
#ifndef COINS_H
#define COINS_H

// Function prototypes
void Coins_Initialise();
event_e Coins_checkCoins();
event_e Coins_BalanceProcessor();
state_e Coins_inputCoins(const int enteredCoins);
state_e Coins_returnCoins(int *amountToReturn, int amount_returned);
event_e Coins_CoffeeChoice(const int balance);

// Global variable
extern int Coins_UserBalance;
extern int Coins_ChosenDrinkIndex;



#endif
