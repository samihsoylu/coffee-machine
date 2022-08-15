TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    coins.c \
    display.c \
    drink.c \
    admin_handler.c

HEADERS += \
    states.h \
    coins.h \
    display.h \
    drink.h \
    settings.h \
    admin_handler.h
