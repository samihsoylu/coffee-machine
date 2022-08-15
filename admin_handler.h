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
#ifndef ADMIN_HANDLER_H
#define ADMIN_HANDLER_H

#include "states.h"

// Prototypes
void Admin_ImportData();
void Admin_ExportData();
void Admin_Initialise();
event_e Admin_MainScreen_WaitForInput();
int Admin_UpdateStock_WaitForInput();
event_e Admin_UpdateStock_Edit_WaitForInput(int index);
event_e Admin_InsertCoin();
event_e Admin_UpdateStock_Input(int index);

// Global var
typedef struct {

    // Machine constants
    int change_available;

    // Statistics
    int total_earned_money;
    int total_sold_drinks;

} admin_t;

admin_t admin_data;

#endif // ADMIN_HANDLER_H
