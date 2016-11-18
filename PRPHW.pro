TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CFLAGS += -std=c99
DEFINES += HOME
#dddsf

SOURCES += main.c \
    HW02.c \
    HWNAN.c \
    HW03.c \
    HW05.c

HEADERS += \
    hw_templates.h \
    HW02.h \
    HWNAN.h \
    HW03.h \
    math_helpers.h \
    types.h \
    text_helpers.h \
    HW05.h \
    arrays.h \
    matrix.h
