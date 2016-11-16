#include "arrays.h"
int numPlaces (int n) {
    if (n < 0) n = -n;//n = (n == MININT) ? MAXINT : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    if (n < 10000000000) return 10;
    /*      2147483647 is 2^31-1 - add more ifs as needed
       and adjust this final return as well. */
    return 11;
}
void array_print2d(Array* array2d) {
   Array* row = NULL;
   int number;
   AR_FOREACH(row, array2d, Array*) {
     AR_FOREACH(number, row, int) {
         printf("%3d ", number);
     }
     printf("\n");
   }
}
int main()
{
   const int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   Array* ar = array_create(sizeof(int), 0);
   // Oh, one thing that sucks is that you can't add literals
   // because you can't provide pointer to them
   array_push(ar, numbers+0);
   array_push(ar, numbers+1);
   array_push(ar, numbers+2);
   array_push(ar, numbers+3);
   array_push(ar, numbers+4);
   
   int number;
   // This is super cool macro
   // Remember this is gcc with -pedantic -Wall  -std=c99
   AR_FOREACH(number, ar, int) {
     printf("Number: %d\n", number);
   }
   
   array_destroy(ar);
   // Multidimensional
   printf("\n---------------------\nMultidimensional test:\n");
   Array* array2d = array_create(sizeof(Array*), 0);
   // Matrix 3x3
   // first fill up
   for(short row = 0; row<3; ++row) {
     Array* tmp = array_create(sizeof(int), 3);
     array_push(array2d, &tmp);
     for(short col = 0; col<3; ++col) {
       array_push(tmp, numbers+row*3+col);
     }
   }
   // The double * here is just plain crazy...
   printf("Final dimensions: %d rows, %d columns.\n", array2d->length, (*(Array**)array_get(array2d, 0))->length);
   // Set some value at X Y
   const size_t x = 1;
   const size_t y = 1;
   const int value = 666;
   array_set(*(Array**)array_get(array2d, y), x, &value);
   
   //system("pause");
   array_print2d(array2d);
   // Set a row to something
   int* num;
   Array* row = *((Array**)array_get(array2d, 2));
   AR_FOREACH_PTR(num, row, int) {
     *num = (int)1024;
   }
   array_print2d(array2d);
   // Finally destroy all sub items and the array itself
   // Note that the foreach loop here helps a lot by already changing Array** to Array*
   AR_FOREACH(row, array2d, Array*) {
     array_destroy(row);
   }
   array_destroy(array2d);
   system("pause");
}

