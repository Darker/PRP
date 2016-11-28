#ifndef MATRIX_OPERATION_H
#define MATRIX_OPERATION_H
#include "matrix.h"
/**
  This struct has a special state:

  IF AND ONLY IF type=='\0' AND B==NULL
    then A is actually Matrix* and evaluating this operation
    creates single matrix.
**/
typedef struct MatrixOperation MatrixOperation;
struct MatrixOperation{
    // '+' '-' '*' '\0'
    char type;
    MatrixOperation* A;
    MatrixOperation* B;
};

MatrixOperation* matop_create_end_node(Matrix* value);
MatrixOperation* matop_create(const char type, MatrixOperation*A, MatrixOperation*B) {
    MatrixOperation* op = NEW(MatrixOperation);
    op->A = A;
    op->B = B;
    op->type = type;
    return op;
}
MatrixOperation* matop_create_simple(const char type, Matrix*A, Matrix*B) {
    MatrixOperation* op = NEW(MatrixOperation);
    MatrixOperation* endA = matop_create_end_node(A);
    MatrixOperation* endB = matop_create_end_node(B);
    op->A = endA;
    op->B = endB;
    op->type = type;
    return op;
}
MatrixOperation* matop_create_end_node(Matrix* value) {
    MatrixOperation* op = NEW(MatrixOperation);
    op->A = (MatrixOperation*)value;
    op->B = NULL;
    op->type = '\0';
    return op;
}
bool matop_is_binary(const MatrixOperation* const op) {
    return op->type != '\0';
}

// Manager parameter is used for new matrices
Matrix* matop_eval(MatrixOperation* op, MatrixMgr* mgr) {
    if(op->type=='\0') {
        return (Matrix*)op->A;
    }
    else {
        Matrix* result = NULL;
        Matrix* A = matop_eval(op->A, mgr);
        Matrix* B = matop_eval(op->B, mgr);
        if(A==NULL || B==NULL)
            return NULL;
        switch(op->type) {
            case '+': {
                result = matrix_add(A, B);
                break;
            }
            case '-': {
                result = matrix_subtract(A, B);
                break;
            }
            case '*': {
                result = matrix_multiply_matrices(A, B);
                break;
            }
            default: {
                chyba_debug("Invalid operation %c.", 88, op->type);
            }
        }
        if(mgr != NULL)
            mmgr_add(mgr, result);
        return result;
    }
}
MatrixOperation* matop_destroy(MatrixOperation* op, bool freeMatrices) {
    if(op==NULL) {
        return NULL;
    }
    else if(op->type=='\0') {
        if(freeMatrices)
            matrix_destroy((Matrix*)op->A);
    }
    else {
        matop_destroy(op->A, freeMatrices);
        matop_destroy(op->B, freeMatrices);
    }
    free(op);
    return NULL;
}

#endif // MATRIX_OPERATION_H
