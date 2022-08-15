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
#ifndef DISPLAY_H
#define DISPLAY_H

/*
 * List of includes for cross-platform delay
 */
#ifdef _WIN32
    #include <Windows.h>
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    #include <unistd.h>

#else
    #error "OS not supported."

#endif

/* Initial prototypes */
void Display_Initialise(void);
void Display_DebugSystemInfo(const char *text);
void Display_SystemError(const char *text);
void Display_DebugSystemInfo_WithDelay(const char *text);
void Display_Delay(unsigned int ms);

/* Program prototypes */
state_e Display_BalanceScreen(const int balance);
state_e Display_CancellationScreen(const int coins);
void Display_DrinkSelectionScreen(const int userBalance);
state_e Display_SelectedDrink(const int drink_id);
state_e Display_ThanksScreen();
void Display_ClearScreen(void);
void Display_DrinkConfirmationScreen(const int i);
void Display_AdminScreen();
void Display_AdminScreen_UpdateStocks();
void Display_AdminScreen_UpdateStocks_Edit(int choice);
void Display_AdminScreen_UpdateStocks_Edit_Updated();
state_e Display_DeniedCoins();
void Display_AdminScreen_InsertCoins();
void Display_DrinkDisposing();

#endif
