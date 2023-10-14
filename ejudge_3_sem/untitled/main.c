#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <immintrin.h>

int main() {
  int n;
  scanf("%d", &n);
  double a[n][n + 4 - n % 4];
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n + 1; ++j) {
      scanf("%lf", &a[i][j]);
    }
  }
  int sel;
  for (int row = 0; row < n; ++row) {
    sel = row;
    for (int i = row; i < n; ++i) {
      if (fabs(a[i][row]) > fabs(a[sel][row])) {
        sel = i;
      }
    }
    for (int j = 0; j < n / 4 + 1; j += 4) {
      __m256d y1 = _mm256_loadu_pd(a[sel] + 4 * j);
      __m256d y2 = _mm256_loadu_pd(a[row] + 4 * j);
      _mm256_storeu_pd(a[sel] + 4 * j, y2);
      _mm256_storeu_pd(a[row] + 4 * j, y1);
    }
    for (int i = 0; i < n; ++i) {
      if (i == row) continue;
      __m256d y = _mm256_set1_pd(a[i][row] / a[row][row]);
      for (int j = row / 4; j < n / 4 + 1; j += 4) {
        __m256d y1 = _mm256_loadu_pd(a[row] + 4 * j);
        y1 = _mm256_mul_pd(y1, y);
        __m256d y2 = _mm256_loadu_pd(a[i] + 4 * j);
        y2 = _mm256_sub_pd(y2, y1);
        _mm256_storeu_pd(a[i] + 4 * j, y2);
      }
    }
  }
  for (int row = 0; row < n; ++row) {
    printf("%lf ", a[row][n] / a[row][row]);
  }
}