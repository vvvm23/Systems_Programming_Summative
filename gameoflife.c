#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"

// TODO: can I use **argv? instead of *argv[]
int main(int argc, char **argv){
  struct universe v; 

  int torus = 0;
  int stats = 0;
  char *iname = NULL;
  char *oname = NULL;
  unsigned int nb_generations = 5;

  // Iterate through command line arguments 
  for (unsigned int i = 1; (int)i < argc; i++) {
    // Check if a flag (starts with -)
    if (!(argv[i][0] == '-')) {
      fprintf(stderr, "ERROR: Invalid command line argument.\n");
      exit(1);
    }

    // Check second character in flag (the operator)
    switch(argv[i][1]) {
      case 'i':
        // Iterate i and get input file name
        i++;
        if ((int)i >= argc) {
          fprintf(stderr, "ERROR: Invalid argument option.\n");
          exit(1);
        }
        iname = argv[i];
        break;
      case 'o':
        // Iterate i and get output file name
        i++;
        if ((int)i >= argc) {
          fprintf(stderr, "ERROR: Invalid argument option.\n");
          exit(1);
        }
        oname = argv[i];
        break;
      case 'g':
        i++;
        if ((int)i >= argc) {
          fprintf(stderr, "ERROR: Invalid argument option.\n");
          exit(1);
        }

        nb_generations = atoi(argv[i]);
        break;
      case 's':
        stats = 1;
        break;
      case 't':
        torus = 1;
        break;
      default:
        fprintf(stderr, "ERROR: Unknown flag!\n");
        exit(1);
    }
  }

  if (iname == NULL) {
    read_in_file(stdin, &v);
  } else {
    FILE *fp = fopen(iname, "r");
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open file to read!\n");
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
      fprintf(stderr, "ERROR: Failed to open file to write!\n");
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
