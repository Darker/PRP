#ifndef MATRIX_J_MAREDA
#define MATRIX_J_MAREDA
// My old crazy lib. Let's pray it works
#include "arrays.h"
#include "hw_templates.h"
#include "types.h"
#include <stdarg.h>
int numPlaces (long n) {
    if (n < 0) n = -n;//n = (n == MININT) ? MAXINT : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    else {
      return numPlaces(n/1000000) + 6;
    }
}
/**
 * This is dynamic matrix class. Dynamic here means it will change size as you
 * add numbers to it. Empty columns will be filled with zeroes.
 * **/  
typedef struct {
  // Array of arrays, man shit is gettin' serious
  // I had to think like 5 minutes just to figure out how to write it properly
  // Turns out it's not possible
  // this array will be initialized to contain other arrays
  
  /* private! */ Array* rows;
  // these are for convenience
  // still not sure if I'll actually fill them
  size_t width;
  size_t height;
  bool fixed_length;
} Matrix;

void matrix_setXY(Matrix* matrix, const size_t x, const size_t y, const long value);
Matrix* matrix_add_multiplier(const Matrix* A, const Matrix* B, const long m_A, const long m_B);

Matrix* matrix_create() {
  Matrix* m = NEW(Matrix);
  // creates the outer shell to contain rows
  // every row is array of longs
  m->rows = array_create(sizeof(Array*), 0);
  m->width = 0;
  m->height = 0;
  m->fixed_length = false;
  return m;
}
Matrix* matrix_create_fixed(size_t rows, size_t cols) {
  Matrix* m = matrix_create();
  array_reserve(m->rows, rows);
  m->height = rows;
  // Add 'cols'
  for(int i=0; i<rows; ++i) {
    Array* tmp = array_create(sizeof(long), cols);
    array_push(m->rows, &tmp);
    // Create the columns and set them to zero
    const long zero = 0;
    for(int c=0; c<cols; ++c) {
      array_push(tmp, &zero);
    }
    //AR_FOREACH_PTR(item, tmp, long) {
    //   *item = 0;
    //}
  }
  m->width = cols;
  m->fixed_length = true;
  return m;
}
/** Varargs method to create matrix. Mess up the dimensions and you're fucked! **/
Matrix* matrix_create_va( size_t rows, size_t cols, ... ) {
    Matrix* result = matrix_create_fixed(rows, cols);
    //log_info("Making %d by %d matrix, max indexes [%d, %d].", rows, cols, rows-1, cols-1);
    va_list arguments;
    /* Initializing arguments to store all values after num */
    va_start ( arguments, cols );           
    /* we still rely on the function caller to tell us how
     * many there are */
    for(int row=0; row<rows; ++row) {
      for(int col=0; col<cols; ++col) {
        //log_info(" - Setting [%d, %d].", row, col);
        matrix_setXY(result, row, col, va_arg ( arguments, long ));
      }
    }
    va_end ( arguments );                  // Cleans up the list

    return result;
}

Array* matrix_getrow(const Matrix* matrix, const size_t rowIndex) {
  return *((Array**)array_get(matrix->rows, rowIndex));
}
/** Y - column
 *  X - row **/
void matrix_setXY(Matrix* matrix, const size_t x, const size_t y, const long value) {
  if(x>=matrix->height || y>=matrix->width) {
    chyba("Tried to set index [%d, %d] which is out of bounds in matrix.", 78, y, x);
  }
  array_set(matrix_getrow(matrix, x), y, &value);
}
long matrix_getXY(const Matrix* matrix, const size_t x, const size_t y) {
  if(x>=matrix->height || y>=matrix->width) {
    chyba("Tried to get index [%d, %d] which is out of bounds in matrix.", 75, y, x);
  }
  return *((long*)array_get(matrix_getrow(matrix, x), y));
}
void matrix_print(const Matrix* matrix, FILE* stream) {
  Array* row;
  long number;
  //bool firstRow = true;
  AR_FOREACH(row, matrix->rows, Array*) {
    //fprintf(stream, "(%2d entries)", row->length);
    fprintf(stream, "|| ");
    AR_FOREACH(number, row, long) {
      fprintf(stream, "%3ld ", number);
    }
    fprintf(stream, "||\n");
  }
}
void matrix_print_prp_1(const Matrix* matrix, FILE* stream) {
  Array* row;
  long number;
  fprintf(stream, "%d %d", (int)matrix->height, (int)matrix->width);
  AR_FOREACH(row, matrix->rows, Array*) {
    AR_FOREACH(number, row, long) {
      fprintf(stream, "%ld ", number);
    }
    fprintf(stream, "\n");
  }
}
Matrix* matrix_clone(Matrix* original) {
  Matrix* result = matrix_create_fixed(original->height, original->width);
  result->fixed_length = original->fixed_length;
  // Copy all items
  for(int row=0; row<original->height; ++row) {
    for(int col=0; col<original->height; ++col) {
      matrix_setXY(result, row, col, matrix_getXY(original, row, col));
    }
  }
  return result;
}
// Adds two matrices together
// MUST DELETE NEW AND OLD MATRICES EVENTUALY!!!
Matrix* matrix_add(const Matrix* A, const Matrix* B) {
  return matrix_add_multiplier(A, B, 1, 1);
}
Matrix* matrix_subtract(const Matrix* A, const Matrix* B) {
  return matrix_add_multiplier(A, B, -1, 1);
}
// Adds two matrices but also multiplies their elements by given multipliers
// best used for subtraction where one multiplier is -1
Matrix* matrix_add_multiplier(const Matrix* A, const Matrix* B, const long m_A, const long m_B) {
  if(A->width!=B->width || A->height!=B->height) {
    chyba("Adding matrices of different size!", 18);
    return NULL;
  }
  Matrix* res = matrix_create_fixed(A->height, A->width);
  for(int row=0; row<A->height; ++row) {
    for(int col=0; col<A->width; ++col) {
      matrix_setXY(res, row, col, m_A*matrix_getXY(A, row, col)+m_B*matrix_getXY(B, row, col));
    }
  }
  return res;
}
Matrix* matrix_multiply_scalar(const Matrix* A, const long scalar) {
  Matrix* res = matrix_create_fixed(A->height, A->width);
  for(int row=0; row<A->height; ++row) {
    for(int col=0; col<A->width; ++col) {
      matrix_setXY(res, row, col, scalar*matrix_getXY(A, row, col));
    }
  }
  return res;
}
Matrix* matrix_multiply_matrices(const Matrix* A, const Matrix* B) {
  if(A->width!=B->height || A->height!=B->width) {
    chyba("Multiplying mattrices of invalid dimensions, (%d by %d)x(%d by %d)!", 19,
      A->width, A->height, B->height, B->width
    );
    return NULL;
  }
  Matrix* res = matrix_create_fixed(A->height, B->width);
  // first loop by A's rows (which are as long as B's columns)
  for(int arow=0; arow<A->height; ++arow) {
    // For every A's row, go through B's columns
    for(int bcol=0; bcol<B->width; ++bcol) {
      // And every nth item in A's row corresponds with B's col item
      int sum = 0;
      for(int item=0; item<A->width; ++item) {
        sum += matrix_getXY(A, arow, item)*matrix_getXY(B, item, bcol);
      }
      matrix_setXY(res, arow, bcol, sum);
    }
  }
  return res;
}
void matrix_destroy(Matrix* matrix) {
  Array* row;
  AR_FOREACH(row, matrix->rows, Array*) {
     array_destroy(row);
  }
  array_destroy(matrix->rows);
  free(matrix);
}

// Simple struct to destroy all matrices
typedef struct {
    Array* matrices;
} MatrixMgr;

MatrixMgr* mmgr_create() {
  MatrixMgr* mgr = NEW(MatrixMgr);
  mgr->matrices = array_create(sizeof(Matrix*), 0);
  return mgr;
}
// Conveniently returns Matrix* to allow chaining... sort of
Matrix* mmgr_add(MatrixMgr* mgr, Matrix* matrix) {
  array_push(mgr->matrices, &matrix);
  return matrix;
}
/* Destroys the manager and all contained matrices **/
void mmgr_destroy(MatrixMgr* mgr) {
  Matrix* matrix;
  AR_FOREACH(matrix, mgr->matrices, Matrix*) {
     matrix_destroy(matrix);
  }
  array_destroy(mgr->matrices);
  free(mgr);
}

#endif