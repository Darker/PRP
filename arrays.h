#ifndef CRAZY_ARRAY_LIB666
#define CRAZY_ARRAY_LIB666

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hw_templates.h"
// Is this even used?
void swap(void*a, void*b, size_t size) {
  void* tmp = calloc(size, 1);
  memcpy(tmp, a, size);
  memcpy(a, b, size);
  memcpy(b, tmp, size);
  free(tmp);
}
/** STRUCTURE Array
 *   - Dynamic array of any size and any size per element
 *   - Initialise using 
 *      - array_init
 *      - void* array_create ->DO NOT FORGET TO FREE UP the pointer  
 *  
**/ 
// Adopted from http://stackoverflow.com/a/400970/607407
#define AR_FOREACH(item, array, type)  \
    for(size_t keep = 1, \
            count = 0,\
            size = array->length; \
        keep && count < size; \
        keep = !keep, count++) \
      for(item = *((type*)array_get(array, count)); keep; keep = !keep)
#define AR_FOREACH_PTR(item, array, type)  \
    for(size_t keep = 1, \
            count = 0,\
            size = array->length; \
        keep && count < size; \
        keep = !keep, count++) \
      for(item = ((type*)array_get(array, count)); keep; keep = !keep)
#define NEW(Type) (Type*)calloc(sizeof(Type),1)

typedef struct {
  //Pointer to array data
  void *array;
  //How many elements are in array
  size_t length;
  //How many elements can further fit in array (allocated memory)
  size_t size;
  //Bytes per element
  size_t elm_size;
} Array;
void array_reserve(Array* a, size_t count);
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
/** Creates new empty array. The returned pointer must be 
 * correctly destroyed using array_destroy.
 * Inital size is allowed to be zero
 *   **/
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
/**
 * Allocate memory so that `count` ENTRIES fits into the array.
 * If the array already has enough memory, nothing happens.
*/
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
/** Expands the array to required size. The value of
 * new fields is undefined. There is little reason to use
 * this instead of array_push or array_reserve */ 
void array_expand(Array* a, size_t length) {
  if(length>a->length) {
    if(length>a->size)
      array_reserve(a, length);
    a->length = length;
  }
}
/**
 * Adds an element at the end of the array, realocating memory if necesary **/
void array_push(Array* a, const void* element) {
  if (a->length >= a->size) {
    array_reserve(a, a->length + 1);
  }
  memcpy(((char*)a->array) + (a->length++)*a->elm_size, element, a->elm_size);
}
/** Retrieves index of array. If out of bounds, exit() is called and 
 * error appears on stdout (todo: change to stderr) */
void* array_get(Array* a, size_t offset) {
  if(offset>=a->length) {
    printf("Error: access of unallocated offset %d in array.\a\n", (int)offset);
    exit(166);
  }
  return (void*)(((char*)a->array) + offset*a->elm_size);
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
/** Removes `length` chars from array starting at `start` offset **/
void array_splice(Array* a, size_t start, size_t length) {
  if(length<=0) {
     return;
  }
  else 
  //Error on undefined offset
  if(start>=a->length) {
    log_err("Error: invalid splice start offset [%d]\a\n", (unsigned int)start);
  }
  else
  //If the deleted fragment ends at the end of the array, just change length
  if(start+length>=a->length) {
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
typedef struct {
  size_t length;
  size_t size;
  char ** array;
} StringArray;

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
typedef struct  {
    Array keys;
    Array values;
} ArrayMap;
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

/**String**/
typedef struct {
  size_t length;
  size_t size;
  char* c_str;
} String;

void str_create(String* s, int initsize) {
  if(initsize<1)
    initsize = 1;
  s->c_str = NULL;
  s->c_str = (char*)calloc(initsize, sizeof(char));
  if(s->c_str==NULL) {
    printf("ERROR: failed to allocate memory!\n");
    exit(1);
  }                  
  s->c_str[0] = 0;
  s->size = initsize;
  s->length = 0;
}
void str_add_char(String* s, const char ch) {
  size_t length = s->length;
  if(length+1>=s->size) {
    s->size+=s->size/2+2;
    char* newP = NULL;
    newP = (char*)realloc(s->c_str, s->size*sizeof(char));
    if(newP==NULL) {
      log_err("Failed to allocate memory of size %d", (unsigned int)s->size);
    }  
    else {
      //log_info("Sucesfully allocated memory (%d bytes total)", s->size);
      s->c_str = newP;
    }
  }
  s->c_str[length]=ch;
  s->c_str[length+1] = 0;
  s->length++;
}
void str_add_string(String* s, const char* c_str) {
  size_t length = s->length;
  size_t strl = strlen(c_str);
  
  if(length+strl>=s->size) {
    s->size+=strl+1+1;
    char* newP = NULL;
    newP = (char*)realloc(s->c_str, s->size*sizeof(char));
    if(newP==NULL) {
      log_err("Failed to allocate memory of size %d", (unsigned int)s->size);
    }  
    else {
      s->c_str = newP;
    }
  }
  memcpy(s->c_str+length, c_str, strl);
  s->c_str[length+strl+1] = 0;
  s->length+=strl;
}
void str_clear(String* s) {
  //memset(s->c_str, 0, s->length);
  s->length = 0;
  if(s->size>0)
    s->c_str[0] = 0;
}
void str_destroy(String* s) {
  free(s->c_str);
  s->c_str = NULL;
  s->length = s->size = 0;
}

#endif