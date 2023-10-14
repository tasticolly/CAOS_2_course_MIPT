#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ROWS 100
#define MAX_COLS 26

typedef struct {
  char formula[512];
  int type;
} Cell;

Cell table[MAX_ROWS][MAX_COLS];

void evaluate_formula(char* formula, int row, int col) {
  int a = 0;
  char arif[512];
  for (int i = 0; i < strlen(formula); ++i) {
    if (formula[i] >= 'A' && formula[i] <= 'Z') {
      int col_ref = formula[i] - 'A';
      int row_ref = formula[i + 1] - '0' - 1;
      if (table[row_ref][col_ref].type == 1) { // ячейка содержит формулу
        evaluate_formula(table[row_ref][col_ref].formula, row_ref, col_ref);
      }
      strncpy(arif + a, table[row_ref][col_ref].formula, strlen(table[row_ref][col_ref].formula));
      a += strlen(table[row_ref][col_ref].formula);
      ++i;
    } else {
      arif[a] = formula[i];
      ++a;
    }
  }
  arif[a] = '\0';
  snprintf(table[row][col].formula, strlen(arif) + 3, "(%s)", arif);
  table[row][col].type = 0;
}

//1;1;3;4
//=D2;=A1+B1;;=B2*C1

void print_counted_expression(char* expr, int last) {
  char script[512];
  if(last == 0) {
    if(expr[0] == '\0') {
      snprintf(script, 512, "print('', end=';')", expr);
    }
    else snprintf(script, 512, "print(%s, end=';')", expr);
  } else {
    if(expr[0] == '\0') return;
    snprintf(script, 512, "print(%s)", expr);
  }
  PyRun_SimpleString(script);
}

void process_table(int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (table[i][j].type == 1) {
        evaluate_formula(table[i][j].formula, i, j);
      }
      if(j == cols - 1) {
        print_counted_expression(table[i][j].formula, 1);
      } else print_counted_expression(table[i][j].formula, 0);
    }
  }
}

int main() {
  Py_Initialize();
  int rows = 0, cols = 0;
  char input[128];
  while (fgets(input, 128, stdin)) {
    if(rows == 1) {
      rows = 1;
    }
    if(input[0] == 'p') break;
    int a = 0;
    int prev = 0;
    input[strlen(input) - 1] = ';';
    input[strlen(input)] = '\0';
    cols = 0;
    table[rows][cols].type = 0;
    while(input[a] != '\0') {
      if (input[a] == ';') {
        strncpy(table[rows][cols].formula, input + prev, a - prev);
        ++cols;
        table[rows][cols].type = 0;
        ++a;
        prev = a;
        continue;
      }
      if (input[a] == '=') {
        table[rows][cols].type = 1;
        prev = a + 1;
      }
      ++a;
    }
    ++rows;
  }
  process_table(rows, cols);
  Py_Finalize();
  return 0;
}