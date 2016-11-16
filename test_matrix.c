#include "matrix.h"

int main()
{
   const int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   MatrixMgr* mgr = mmgr_create();
   
   Matrix* A = mmgr_add(mgr, matrix_create_fixed(2, 2));
   
   printf("Rows: %d\n", (int)A->rows->length);
   printf("Cols: %d\n", (int)(matrix_getrow(A, 0)->length));
   
   printf("A(0,0) = %ld\n", matrix_getXY(A, 0,0));
   printf("A(0,0) := 5\n");
   matrix_setXY(A, 0,0,5);
   matrix_setXY(A, 0,1,3);
   printf("A(0,0) = %ld\n", matrix_getXY(A, 0,0));
   
   matrix_print(A, stdout);
   // add A to A:
   printf("A + A = \n");
   Matrix* A2 = matrix_add(A, A);
   matrix_print(A2, stdout);
   
   Matrix* B = matrix_create_va(2, 3,
     1, 2, 3,
     4, 5, 6);
   printf("B = \n");
   matrix_print(B, stdout);
   Matrix* C = matrix_create_va(3, 2,
     7,   8,
     9,  10,
     11, 12);
   printf("C = \n");
   matrix_print(C, stdout);
   Matrix* BxC = mmgr_add(mgr, matrix_multiply_matrices(B, C));
   printf("B * C = \n");
   matrix_print(BxC, stdout);
   
   //matrix_destroy(A);
   matrix_destroy(B);
   matrix_destroy(A2);
   matrix_destroy(C);
   //matrix_destroy(BxC);
   mmgr_destroy(mgr);
   system("pause");
}

