#ifndef CRAZY_ARRAY_LIB666
#define CRAZY_ARRAY_LIB666

#include <stdlib.h>
#include <stdbool.h>
#include "hw_templates.h"
// Is this even used?
void swap(void*a, void*b, size_t size);

// Adopted from http://stackoverflow.com/a/400970/607407
#define AR_FOREACH(item, array, type)  \
    for(size_t keep = 1, \
            count = 0,\
            size = array->length; \
        keep && count < size; \
        keep = !keep, count++) \
      for(item = *((type*)array_get(array, count)); keep; keep = !keep)
// Remember that the type here is without the pointer type
// eg. just int for array of integers
#define AR_FOREACH_PTR(item, array, type)  \
    for(size_t keep = 1, \
            foreach_count = 0,\
            size = array->length; \
        keep && foreach_count < size; \
        keep = !keep, foreach_count++) \
      for(item = ((type*)array_get(array, foreach_count)); keep; keep = !keep)
#include "cpp_tools.h"
/** STRUCTURE Array
 *   - Dynamic array of any size and any size per element
 *   - Initialise using
 *      - array_init
 *      - void* array_create ->DO NOT FORGET TO FREE UP the pointer
 *
**/
typedef struct Array {
  //Pointer to array data
  void *array;
  //How many elements are in array
  size_t length;
  //How many elements can further fit in array (allocated memory)
  size_t size;
  //Bytes per element
  size_t elm_size;
} Array;
//Initialises the array structure
void array_init(Array* a, size_t initalSize, size_t elm_size);
/** Creates new empty array. The returned pointer must be 
 * correctly destroyed using array_destroy.
 * Inital size is allowed to be zero
 *   **/
Array* array_create(size_t elm_size, size_t initalSize);
/**
 * Allocate memory so that `count` ENTRIES fits into the array.
 * If the array already has enough memory, nothing happens.
*/
void array_reserve(Array* a, size_t count);
/**
  Free extra memory that is not occupied by elements **/
void array_free_empty_memory(Array* a);
/**
  Free memore corresponding to given item count **/
void array_free_mem(Array* a, size_t items);
/** Expands the array to required size. The value of
 * new fields is undefined. There is little reason to use
 * this instead of array_push or array_reserve */ 
void array_expand(Array* a, size_t length);
/**
 * Adds an element at the end of the array, realocating memory if necesary **/
void array_push(Array* a, const void* const element);
/**
 * Adds an element at the end of the array, realocating memory if necesary
 * THIS METHOD WAS NEVER TESTED!
**/
void array_insert(Array* const a, const size_t position, const void* const element);
void array_shift(Array* const a, void* target);

/**
 * Removes last element from the array. The element's value cannot be retrieved
 * because **** C has no templates.
 * @returns new size of the array
**/
size_t array_pop_back(Array* a);
/**
 * Removes all elements form array.
 * @returns emptiness of the void
**/
void array_clear(Array* a);

/** Retrieves pointer to entry in array. If out of bounds, exit() is called and
 * error appears on stdout (todo: change to stderr) */
void* array_get(Array* a, size_t offset);
/**
 * @brief array_get_fast is same as array_get sans error checking
 * @param a
 * @param offset
 * @return pointer to element in an array, or outside it if you pass invalid args
 */
void* array_get_fast(Array* a, size_t offset);
/**
 * @brief array_offset_valid returns true if given offset is valid in array
 */
bool array_offset_valid(Array* a, size_t offset);

/**
 * Sets value at offset in array. This ofset MUST be valid.
 * The `element` must be pointer to correct ammount of memory, that is
 * the correct type */  
void array_set(Array* a, size_t offset, const void* element);
/** Removes `length` items from array starting at `start` offset **/
void array_splice(Array* a, size_t start, size_t length);
/**
 * @brief array_move Moves one or more elements in an array
 * @param a
 * @param start index where first moved element is
 * @param end   index where first element should end up
 * @param count how many elements should be moved
 */
void array_move(Array* const a, size_t start, size_t end, size_t count);
/** tries to find an index in the array **/
int array_find(Array* a, void* value);
/** sorts the array using provided comparator method
 * if metod not provided, memcmp is used
 * Comparator signature
 *
 *  int memcmp ( const void * ptr1, const void * ptr2, size_t type_size );
 * you should be able to cast your comparator
 * to void* even if it is typed. Remember to realise corectly the size of the array entry type
**/

void array_sort(Array* a, int(*comparator)(const void*, const void*, size_t));

void array_destroy(Array* a);

void array_int_print(Array* a);
void array_push_int(Array* a, const int val);


/**String array**/
typedef struct {
  size_t length;
  size_t size;
  char ** array;
} StringArray;

void string_array_init(StringArray* a, size_t initialSize);
void string_array_push(StringArray* a, const char* string);
void string_array_print(StringArray* a);
void string_array_destroy(StringArray* a);
/**Associative array*/
typedef struct  {
    Array keys;
    Array values;
} ArrayMap;
void arrmap_init(ArrayMap* map, size_t keysz, size_t valsz, size_t initsz);
void arrmap_set(ArrayMap* map, void* key, void*value);
void* arrmap_get(ArrayMap* map, void* key);
void arrmap_sort_keys(ArrayMap* map, char direction);
void arrmap_char_int_print(ArrayMap* map, const char* name);
void arrmap_destroy(ArrayMap* map);

#if defined(PREPROCESING_FOR_ODEVSYS) || defined(TESTING_HW)
#define ARRAYS_C_QMAKE_IS_PROBLEMATIC "arrays.c"
#include ARRAYS_C_QMAKE_IS_PROBLEMATIC
#endif

#endif
