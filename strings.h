#ifndef STRINGS_H
#define STRINGS_H
#include "arrays.h"
#include "arrays_char.h"
#include <string.h>
#include "types.h"
#include <stdarg.h>
// used for min_3
#include "math_helpers.h"

/**String**/
typedef struct {
  Array* char_array;
} String;
const char* str_cstr(const String* const);
size_t str_len(const String* const s);
bool str_empty(const String* const s);
char str_at(const String* const s, size_t offset);
void str_add_string(String *s, const char *c_str);
void str_cpy(String* s, const String* const input);

String* str_create(size_t initsize) {
    // one more for \0
    initsize++;
    String* s = NEW(String);
    s->char_array = NULL;
    s->char_array = array_create(sizeof(char), initsize);
    array_push_char(s->char_array, '\0');
    return s;
}
String* str_from_cstr(const char* chars) {
    String* s = str_create(strlen(chars));
    str_add_string(s, chars);
    return s;
}
String* str_clone(const String* const s) {
    String* new_str = str_create(s->char_array->length);
    str_cpy(new_str, s);
    return new_str;
}
//PRIVATE!!!
// opens string for writing by removing the leading \0
// this function is not reentrant
void str_open(String* s) {
    if(str_at(s, str_len(s)) != '\0')
        log_warn("Warning, given string ends with %c instead of \\0", str_at(s, str_len(s)));
    else {
        // remove the trailing \0
        array_pop_back(s->char_array);
    }
}
void str_close(String* s) {
    if(str_at(s, str_len(s)) == '\0')
        log_warn("Warning, given string %s already ends with \\0", str_cstr(s));
    else {
        // remove the trailing \0
        array_push_char(s->char_array, '\0');
    }
}
/**
 * @brief str_reserve reserves memory given by size. This can be used
 *  to prevent multiple realloc calls in loops etc.
 * @param s string
 * @param size new size. Note that this does not increase length of the string,
 * just reserved the memory size
 */
void str_reserve(String* s, size_t size) {
    // +1 for \0
    array_reserve(s->char_array, size+1);
}
void str_add_char(String* s, const char ch) {
    str_open(s);
    // insert char
    array_push_char(s->char_array, ch);
    // add trailing '\0'
    str_close(s);
}

void str_add_string(String* s, const char* c_str) {
    size_t len = strlen(c_str);
    str_reserve(s, len+s->char_array->length);
    // remove the trailing \0
    array_pop_back(s->char_array);
    while((*c_str)!='\0') {
        array_push_char(s->char_array, *c_str);
        ++c_str;
    }
    array_push_char(s->char_array, '\0');
}
void str_add_string_class(String* s, const String* const str) {
    str_add_string(s, str_cstr(str));
}
/** Set char at position **/
void str_set_char(String* s, const char chr, size_t pos) {
    if(pos >= s->char_array->length-1) {
        exit(22);
    }
    array_set(s->char_array, pos, &chr);
}
/** Copies string `input` to string `s`**/
void str_cpy(String* s, const String* const input) {
    s->char_array->length = 0;
    str_reserve(s, str_len(input));
    char chr;
    AR_FOREACH(chr, input->char_array, char) {
        array_push_char(s->char_array, chr);
    }
}
/** Retrieves const pointer to contents */
const char* str_cstr(const String* const s) {
    return (const char*)s->char_array->array;
}
size_t str_len(const String* const s) {
    return s->char_array->length-1;
}
bool str_is_empty(const String* const s) {
    return s->char_array->length<2;
}
char str_at(const String* const s, size_t offset) {
    return array_get_char(s->char_array, offset);
}
char str_last_char(const String* const s) {
    return str_at(s, str_len(s)-1);
}
/**
 * Returns POINTER to character at given offset. ONLY FOR IMMEDIATE USE!!!
 */
char* str_at_ref(const String* const s, size_t offset) {
    if(offset<str_len(s))
        return ((char*)s->char_array->array)+offset;
    else {
        chyba("Tried to access offset %ud in a string %ud characters long.", 113, offset, str_len(s));
    }
}
int str_index_of_char(const String* const s, const char character) {
    for(size_t i=0, l=str_len(s); i<l; ++i) {
        if(str_at(s, i) == character) {
            return i;
        }
    }
    return -1;
}
bool str_contains_char(const String* const s, const char character) {
    return str_index_of_char(s, character)>=0;
}
/** compares two strings.  example for "a" and "b" this function returns 1 abs('a'-'b')
  output is sum of absolute values of each char's differences.
  if lengths differ, every extra character adds +127 to diff.
**/
int str_cmp(const String* const s1, const String* const s2) {
    const size_t s1_len = str_len(s1);
    const size_t s2_len = str_len(s2);
    const size_t max_len = s1_len>s2_len?s1_len:s2_len;
    const char* s1_str = str_cstr(s1);
    const char* s2_str = str_cstr(s2);


    int sum = 0;
    for(size_t i=0; i<max_len; ++i) {
        // we're out of s1's size already
        if(i>s1_len)
            sum+=127;
        else if(i>s2_len)
            sum+=127;
        else {
            char diff = s1_str[i]-s2_str[i];;
            sum+=diff>=0?diff: -1*diff;
        }

    }
    return sum;
}
//#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
//int levenshtein(const char *s1, const char *s2) {
//    unsigned int x, y, s1_len, s2_len;
//    s1_len = strlen(s1);
//    s2_len = strlen(s2);
//    unsigned int matrix[s2_len+1][s1_len+1];
//    matrix[0][0] = 0;
//    for (x = 1; x <= s2_len; x++)
//        matrix[x][0] = matrix[x-1][0] + 1;
//    for (y = 1; y <= s1_len; y++)
//        matrix[0][y] = matrix[0][y-1] + 1;
//    for (x = 1; x <= s2_len; x++)
//        for (y = 1; y <= s1_len; y++)
//            matrix[x][y] = MIN3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));
//#ifdef HOME
//    printf("Matrix:\n");
//    for(size_t j=0; j<=s2_len; ++j) {
//        for(size_t i=0; i<=s1_len; ++i) {
//           printf("%3d ", matrix[i][j]);
//        }
//        printf("\n\n");
//    }
//    printf("----------------\n");
//#endif
//    return(matrix[s2_len][s1_len]);
//}
int str_levenshtein_dist(const String* const s1, const String* const s2) {
    const size_t s1_len = str_len(s1);
    const size_t s2_len = str_len(s2);
    const char* s1_str = str_cstr(s1);
    const char* s2_str = str_cstr(s2);

    int (*matrix)[s1_len+1][s2_len+1] = malloc(sizeof(float[s1_len+1][s2_len+1]));
//    int** const matrix = (int**)calloc(sizeof(int*), s1_len+1);
//    memset((void*)matrix, 0, sizeof(matrix));
//    for(size_t i=0; i<=s1_len; ++i) {
//        matrix[i] = (int*)calloc(sizeof(int), s1_len+2);
//    }

    memset(matrix, 0, sizeof(int[s1_len+1][s2_len+1]));

    for(size_t i=0; i<=s1_len; ++i) {
        (*matrix)[i][0] = (int)i;
    }
    for(size_t i=0; i<=s2_len; ++i) {
        (*matrix)[0][i] = (int)i;
    }

    for(size_t j=1; j<=s2_len; ++j) {
        for(size_t i=1; i<=s1_len; ++i) {

            if( s1_str[i-1]==s2_str[j-1] ) {
                (*matrix)[i][j] = (int)(*matrix)[i-1][j-1];
            }
            else {
                (*matrix)[i][j] = (int)min_3(
                            (*matrix)[i-1][j  ] + 1,
                            (*matrix)[i  ][j-1] + 1,
                            (*matrix)[i-1][j-1] + 1
                );
            }

        }
    }
#ifdef HOME
//    printf("Matrix:\n");
//    for(size_t j=0; j<=s2_len; ++j) {
//        for(size_t i=0; i<=s1_len; ++i) {
//           printf("%3d ", (*matrix)[i][j]);
//        }
//        printf("\n\n");
//    }
//    printf("----------------\n");
#endif
//    if( matrix[s1_len][s2_len]!=levenshtein(s1_str, s2_str) ) {
//        chyba_debug("Levenshtein output does not match: %d vs %d", 112, matrix[s1_len][s2_len], levenshtein(s1_str, s2_str));
//    }
    int result = (*matrix)[s1_len][s2_len];
//    for(size_t i=0; i<=s1_len; ++i) {
//        free(matrix[i]);
//    }
    free(matrix);
    return result;
}
void str_clear(String* s) {
  array_clear(s->char_array);
  str_add_char(s, '\0');
}
/**
 * @brief str_destroy
 * @param s
 * @return NULL for convenience
 */
String* str_destroy(String* s) {
  array_destroy(s->char_array);
  free(s);
  return NULL;
}
/** Varargs method to create matrix. Mess up the dimensions and you're fucked! **/
String* str_destroy_va( String* first, ... ) {

    //log_info("Making %d by %d matrix, max indexes [%d, %d].", rows, cols, rows-1, cols-1);
    va_list arguments;
    /* Initializing arguments to store all values after num */
    va_start ( arguments, first );
    /* we still rely on the function caller to tell us how
     * many there are */
    str_destroy(first);
    String* pointer = NULL;
    while( (pointer = va_arg ( arguments, String* ))!=NULL ) {
        str_destroy(pointer);
    }
    va_end ( arguments );                  // Cleans up the list

    return NULL;
}
#endif // STRINGS_H
