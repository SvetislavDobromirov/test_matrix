#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

#include "s21_matrix.h"

void print_matrix(matrix_t* A);
void init_matrix_by_number(double number, matrix_t *A);
int s21_is_both_ok(matrix_t *A, matrix_t *B);
int s21_is_matrix_ok(matrix_t *A);

#endif //  SRC_DEBUG_H_