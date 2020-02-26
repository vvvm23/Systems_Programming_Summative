#include "gol.h"

void read_in_file(FILE *infile, struct universe *u) {
  if (!infile) {
    fprintf(stderr, "ERROR: 'infile' is null!\n");
    exit(1);
  }
  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null!\n");
    exit(1);
  }

  unsigned int nb_rows = 0;
  unsigned int nb_columns = 0;

  char *buf = (char*) malloc(MAX_COLUMNS*sizeof(char));

  if (!buf) {
    fprintf(stderr, "ERROR: Failed to allocate memory to 'buf'!\n");
    exit(1);
  }

  fgets(buf, MAX_COLUMNS, infile); // Check this.

  nb_columns = strlen(buf) - 1;
  if (nb_columns < 1) {
    fprintf(stderr, "ERROR: Input file had no columns!\n");
    exit(1);
  }

  /* TODO: Allow for 512 columns!! <26-02-20, alex> */
  /*if (! (*(buf + nb_columns) == '\n')) {*/
    /*fprintf(stderr, "ERROR: Input file had columns greater than buffer!\n");*/
    /*exit(1);*/
  /*}*/

  *(buf + nb_columns) = '\0';
  nb_rows = 1;

  u->cells = (unsigned int**)malloc(sizeof(int*));

  if (!u->cells) {
    fprintf(stderr, "ERROR: Failed to allocate memory for row in 'universe'!\n");
    exit(1);
  }

  *(u->cells) = (unsigned int*)malloc(sizeof(int)*nb_columns);

  if (!*(u->cells)) {
    fprintf(stderr, "ERROR: Failed to allocate memory to cells in row in 'universe'!\n");
    exit(1);
  }

  for (unsigned int i = 0; i < nb_columns; i++) {
      char c_char = *(buf+i);
      if (! (c_char == '*' || c_char == '.')) {
        fprintf(stderr, "Invalid symbol in input file!\n");
        exit(1);
      }
    *(*(u->cells) + i) = c_char == '*' ? 1 : 0;
  }

  nb_rows = 1;

  while (fgets(buf, MAX_COLUMNS, infile)) {

    unsigned int count = strlen(buf) - 1;

    /*if (!(*(buf + count) == '\n')) {*/
      /*fprintf(stderr, "ERROR: Input file had columns greater than buffer!\n");*/
      /*exit(1);*/
    /*}*/

    *(buf + count) = '\0';

    if (count != nb_columns) {
      fprintf(stderr, "ERROR: Malformed input file!\n");
      exit(1);
    }
    nb_rows++;
    u->cells = (unsigned int**)realloc(u->cells, sizeof(int*)*nb_rows);

    if (!u->cells) {
      fprintf(stderr, "ERROR: Failed to allocate memory for row in 'universe'!\n");
      exit(1);
    }

    *(u->cells + nb_rows - 1) = (unsigned int*)malloc(sizeof(int)*nb_columns);
      
    if (!*(u->cells + nb_rows - 1)) {
      fprintf(stderr, "ERROR: Failed to allocate memory to cells in row in 'universe'!\n");
      exit(1);
    }

    for (unsigned int i = 0; i < nb_columns; i++) {
      char c_char = *(buf+i);
      if (! (c_char == '*' || c_char == '.')) {
        fprintf(stderr, "Invalid symbol in input file!");
        exit(1);
      }
      *(*(u->cells + nb_rows - 1) + i) = c_char == '*' ? 1 : 0;
    }
  }
  u->nb_rows = nb_rows;
  u->nb_columns = nb_columns;
  u->nb_steps = 1;

  // Initialise next state
  u->next_state = (unsigned int**)malloc(sizeof(unsigned int*)*nb_rows);
  if (!u->next_state) {
    fprintf(stderr, "Failed to allocate memory to 'next_state!'\n");
  }

  for (unsigned int i = 0; i < nb_rows; i++) {
    *(u->next_state + i) = (unsigned int*)malloc(sizeof(unsigned int)*nb_columns);
    if (!*(u->next_state + i)) {
      fprintf(stderr, "Failed to allocate memory to row in 'next_state'!\n"); 
    }
  }

  // Update average
  unsigned int nb_alive = 0;
  unsigned int nb_total = u->nb_rows * u->nb_columns;

  for (unsigned int i = 0; i < u->nb_rows; i++) {
    for (unsigned int j = 0; j < u->nb_columns; j++) {
      nb_alive += is_alive(u, j, i);
    }
  }
  u->average_alive = (float)nb_alive / (float)nb_total;
  free(buf); // Free buffer memory
}

// Write out current universe to file
void write_out_file(FILE *outfile, struct universe *u) {
  if (!outfile) {
    fprintf(stderr, "ERROR: 'outfile' is null!\n");
    exit(1);
  }
  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null!\n");
    exit(1);
  }

  if (!u->cells) {
    fprintf(stderr, "ERROR: 'cells' is undefined!\n");
    exit(1);
  }

  for (unsigned int i = 0; i < u->nb_rows; i++) {
    for (unsigned int j = 0; j < u->nb_columns; j++) {
      fputc(*(*(u->cells + i) + j) ? 0x2A : 0x2E, outfile); // Write either * or .
    }
    fputc(0x0A, outfile); // Add newline character
  }
}

// Checks if a given cell is alive
int is_alive(struct universe *u, int column, int row) {
  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null!\n");
    exit(1);
  }

  if (!u->cells) {
    fprintf(stderr, "ERROR: 'cells' is undefined!\n");
    exit(1);
  }

  if (column < 0 || (unsigned int)column >= u->nb_columns) {
    fprintf(stderr, "ERROR: 'column' outside valid range!\n");
    exit(1);
  }

  if (row < 0 || (unsigned int)row >= u->nb_rows) {
    fprintf(stderr, "ERROR: 'row' outside valid range!\n");
    exit(1);
  }

  return *(*(u->cells + row) + column); // Dereference along rows, then columns and return cell value
}

// Rule function to check if given cell will be alive next time
int will_be_alive(struct universe *u, int column, int row) {
  unsigned int nb_alive = 0;
  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null!\n");
    exit(1);
  }

  if (!u->cells) {
    fprintf(stderr, "ERROR: 'cells' is undefined!\n");
    exit(1);
  }

  if (column < 0 || (unsigned int)column >= u->nb_columns) {
    fprintf(stderr, "ERROR: 'column' outside valid range!\n");
    exit(1);
  }

  if (row < 0 || (unsigned int)row >= u->nb_rows) {
    fprintf(stderr, "ERROR: 'row' outside valid range!\n");
    exit(1);
  }

  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Ignore centre cell when counting
      if (i == 0 && j == 0) continue;

      // Ignore if extends out of universe
      if (row+i < 0 || (unsigned)(row+i) >= u->nb_rows) continue;
      if (column+j < 0 || (unsigned)(column+j) >= u->nb_columns) continue;
      nb_alive += is_alive(u, column+j, row+i); // Increment by cell value
    }
  }

  // Evaluate rules depending on nb_alive and cell state
  if (is_alive(u, column, row)) {
    return nb_alive == 2 || nb_alive == 3 ? 1:0;
  }
  return nb_alive == 3 ? 1:0;
}

// Rule function to check if given cell will be alive next time (assuming torus universe)
int will_be_alive_torus(struct universe *u, int column, int row) {
  unsigned int nb_alive = 0;

  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null!\n");
    exit(1);
  }

  if (!u->cells) {
    fprintf(stderr, "ERROR: 'cells' is undefined!\n");
    exit(1);
  }

  if (column < 0 || (unsigned int)column >= u->nb_columns) {
    fprintf(stderr, "ERROR: 'column' outside valid range!\n");
    exit(1);
  }

  if (row < 0 || (unsigned int)row >= u->nb_rows) {
    fprintf(stderr, "ERROR: 'row' outside valid range!\n");
    exit(1);
  }

  for (int i = -1; i < 2; i++) { 
    for (int j = -1; j < 2; j++) {
      if (i == 0 && j == 0) continue; // Again, skip centre cell
      // When calculating with torus, we consider horizontal, vertical AND diagonal wrapping neighbours
      int t_column = column+j;
      int t_row = row+i;

      t_column = t_column < 0 ? (int)u->nb_columns - 1 : t_column;
      t_column = t_column >= (int)u->nb_columns ? 0 : t_column;

      t_row = t_row < 0 ? (int)u->nb_rows - 1 : t_row;
      t_row = t_row >= (int)u->nb_rows ? 0 : t_row;

      nb_alive += is_alive(u, t_column, t_row); // Increment by cell value
    }
  }

  // Evaluate rules depending on nb_alive and cell state
  if (is_alive(u, column, row)) {
    return nb_alive == 2 || nb_alive == 3 ? 1:0;
  }
  return nb_alive == 3 ? 1:0;
}

// Update state (generate next_state, then update pointers)
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)) {
  if (!u) {
    fprintf(stderr, "ERROR: 'universe is null'!\n");
    exit(1);
  } 

  if (!rule) {
    fprintf(stderr, "ERROR: 'rule' is null!\n");
    exit(1);
  }

  if (!u->cells) {
    fprintf(stderr, "ERROR: 'cells' is undefined!\n");
    exit(1);
  }

  if (!u->next_state) {
    fprintf(stderr, "ERROR: 'next_state' is undefined!\n");
    exit(1);
  }

  for (unsigned int i = 0; i < u->nb_rows; i++) {
    for (unsigned int j = 0; j < u->nb_columns; j++) {
      *(*(u->next_state + i) + j) = rule(u, j, i) ;
    }
  }

  // Swap next_state and cells pointers.
  unsigned int **temp = u->cells;
  u->cells = u->next_state;
  u->next_state = temp;

  // Update average
  unsigned int nb_alive = 0;
  unsigned int nb_total = u->nb_rows * u->nb_columns;

  for (unsigned int i = 0; i < u->nb_rows; i++) {
    for (unsigned int j = 0; j < u->nb_columns; j++) {
      nb_alive += is_alive(u, j, i);
    }
  }
  float new_average = (float)nb_alive / (float)nb_total;

  // Update running average
  u->nb_steps += 1;
  u->average_alive += (new_average / u->nb_steps) - (u->average_alive / u->nb_steps); 
}

/* TODO: counts first generation? 0 <25-02-20, alex> */
// Print statistics about current and previous generations
void print_statistics(struct universe *u) {
  if (!u) {
    fprintf(stderr, "ERROR: 'universe' is null!\n");
    exit(1);
  }

  if (!u->cells) {
    fprintf(stderr, "ERROR: 'cells' is undefined!\n");
    exit(1);
  }

  unsigned int nb_alive = 0;
  unsigned int nb_total = u->nb_rows * u->nb_columns;

  for (unsigned int i = 0; i < u->nb_rows; i++) {
    for (unsigned int j = 0; j < u->nb_columns; j++) {
      nb_alive += is_alive(u, j, i);
    }
  }
  float new_average = (float)nb_alive / (float)nb_total;

  // Print statistics to stdout
  fprintf(stdout, "%.3f%% of cells currently alive\n", new_average*100.0);
  fprintf(stdout, "%.3f%% of cells alive on average\n", u->average_alive*100.0);
}

