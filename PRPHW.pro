TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CFLAGS += -std=c99
DEFINES += HOME
DEFINES += QT_CREATOR
#ddsdsf

SOURCES += main.c \
    #HW02.c \
    #HWNAN.c \
    HW03.c \
    HW05.c \
    #HW05B.c \
    #HW06.c \
    #HW07.c \
    #node_queue.c \
    arrays.c \
    queue.c \
    HW08.c


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
    HW05B.h \
    HW06.h \
    HWXX.h \
    strings.h \
    arrays_char.h \
    char_info.h \
    HW07.h \
    HW07/lock.h \
    cpp_tools.h \
    queue.h \
    HW08.h
