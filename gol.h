#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct universe {
/*Put some appropriate things here*/
  unsigned int **cells;
  unsigned int **next_state;
  unsigned int nb_rows;
  unsigned int nb_columns;
  float average_alive;
  unsigned int nb_steps;
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
#define MAX_COLUMNS 512
