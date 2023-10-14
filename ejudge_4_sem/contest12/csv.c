#include <Python.h>
#include <stdio.h>

#define MAX_ROWS 9
#define MAX_COLUMNS 26
#define TOKEN_SIZE 1024

typedef enum {
  Integer = 0,
  Float = 1,
  String = 2,
  Empty = 3,
  Formula = 4,
} type_t;

typedef struct table_cell {
  char expression[TOKEN_SIZE];
  type_t type;
} table_cell;

table_cell table[MAX_ROWS][MAX_COLUMNS];

void token_handler(char* token, int row, int column) {
  strncpy(table[row][column].expression, token, TOKEN_SIZE);
  if (token[0] == '=') {
    table[row][column].type = Formula;
  } else if (token[0] == '"') {
    table[row][column].type = String;
  } else if (token[0] == '\0') {
    table[row][column].type = Empty;
  } else if (strstr(token, ".") != NULL) {
    table[row][column].type = Float;
  } else {
    table[row][column].type = Integer;
  }
}

void process_token(table_cell* cell) {
  if (cell->type == Formula) {
    char new_expression[TOKEN_SIZE] = {0};
    int read_idx = 1;
    int write_idx = 0;
    while (cell->expression[read_idx] != '\0') {
      if (cell->expression[read_idx] >= 'A' && cell->expression[read_idx] <= 'Z' &&
          cell->expression[read_idx + 1] <= '9' && cell->expression[read_idx + 1] >= '0') {

        int current_row = cell->expression[read_idx + 1] - '1';
        int current_column = cell->expression[read_idx] - 'A';
        if (table[current_row][current_column].type == Formula) {
          process_token(&table[current_row][current_column]);
        }
        if (table[current_row][current_column].type != Empty) {
          cell->type = table[current_row][current_column].type;
        }
        new_expression[write_idx++] = '(';
        int i = 0;
        while (table[current_row][current_column].expression[i] != '\0') {
          new_expression[write_idx++] = table[current_row][current_column].expression[i];
          ++i;
        }
        new_expression[write_idx++] = ')';
        read_idx += 2;
      } else {
        new_expression[write_idx++] = cell->expression[read_idx++];
      }
    }
    new_expression[write_idx++] = '\0';
    strncpy(cell->expression, new_expression, TOKEN_SIZE);
  }
}
void print_cell(table_cell* cell) {
  Py_Initialize();
  char formated_expression[TOKEN_SIZE];
  snprintf(formated_expression, TOKEN_SIZE, "print(%s, end = '')", cell->expression);
  if (cell->type == String) {
    printf("\"");
    fflush(stdout);
    PyRun_SimpleString(formated_expression);
    Py_Finalize();
    printf("\"");
    fflush(stdout);
  } else {
    PyRun_SimpleString(formated_expression);
    Py_Finalize();
  }
}

void process_table(int num_rows, int num_cols) {
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      if (table[i][j].type == Formula) {
        process_token(&table[i][j]);
      }
      if (table[i][j].type != Empty) {
        print_cell(&table[i][j]);
      }
      if (j != num_cols - 1) {
        printf(";");
        fflush(stdout);
      }
    }
    printf("\n");
    fflush(stdout);
  }
}

int main() {

  const size_t buff_size = 4096;
  char* line_buffer = (char*) malloc(buff_size);
  int num_of_rows = 0;
  int num_of_columns = 0;
  while (fgets(line_buffer, buff_size, stdin) != NULL) {
    if (line_buffer[0] == 'g') {
      break;
    }
    int curr_line_idx = 0;
    char current_token[TOKEN_SIZE];
    int i = 0;
    num_of_columns = 0;
    while (curr_line_idx <= strlen(line_buffer)) {
      if (line_buffer[curr_line_idx] == ';' || line_buffer[curr_line_idx] == '\n'
          || line_buffer[curr_line_idx] == '\0') {
        current_token[i++] = '\0';
        i = 0;
        token_handler(current_token, num_of_rows, num_of_columns);
        ++num_of_columns;
        if (line_buffer[curr_line_idx] == '\n' || line_buffer[curr_line_idx] == '\0') {
          break;
        }
      } else {
        current_token[i++] = line_buffer[curr_line_idx];
      }
      ++curr_line_idx;
    }
    ++num_of_rows;
  }

  process_table(num_of_rows, num_of_columns);
  free(line_buffer);
}



