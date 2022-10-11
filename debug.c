#include "debug.h"

#include "s21_matrix.h"

void print_matrix(matrix_t* A) {
  printf("MATRIX[%d][%d] = {\n", A->columns, A->rows);
  for (int i = 0; i < A->columns; i++) {
    printf("{");
    for (int j = 0; j < A->rows; j++) {
      printf(" %.2lf ", A->matrix[i][j]);
    }
    printf("}\n");
  }
}

void init_matrix_by_number(double number, matrix_t* A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->matrix[i][j] = number;
      number += 1.0;
    }
  }
}

int s21_is_matrix_ok(matrix_t* A) {
  int error = OK;
  if (A) {            // NULL?
    if (A->matrix) {  // NULL?
      // проверка на NAN INF в ячейках матрицы
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          if (is_inf_nan(A->matrix[i][j]) != 0) {
            error = 2;
            break;
          }
        }
        if (error != 0) break;
      }
    } else {
      error = 1;
    }
  } else {
    error = 1;
  }
  return error;
}

int s21_is_both_ok(matrix_t* A, matrix_t* B) {
  int error = s21_is_matrix_ok(A);
  if (error == 0) {
    error = s21_is_matrix_ok(B);
  }
  return error;
}