#include <stdio.h>
#include <stdlib.h>

struct universe {
/*Put some appropriate things here*/
  unsigned int **cells;
  unsigned int nb_rows;
  unsigned int nb_columns;
};

/*Do not modify the next seven lines*/
void read_in_file(FILE *infile, struct universe *u);
void write_out_file(FILE *outfile, struct universe *u);
int is_alive(struct universe *u, int column, int row);
int will_be_alive(struct universe *u, int column, int row);
int will_be_alive_torus(struct universe *u,  int column, int row);
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row));
void print_statistics(struct universe *u);
/*You can modify after this line again*/

void read_in_file(FILE *infile, struct universe *u) {
  
}

void write_out_file(FILE *outfile, struct universe *u) {

}

int is_alive(struct universe *u, int column, int row) {
  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null");
    exit(1);
  }

  if (column < 0 || (unsigned int)column >= u->nb_columns) {
    fprintf(stderr, "ERROR: 'column' outside valid range");
    exit(1);
  }

  if (row < 0 || (unsigned int)row >= u->nb_rows) {
    fprintf(stderr, "ERROR: 'row' outside valid range");
    exit(1);
  }

  return *(*(u->cells + row) + column);
}

int will_be_alive(struct universe *u, int column, int row) {
  unsigned int nb_alive = 0;

  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null");
    exit(1);
  }

  if (column < 0 || (unsigned int)column >= u->nb_columns) {
    fprintf(stderr, "ERROR: 'column' outside valid range");
    exit(1);
  }

  if (row < 0 || (unsigned int)row >= u->nb_rows) {
    fprintf(stderr, "ERROR: 'row' outside valid range");
    exit(1);
  }

  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (i == 0 && j == 0) continue;
      if (row+i < 0 || (unsigned)(row+i) >= u->nb_rows) continue;
      if (column+i < 0 || (unsigned)(column+i) >= u->nb_columns) continue;
      nb_alive += is_alive(u, column+i, row+j);
    }
  }

  if (is_alive(u, column, row)) {
    return nb_alive == 2 || nb_alive == 3 ? 1:0;
  }
  return nb_alive == 3 ? 1:0;
}

int will_be_alive_torus(struct universe *u, int column, int row) {
  unsigned int nb_alive = 0;

  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null");
    exit(1);
  }

  if (column < 0 || (unsigned int)column >= u->nb_columns) {
    fprintf(stderr, "ERROR: 'column' outside valid range");
    exit(1);
  }

  if (row < 0 || (unsigned int)row >= u->nb_rows) {
    fprintf(stderr, "ERROR: 'row' outside valid range");
    exit(1);
  }

  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (i == 0 && j == 0) continue;
      
      // When calculating with torus, we consider horizontal, vertical AND diagonal wrapping neighbours
      int t_column = column+i;
      int t_row = row+j;

      t_column = t_column < 0 ? u->nb_columns - 1 : t_column;
      t_column = t_column >= (int)u->nb_columns ? 0 : t_column;

      t_row = t_row < 0 ? u->nb_rows - 1 : t_row;
      t_row = t_row >= (int)u->nb_rows ? 0 : t_row;

      nb_alive += is_alive(u, t_column, t_row);
      
    }
  }
  if (is_alive(u, column, row)) {
    return nb_alive == 2 || nb_alive == 3 ? 1:0;
  }
  return nb_alive == 3 ? 1:0;

}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)) {

}

void print_statistics(struct universe *u) {

}
