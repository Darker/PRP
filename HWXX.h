// test/debug:
//    gcc HWXX.c -DHOME -DNORMAL_MAIN -std=c99 -oHWXX -ggdb && ./HWXX
// test normal:
//    gcc HWXX.c -std=c99 -oHWXX && ./HWXX
#ifndef HWXX_H
#define HWXX_H
#include "hw_templates.h"
int HWXX(int argc, char *argv[]);
#define CURRENT_HW HWXX
#endif
