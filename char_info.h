#ifndef CHAR_INFO_H
#define CHAR_INFO_H
#include "types.h"
#define NO_LOWERCASE_LETTERS 'z'-'a'
#define NO_UPPERCASE_LETTERS 'Z'-'A'
#define NO_LETTERS NO_LOWERCASE_LETTERS+NO_UPPERCASE_LETTERS
bool is_lowercase_letter(const char chr) {
    return chr>='a' && chr<='z';
}
bool is_uppercase_letter(const char chr) {
    return chr>='A' && chr<='Z';
}
bool is_letter(const char chr) {
    return is_lowercase_letter(chr) || is_uppercase_letter(chr);
}
bool is_number(const char chr) {
    return chr>='0' && chr<='9';
}
bool is_alphanumeric(const char chr) {
    return is_letter(chr) || is_number(chr);
}

/**
 * @brief next_letter returns next nth letter character counting from
 *                    a to z, then from A to Z. This loop is wrapped around
 * @param chr
 * @return character next to given character. If given character is not letter, 'a' is returned
 */
char next_letter(char chr) {
    if(is_letter(chr)) {
        ++chr;
        if(chr==('z'+1))
            chr = 'A';
        if(chr==('Z'+1))
            chr = 'a';
        return chr;
    }
    return 'a';
}
char next_nth_letter(char chr, char count) {
    if(is_letter(chr)) {
        bool is_lower = is_lowercase_letter(chr);
        const char a = is_lower?'a':'A';
        const char z = is_lower?'z':'Z';
        const char A = is_lower?'A':'a';
        const char Z = is_lower?'Z':'z';

        //printf("Moving %c by %d\n", chr, (int)count);
        // joke's on you, there's the same ammout of both kinds of letters
        const unsigned char no_letters= is_lower?NO_LOWERCASE_LETTERS: NO_UPPERCASE_LETTERS;
        // from now on, we write the algorithm as if we were always used lowercase
        // by swapping lowercase to upper using nice variable names
        if((chr+count)>z || (chr+count)<a) {
            // remmember how many letters did we move
            char moves_used = count>0?z-chr: chr-a;
            char direction = count>0?1:-1;
            chr = count>0?z:a;
            // remaining letters to move
            count-=moves_used*direction;
            //printf("  Remains to move %c by %d\n", chr, (int)count);
            // one more to "wrap around"
            chr = chr==z?A:Z;
            count -= direction;
            //printf("  Wrap around, remains %c by %d\n", chr, (int)count);
            // now, if count is bigger than no letters
            // we have to wrap around again
            if(count>no_letters)
                return next_nth_letter(chr, count);
            else
                return chr+count;

        }
        else {
            return chr+count;
        }
    }
    return 'a';
}

#endif // CHAR_INFO_H
