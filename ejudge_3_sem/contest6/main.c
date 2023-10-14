#include <stdio.h>
#include <immintrin.h>

int main() {
  int M;
  scanf("%d", &M);
  int N = M + 1;
  int nice_M = M + 4 - M % 4;

  double mat[M][N], ans[M];
  int nonzero[M];

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      scanf("%lf", &mat[i][j]);
    }
  }

  for (int i = 0; i < M; i++) {
    ans[i] = 0.0;
    nonzero[i] = -1;
  }

  for (int column = 0, row = 0; column < M && row < N; column++) {
    int sel = row;
    for (int i = row; i < N; i++) {
      if (abs(mat[i][column]) > abs(mat[sel][column])) sel = i;
    }
    for (int i = 0; i <= nice_M / 4; i++) {
      __m256d first_row = _mm256_loadu_pd(mat[sel] + 4 * i);
      __m256d second_row = _mm256_loadu_pd(mat[row] + 4 * i);
      _mm256_storeu_pd(mat[sel] + 4 * i, second_row);
      _mm256_storeu_pd(mat[row] + 4 * i, first_row);
    }
    nonzero[column] = row;
    for (int i = 0; i < N; i++) {
      if (i != row) {
        double ratio = mat[i][column] / mat[row][column];
        double ratios[4] = {ratio, ratio, ratio, ratio};
        __m256d ratios_chunk = _mm256_loadu_pd(ratios);
        for (int j = column / 4; j <= nice_M / 4; j += 4) {
          __m256d row_chunk = _mm256_loadu_pd(mat[row] + 4 * j);
          __m256d i_chunk = _mm256_loadu_pd(mat[i] + 4 * j);
          ratios_chunk = _mm256_mul_pd(row_chunk, ratios_chunk);
          i_chunk = _mm256_sub_pd(i_chunk, ratios_chunk);
          _mm256_storeu_pd(mat[i] + 4 * j, i_chunk);
        }
      }
    }
    row++;
  }

  for (int i = 0; i < M; i++) {
    if (nonzero[i] != -1) {
      ans[i] = mat[nonzero[i]][M] / mat[nonzero[i]][i];
    }
  }

  for (int i = 0; i < M; i++) {
    printf("%lf ", ans[i]);
  }
}