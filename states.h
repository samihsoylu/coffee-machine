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
#ifndef STATES_H
#define STATES_H

typedef enum {
    // Customer
	STATE_START,
	STATE_INITIALISED,
    STATE_WAIT_FOR_COINS,
    STATE_RETURNED_MONEY,
    STATE_DRINK_SELECTION,
    STATE_DRINK_CONFIRMATION,
    STATE_THANK_CUSTOMER,

    // Administrator
    STATE_ADMIN_MODE,
    STATE_ADMIN_MODE_STOCKS,
    STATE_ADMIN_MODE_STOCKS_EDIT,
    STATE_ADMIN_MODE_STOCKS_UPDATED,
    STATE_ADMIN_MODE_INSERTCHANGE
} state_e;

typedef enum {
    // Customer
    EVENT_READY,
    EVENT_50C_ENTERED_TO_MACHINE,
    EVENT_USER_WANTS_TO_SELECT_DRINK,
    EVENT_USER_WANTS_ALL_MONEY_RETURNED,
    EVENT_NOT_ENOUGH_MONEY_FOR_SELECTED_PRODUCT,
    EVENT_ENOUGH_MONEY_FOR_SELECTED_PRODUCT,
    EVENT_SELECTED_DRINK_HOTWATER,
    EVENT_SELECTED_DRINK_EXPRESSO,
    EVENT_SELECTED_DRINK_COFFEE,
    EVENT_SELECTED_DRINK_CANCEL,
    EVENT_USER_ACCEPTS_SELECTED_DRINK,
    EVENT_USER_DECLINES_SELECTED_DRINK,

    // Administrator
    EVENT_ADMIN_MODE,
    EVENT_ADMIN_MODE_UPDATESTOCKS,
    EVENT_ADMIN_MODE_STOCK_UPDATED,
    EVENT_ADMIN_MODE_STOCK_SELECTED,
    EVENT_ADMIN_MODE_STOCK_EXIT,
    EVENT_ADMIN_MODE_INSERTCHANGE,
    EVENT_ADMIN_MODE_RETURN_TO_CUSTOMER_SCREEN,
    EVENT_ADMIN_MODE_INSERTCHANGE_OK,
    EVENT_ADMIN_MODE_INSERTCHANGE_EXIT
} event_e;

#endif // STATES_H
