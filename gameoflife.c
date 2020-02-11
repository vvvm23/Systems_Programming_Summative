#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"

int main(int argc, char **argv){
  struct universe v; 

  int torus = 0;
  int stats = 0;
  char *iname = NULL;
  char *oname = NULL;
  unsigned int nb_generations;

  // Iterate through command line arguments 
  for (unsigned int i = 1; (int)i < argc; i++) {
    // Check if a flag (starts with -)
    if (!(argv[i][0] == '-')) {
      fprintf(stderr, "ERROR: Invalid command line argument. ");
      exit(1);
    }

    // Check second character in flag (the operator)
    switch(argv[i][1]) {
      case 'i':
        // Iterate i and get input file name
        i++;
        iname = argv[i];
        break;
      case 'o':
        // Iterate i and get output file name
        i++;
        oname = argv[i];
        break;
      case 'g':
        // TODO: Check if generations is a valid numeric string
        // Iterate i and get number of generations
        i++;
        nb_generations = atoi(argv[i]);
        break;
      case 's':
        stats = 1;
        break;
      case 't':
        torus = 1;
        break;
      default:
        fprintf(stderr, "ERROR: Unknown flag!");
        exit(1);
    }
  }

  nb_generations = nb_generations ? nb_generations : 5;

  if (iname == NULL) {
    read_in_file(stdin, &v);
  } else {
    FILE *fp = fopen(iname, "r");
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open file to read!");
      exit(1);
    }
    read_in_file(fp, &v);
    fclose(fp);
  }

  for (unsigned int i = 0; i < nb_generations; i++) {
    if (torus) {
      evolve(&v, will_be_alive_torus);
    } else {
      evolve(&v, will_be_alive);
    }
  }

  if (!oname) {
    write_out_file(stdout, &v);
  } else {
    
    FILE *fp = fopen(oname, "w"); 
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open file to write!");
      exit(1);
    }
    write_out_file(fp, &v);
    fclose(fp);
  }

  if (stats) {
    print_statistics(&v);
  }

  return 0;
}
