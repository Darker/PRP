TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CFLAGS += -std=c99
DEFINES += HOME
DEFINES += QT_CREATOR
#dddsf

SOURCES += main.c \
    #HW02.c \
    #HWNAN.c \
    #HW03.c \
    #HW05.c \
    HW05B.c

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
    matrix.h \
    matrix_operation.h \
    HW05B.h
