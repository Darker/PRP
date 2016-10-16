#ifndef TEXT_HELPERS_H
#define TEXT_HELPERS_H
#include <stdio.h>
// Repeats character on standard output
void char_repeat(const char character, const int repeat) {
    int i;
    for(i=0; i<repeat; ++i) {
        printf("%c", character);
    }
}
// Expects 0 terminated string!
// Repeat argument shall indicate number of individual characters printed out, not full cycles
void chars_repeat(const char* chars, const int repeat) {
    int charpos = 0;
    int chars_printed = 0;
    while(chars_printed < repeat) {
        if(chars[charpos] == 0)
            charpos = 0;
        printf("%c", chars[charpos]);
        charpos++;
        chars_printed++;
    }
}
void spaces(const int amount) {
    char_repeat(' ', amount);
}
void nl() {
    printf("\n");
}

#endif // TEXT_HELPERS_H
