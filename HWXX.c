#include "HWXX.h"
#ifdef QT_CREATOR
    //#define INPUT_FILE "D:\\skola\\PRP\\HW\\PRPHW\\TEST-HW06\\opt-pub01.in"
#endif
int HWXX(int argc, char *argv[]) {
    FILE* input = stdin;
#ifdef INPUT_FILE
    const char* file = INPUT_FILE;
    if((input = fopen(file, "r")) == NULL) {
        chyba("Soubor \"%s\" neexistuje.", 6+6+6, file);
    }
#endif
}

HW_MAIN(HWXX)
