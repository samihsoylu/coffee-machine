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
#ifndef DRINK_H
#define DRINK_H

void Drink_Initialise(void);
state_e Drink_Dispose(const int i);
event_e Drink_Confirmation();
int Drink_getChoiceID(const event_e choice);

void Drink_StockUpdate();
void Drink_ExportLogistics();
void Drink_ImportLogistics();

int Drink_CheckIfStockIsCompletelyEmpty();

typedef struct {
    char title[255];
    int price;
    int amount_available;
    char isInStock[4];
    int total_sold;
} drinks_t;

extern drinks_t Drink[3];

#endif
