#ifndef ARRAYS_CHAR_H
#define ARRAYS_CHAR_H
#include "arrays.h"
void array_push_char(Array *a, const char val) {
    if(a->elm_size!=sizeof(val)) {
        printf("Warning: pushing <char> into array that appears not to be char.\a\n");
    }
    array_push(a, (const void*)&val);
}
char array_get_char(const Array* a, size_t offset) {
    if(offset >= a->length) {
        exit(83);
    }
    return ((const char*)a->array)[offset];
}

#endif // ARRAYS_CHAR_H
