#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t* result) {
  int err = OK;
  if (rows <= 0 || columns <= 0 || !result) {
    err = INCORRECT;
  } else {
    result->matrix = malloc(sizeof(double*) * rows);
    if (result->matrix) {
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = malloc(sizeof(double) * columns);
        if (!result->matrix[i]) {
          err = CALC_ERR;
          break;
        }
      }
      result->rows = rows;
      result->columns = columns;
    } else {
      err = CALC_ERR;
    }
  }
  return err;
}

void s21_remove_matrix(matrix_t* A) {
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
  A->columns = 0;
  A->rows = 0;
  A->matrix = NULL;
}

int s21_eq_matrix(matrix_t* A, matrix_t* B) {
  int res = check_matrix_size(A, B);
  if (res == SUCCESS) {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
          res = FAILURE;
          break;
        }
  }
  return res;
}

int check_matrix_size(matrix_t* A, matrix_t* B) {
  return ((A->columns != B->columns) || (A->rows != B->rows)) ? FAILURE
                                                              : SUCCESS;
}

int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int err = OK;
  if (!is_matrix_ok(A, B, result)) {
    err = INCORRECT;
  } else {
    if (check_matrix_size(A, B)) {
      s21_create_matrix(A->rows, A->columns, result);
      for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    } else {
      err = CALC_ERR;
    }
  }
  return err;
}

int is_matrix_ok(matrix_t* A, matrix_t* B, matrix_t* result) {
  return !(!A || !B || !result || !A->matrix || !B->matrix);
}

int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int err = OK;
  if (!is_matrix_ok(A, B, result)) {
    err = INCORRECT;
  } else {
    if (check_matrix_size(A, B)) {
      s21_create_matrix(A->rows, A->columns, result);
      for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    } else {
      err = CALC_ERR;
    }
  }
  return err;
}

int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  int err = OK;
  if (!is_matrix_ok(A, A, result) || is_inf_nan(number)) {
    err = INCORRECT;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
  }
  return err;
}

int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int err = OK;
  if (!is_matrix_ok(A, B, result)) {
    err = INCORRECT;
  } else {
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < result->rows; i++)
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = 0;
        for (int m = 0; m < A->columns; m++)
          result->matrix[i][j] += A->matrix[i][m] * B->matrix[m][j];
      }
  }
  return err;
}

int s21_transpose(matrix_t* A, matrix_t* result) {
  int err = OK;
  if (!is_matrix_ok(A, A, result))
    err = INCORRECT;
  else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < result->rows; i++)
      for (int j = 0; j < result->columns; j++)
        result->matrix[i][j] = A->matrix[j][i];
  }
  return err;
}

int s21_calc_complements(matrix_t* A, matrix_t* result) {
  int err = OK;
  if (is_matrix_ok(A, A, result)) {
    if (A->columns == A->rows) {
      err = s21_create_matrix(A->columns, A->rows, result);  // 4
      if (err == OK) {
        if (A->columns == 1) {
          result->matrix[0][0] = A->matrix[0][0];
        } else if (A->columns == 2) {
          result->matrix[0][0] = A->matrix[1][1];
          result->matrix[0][1] = -A->matrix[1][0];
          result->matrix[1][0] = -A->matrix[0][1];
          result->matrix[1][1] = A->matrix[0][0];
        } else {
          for (int i = 0; i < A->columns; i++) {
            for (int j = 0; j < A->columns; j++) {
              result->matrix[i][j] =
                  pow(-1.0, i + j) * s21_det_of_minor(A, i, j);
            }
          }
        }
      }
    } else {
      err = CALC_ERR;
    }
  } else
    err = INCORRECT;
  return err;
}

double s21_det_of_minor(matrix_t* A, int i_, int j_) {
  double out = 0;
  matrix_t B;
  if (!(s21_create_matrix(A->columns - 1, A->columns - 1, &B))) {
    for (int i = 0; i < A->columns; i++) {
      short im = i;
      if (im > i_) im--;
      for (int j = 0; j < A->columns; j++) {
        short jm = j;
        if (jm > j_) jm--;
        if (i != i_ && j != j_) B.matrix[im][jm] = A->matrix[i][j];
      }
    }
    s21_determinant(&B, &out);
    s21_remove_matrix(&B);
  }
  return out;
}

int s21_determinant(matrix_t* A, double* result) {
  int err = OK;
  if (!(!A || !result || !A->matrix)) {
    if (A->columns == A->rows) {
      if (A->columns == 1) {
        *result = A->matrix[0][0];
      } else {
        int ignore_rows = 0;
        bool ignore_columns[64] = {0};
        *result = s21_determinant_recursive(*A, A->columns, ignore_rows,
                                            ignore_columns);
      }
    } else {
      err = CALC_ERR;
    }
  } else
    err = INCORRECT;
  return err;
}

double s21_determinant_recursive(matrix_t A, int n, int ignore_rows,
                                 bool* ignore_columns) {
  double out = 0;
  if (n == 2) {
    double B[2][2] = {0};
    for (int i = ignore_rows, i_ = 0; i < A.columns; i++) {
      for (int j = 0, j_ = 0; j < A.columns; j++) {
        if (ignore_columns[j] == 0) {
          B[i_][j_] = A.matrix[i][j];
          j_++;
        }
      }
      i_++;
    }
    out = B[0][0] * B[1][1] - B[0][1] * B[1][0];
  } else if (n > 2) {
    int sign = 0;
    n--;
    for (int i = ignore_rows; i < A.columns; i++) {
      ignore_rows += 1;
      for (int j = 0; j < A.columns; j++) {
        if (ignore_columns[j] == 0) {
          ignore_columns[j] = 1;
          double out_ =
              pow(-1.0, sign) * A.matrix[i][j] *
              s21_determinant_recursive(A, n, ignore_rows, ignore_columns);
          out += out_;
          ignore_columns[j] = 0;
          sign++;
        }
      }
      break;
    }
  }
  return out;
}

int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  int err = OK;
  double det = 0;
  if (is_matrix_ok(A, A, result)) {
    if (s21_determinant(A, &det) == 0) {
      if (det != 0) {
        if (A->columns == 1) {
          err = s21_create_matrix(1, 1, result);
          result->matrix[0][0] = 1.0 / A->matrix[0][0];
        } else {
          matrix_t alg, mult;
          err = s21_calc_complements(A, &alg);
          err = s21_transpose(&alg, &mult);
          err = s21_mult_number(&mult, 1.0 / det, result);
          s21_remove_matrix(&alg);
          s21_remove_matrix(&mult);
        }
      } else {
        err = CALC_ERR;
      }
    } else {
      err = CALC_ERR;
    }
  } else {
    err = INCORRECT;
  }
  return err;
}

int is_inf_nan(double f) {
  double zero = 0.0;
  int res = 0;
  if (f != f)
    res = 1;
  else if (f >= 1.0 / zero)
    res = 2;
  else if (f <= -1.0 / zero)
    res = 3;
  return res;
}