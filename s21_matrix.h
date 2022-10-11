#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define SUCCESS 1
#define FAILURE 0

#define OK 0
#define INCORRECT 1
#define CALC_ERR 2


typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

int check_matrix_size(matrix_t* A, matrix_t* B);
int is_inf_nan(double f);
int is_matrix_ok(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_minor_matrix(matrix_t *A, int i, int j, matrix_t *result);
double s21_det_of_minor(matrix_t *A, int i_, int j_);
double s21_determinant_recursive(matrix_t A, int n, int ignore_rows, bool *ignore_columns);

#endif //  SRC_S21_MATRIX_H_