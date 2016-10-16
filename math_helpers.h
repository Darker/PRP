#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H
#include "hw_templates.h"
#include "types.h"
bool readInt(int* target) {
    const int success = scanf("%d", target);
    if(!success) {
        chyba(invalid_input_error, invalid_input_error_code);
    }
    return true;
}
bool validateRange(const int value, const int min, const int max) {
    if(value<min || value>max) {
        return false;
    }
    return true;
}
void validateRangeErr(const int value, const int min, const int max) {
    if(!validateRange(value, min, max)) {
        chyba(range_error, range_error_code);
    }
}

bool readInRange(int* target, const int min, const int max) {
    readInt(target);
    validateRangeErr(*target, min,max);
    return true;
}




#endif // MATH_HELPERS_H
