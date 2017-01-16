#ifndef ARRAYS_ODEVSYS_IS_PROBLEMATIC_621
#define ARRAYS_ODEVSYS_IS_PROBLEMATIC_621
#include "arrays.h"
#include <string.h>

// Is this even used?
void swap(void*a, void*b, size_t size) {
  void* tmp = calloc(size, 1);
  memcpy(tmp, a, size);
  memcpy(a, b, size);
  memcpy(b, tmp, size);
  free(tmp);
}
#include "cpp_tools.h"

//Initialises the array structure
void array_init(Array* a, size_t initalSize, size_t elm_size) {
  array_reserve(a, initalSize);
  
  if(a->array==NULL) {
    printf("ERROR: failed to allocate memory!\n");
    exit(1);
  }
  
  a->elm_size = elm_size;
  a->length = 0;
}
Array* array_create(size_t elm_size, size_t initalSize) {
  Array* a = NEW(Array);
  a->array = NULL;
  a->elm_size = elm_size;
  a->length = 0;
  a->size = 0;
  if(initalSize>0) {     
    array_reserve(a, initalSize);
  }
  
  return a;
}
void array_reserve(Array* a, size_t count) {
  if(a->size<count) {
    void* newPointer = NULL;
    //printf("Allocating more memory!!!\n");
    if(a->array == NULL)
        newPointer = (void*)malloc(count * a->elm_size);
    else
        newPointer = (void*)realloc(a->array, count * a->elm_size);
    if(newPointer==NULL) {
      printf("ERROR: failed to allocate memory!\n");
      exit(1);
    }
    a->array = newPointer;
    a->size = count;
  }
} 
void array_free_empty_memory(Array* a)
{
    if(a->length < a->size) {
        void* newPointer = NULL;
        newPointer = (void*)realloc(a->array, a->length * a->elm_size);
        if(newPointer==NULL) {
            printf("ERROR: failed to de-allocate memory!\n");
            exit(1);
        }
        a->array = newPointer;
        a->size = a->length;
    }
}
void array_free_mem(Array* a, size_t items) {
    size_t orig_size = a->size;
    size_t new_size = orig_size>items?orig_size-items:0;
    if(new_size>0) {
        void* newPointer = NULL;
        newPointer = (void*)realloc(a->array, new_size * a->elm_size);
        if(newPointer==NULL) {
            printf("ERROR: failed to de-allocate memory!\n");
            exit(1);
        }
        a->array = newPointer;
        a->size = new_size;
        if(a->length > new_size)
            a->length = new_size;
    }
    else {
        free(a->array);
        a->array = NULL;
    }
    a->size = new_size;
}

void array_expand(Array* a, size_t length) {
  if(length>a->length) {
    if(length>a->size)
      array_reserve(a, length);
    a->length = length;
  }
}
/**
 * Adds an element at the end of the array, realocating memory if necesary **/
void array_push(Array* a, const void* const element) {
  if (a->length >= a->size) {
    array_reserve(a, a->length + 1);
  }
  memcpy(((char*)a->array) + (a->length++)*a->elm_size, element, a->elm_size);
}
/**
 * Adds an element at the end of the array, realocating memory if necesary
 * THIS METHOD WAS NEVER TESTED!
**/
void array_insert(Array* const a, const size_t position, const void* const element) {
  if(position==a->length-1) {
      array_push(a, element);
  }
  if(position>=a->length) {
      chyba("Error: insert at %d out of bounds of this array.\a", 173, (int)position)
  }
  if (a->length >= a->size) {
    array_reserve(a, a->length + 1);
  }
  // Make space for new element
  for(size_t i=a->length-1; i>=position; --i) {
      memcpy(array_get_fast(a, i+1), array_get_fast(a, i), a->elm_size);
  }

  memcpy(array_get_fast(a, position), element, a->elm_size);
  ++a->length;
}
void array_shift(Array* const a, void* target) {
    if(target != NULL) {
        memcpy(target, a->array, a->elm_size);
    }
    // Move all elements one to the left
    for(size_t i=1; i<a->length; ++i) {
        memcpy(array_get_fast(a, i-1), array_get_fast(a, i), a->elm_size);
    }
    --(a->length);
}

/**
 * Removes last element from the array. The element's value cannot be retrieved
 * because **** C has no templates.
 * @returns new size of the array
**/
size_t array_pop_back(Array* a) {
    if(a->length <= 0) {
      printf("Error: pop_back from empty array.\a\n");
      exit(168);
    }
    return --(a->length);
}
/**
 * Removes all elements form array.
 * @returns emptiness of the void
**/
void array_clear(Array* a) {
    if(a->length > 0) {
      a->length = 0;
    }
}

/** Retrieves pointer to entry in array. If out of bounds, exit() is called and
 * error appears on stdout (todo: change to stderr) */
void* array_get(Array* a, size_t offset) {
  if(offset>=a->length) {
    printf("Error: access of unallocated offset %d in array.\a\n", (int)offset);
    exit(166);
  }
  return (void*)(((char*)a->array) + offset*a->elm_size);
}
/**
 * @brief array_get_fast is same as array_get sans error checking
 * @param a
 * @param offset
 * @return pointer to element in an array, or outside it if you pass invalid args
 */
void* array_get_fast(Array* a, size_t offset) {
  return (void*)((char*)a->array + offset*a->elm_size);
}
/**
 * Sets value at offset in array. This ofset MUST be valid.
 * The `element` must be pointer to correct ammount of memory, that is
 * the correct type */  
void array_set(Array* a, size_t offset, const void* element) {
  if(offset>=a->length) {
    printf("Error: access of unallocated offset %d in array. (There is no implicit allocation)\a\n", (int)offset);
    exit(167);
  }
  memcpy(((char*)a->array) + (offset)*a->elm_size, element, a->elm_size);
}

bool array_offset_valid(Array* a, size_t offset) {
    return offset<a->length;
}
/** Removes `length` items from array starting at `start` offset **/
void array_splice(Array* a, size_t start, size_t length) {
  if(length<=0) {
     return;
  }
  else 
  //Error on undefined offset
  if(start>=a->length) {
    log_err("Error: invalid splice start offset [%d]\a\n", (unsigned int)start);
  }
  //If the deleted fragment ends at the end of the array, just change length
  else if(start+length>=a->length) {
    a->length = start;
  }
  //Alter length and copy elms from end to their new position
  else {
    memcpy(
      ((char*)a->array) + (start)*a->elm_size, 
      ((char*)a->array) + (start+length)*a->elm_size,
      (a->length-(start+length))*a->elm_size
    );
    a->length = a->length-length;
  }
}

void array_move(Array* const a, size_t start, size_t end, size_t count) {
    if(start==end || count==0)
        return;
    // always walk away from destination tile
    int direction = start<end?-1:1;
    // If moving back, we start at start and move forward
    // if moving to the end, we start at last item and move towards start
    int begin = start<end?start+((int)count-1) : start;
    int finish = begin+(int)direction*count;
    if(!array_offset_valid(a, start) || !array_offset_valid(a, start+count-1) ||
       !array_offset_valid(a, end) || !array_offset_valid(a, end+count-1)
    )
        chyba("Moving items out of bounds!%s", 24, "");
    // by how many items are we moving. This is awesome, coz it's
    // always the same regardless of direction
    long offset = (long)end - (long)start;
    //log_info("Moving %d items by $d fields.", (int)count, (int)offset);
    //char* const ptr = a->array;

    for(;begin != finish; begin+=direction) {
        memcpy(array_get_fast(a, begin+offset), array_get_fast(a, begin), a->elm_size);
    }
}

/** tries to find an index in the array **/
int array_find(Array* a, void* value) {
  size_t elmsz = a->elm_size;
  size_t l = a->length;
  for(size_t i=0; i<l; i++) {
    if(memcmp((void*)((char*)a->array+i*(elmsz)), value, elmsz)==0) {
       return i;
    }
  }
  return -1;
}
/** sorts the array using provided comparator method
 * if metod not provided, memcmp is used
 * Comparator signature
 *
 *  int memcmp ( const void * ptr1, const void * ptr2, size_t type_size );
 * you should be able to cast your comparator
 * to void* even if it is typed. Remember to realise corectly the size of the array entry type
**/
#include "q_sort_flex.h"
void array_sort(Array* a, int(*comparator)(const void*, const void*, size_t)) {
    if(comparator == NULL)
        comparator = &memcmp;
    qsort_flexible(a->array, a->length, a->elm_size, comparator);
}

void array_destroy(Array* a) {
  free(a->array);
  a->array = NULL;
  a->length = a->size = 0;
  free(a);
}

void array_int_print(Array* a) {
    if(a->elm_size!=sizeof(int)) {
      printf("Warning: can't print array as <int> array - this array's elements size is not that of <int>.\a\n");
    }
    size_t l=a->length;
    printf("Array of %d elements and %d fields allocated:\n", (unsigned int)l, (unsigned int)a->size);
    for(size_t i=0; i<l; i++) {
      printf("  a[%d] = %d\n", (unsigned int)i, *(int*)array_get(a, i));
    }
}
void array_push_int(Array* a, const int val) {
    if(a->elm_size!=sizeof(int)) {
      printf("Warning: pushing <int> into array that appears not to be int.\a\n");
    }
    array_push(a, (const void*)&val);
}


/**String array**/

void string_array_init(StringArray* a, size_t initialSize) {
  a->array = (void *)calloc(initialSize * sizeof(char*), 1);
  if(a->array==NULL) {
    log_err("failed to allocate memory!%s", "");
  }
  a->length = 0;
  a->size = initialSize;
}
void string_array_push(StringArray* a, const char* string) {
  if (a->length == a->size) {
    a->size +=1+a->size/2;
    a->array = (char**)realloc(a->array, a->size *sizeof(char*));
  }
  char** pos = &a->array[a->length++];
  
  *pos = (char*) calloc(sizeof(char)*(strlen(string)+1), 1);
  if(*pos==NULL) {
    printf("ERROR: failed to allocate memory!\n");
    exit(1);
  }  
  memcpy(*pos, string, sizeof(char)*(strlen(string)+1));

}
void string_array_print(StringArray* a) {
    size_t l=a->length;
    printf("Array of %d strings and %d fields allocated:\n", (unsigned int)l, (unsigned int)a->size);
    for(size_t i=0; i<l; i++) {
      printf("  a[%d] = \"%s\"\n", (unsigned int)i, a->array[i]);
    }
}
void string_array_destroy(StringArray* a) { 
  size_t l=a->length;
  for(size_t i=0; i<l; i++) {
    free((void*)(a->array[i]));
  }
  free(a->array);
}
/**Associative array*/

void arrmap_init(ArrayMap* map, size_t keysz, size_t valsz, size_t initsz) {
    array_init(&(map->keys), initsz, keysz);
    array_init(&(map->values), initsz, valsz);
}
void arrmap_set(ArrayMap* map, void* key, void*value) {
    int offset = array_find(&(map->keys), key);
    if(offset!=-1) {
      array_set(&(map->values), offset, value);
    }
    else {
      array_push(&(map->keys), key);
      array_push(&(map->values), value);
    }
}
void* arrmap_get(ArrayMap* map, void* key) {
    int offset = array_find(&(map->keys), key);
    if(offset!=-1) {
      return array_get(&(map->values), offset);
    }
    else {
      return NULL;
    }
}
void arrmap_sort_keys(ArrayMap* map, char direction) {
  //Onnly allows direction +-1, 0 means +1
  if(direction>1||direction==0) {
    direction = 1;
  }
  else if(direction<-1) {
    direction = -1;
  }

  //Cache refference to keys by which we're sorting
  Array* keys = &map->keys;
  //Cache length
  size_t length= keys->length;
  //tmp refferences on specific key pointers
  void* key1, *key2;
  //Result of the comparison (tmp)
  int cmpres;

  for (size_t c = 0 ; c < ( length - 1 ); c++)
  {
    for (size_t d = 0 ; d < length - c - 1; d++)
    {
      //Get both key refferences
      key1 = array_get(keys, d);
      key2 = array_get(keys, d+1);
      //Compare memory that belongs to the keys
      cmpres = memcmp(key1, key2, keys->elm_size);
      if(cmpres>0)
        cmpres = 1;
      else if(cmpres<0)
        cmpres = -1;
      //If the comparison does not match sort order
      if (cmpres!=0&&cmpres!=direction)
      {
           if(cmpres<0) {
             log_info("%d < %d", *(char*)key1, *(char*)key2);
           }
           else { 
             log_info("%d > %d", *(char*)key1, *(char*)key2);
           }
           //Swap both keys
           log_info("Swapping %d with %d because cmpres=%d and direction=%d", *(char*)key1, *(char*)key2, cmpres, direction);
           swap(key1, key2, keys->elm_size);
           log_info("         %d is %d", *(char*)key1, *(char*)key2);
           //Get value pointers and swap them too
           key1 = array_get(&(map->values), d);
           key2 = array_get(&(map->values), d+1);
           swap(key1, key2, map->values.elm_size);
      }
    }
  }
}
void arrmap_char_int_print(ArrayMap* map, const char* name) {
  size_t l = map->keys.length;
  char* character;
  unsigned int* integer;
  
  
  for(size_t i=0; i<l; i++) {
    character = (char*)array_get(&map->keys, i);
    integer = (unsigned int*)  array_get(&map->values, i);
    printf("  %s[\'%c\']~[%d] = %d\n", name, *character,(int)*character, *integer);
  
  }
}
void arrmap_destroy(ArrayMap* map) {
  array_destroy(&map->keys);
  array_destroy(&map->values);
}

#endif


