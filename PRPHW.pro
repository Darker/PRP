TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += HOME
#ddds

SOURCES += main.c \
    HW02.c \
    HWNAN.c \
    HW03.c

HEADERS += \
    hw_templates.h \
    HW02.h \
    HWNAN.h \
    HW03.h \
    math_helpers.h \
    types.h \
    text_helpers.h
