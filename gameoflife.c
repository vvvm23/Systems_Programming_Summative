#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"
int main(int argc, char **argv){
  struct universe v; 

  // For checking for duplicate flags
  // Not all needed, but I will keep for consistency
  int flags[5] = {0,0,0,0,0};

  // Default values for flags
  int torus = 0;
  int stats = 0;
  char *iname = NULL;
  char *oname = NULL;
  int nb_generations = 5;

  // Iterate through command line arguments 
  for (unsigned int i = 1; (int)i < argc; i++) {
    // Check if a flag (starts with -)
    if (!(argv[i][0] == '-')) {
      fprintf(stderr, "ERROR: Invalid command line argument.\n");
      exit(1);
    }

    // If any additional charactes after -X then it is invalid
    // to avoid cases such as -sandwiches being accepted
    if (!(argv[i][2] == 0x00)) {
      fprintf(stderr, "ERROR: Invalid flag\n");
      exit(1);
    }

    // Check second character in flag (the operator)
    switch(argv[i][1]) {
      case 'i':
        // Iterate i and get input file name
        i++;
        // Check if the secondary argument exists
        if ((int)i >= argc) {
          fprintf(stderr, "ERROR: Invalid argument option.\n");
          exit(1);
        }

        // If flag is duplicate, check if it conflicts with previous
        if (flags[0] && strcmp(iname, argv[i])) {
          fprintf(stderr, "ERROR: Duplicate flag!\n");
          exit(1);
        }
        flags[0]++;

        iname = argv[i];
        break;
      case 'o':
        // Iterate i and get output file name
        i++;
        // Check if secondary argument exists
        if ((int)i >= argc) {
          fprintf(stderr, "ERROR: Invalid argument option.\n");
          exit(1);
        }

        // If flag is duplicate, check if it conflicts with previous
        if (flags[1] && strcmp(oname, argv[i])) {
          fprintf(stderr, "ERROR: Duplicate flag!\n");
          exit(1);
        }
        flags[1]++;
        oname = argv[i];
        break;
      case 'g':
        i++;
        // Check if secondary argument exists
        if ((int)i >= argc) {
          fprintf(stderr, "ERROR: Invalid argument option.\n");
          exit(1);
        }

        // If flag is duplicate, check if conflicts with previous
        if (flags[2] && nb_generations != atoi(argv[i])) {
          fprintf(stderr, "ERROR: Duplicate flag!\n");
          exit(1);
        }
        flags[2]++;

        // Parse string into integer
        nb_generations = atoi(argv[i]);
        if (nb_generations < 0) {
          fprintf(stderr, "ERROR: Negative number of generations!\n");
          exit(1);
        }
        break;
      case 's':
        // Flags cannot conflict so no need to check flag variable
        flags[3]++;
        stats = 1;
        break;
      case 't':
        // Flags cannot conflict so need to check flag variable
        flags[4]++;
        torus = 1;
        break;
      default:
        fprintf(stderr, "ERROR: Unknown flag!\n");
        exit(1);
    }
  }

  if (iname == NULL) {
    // if no name specified, default to stdin
    read_in_file(stdin, &v);
  } else {
    FILE *fp = fopen(iname, "r"); // Open the file for reading
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open file to read!\n");
      exit(1);
    }
    read_in_file(fp, &v);
    fclose(fp);
  }

  // Call evolve with specified rule (based on torus) nb_generations times
  for (unsigned int i = 0; i < (unsigned int)nb_generations; i++) {
    if (torus) {
      evolve(&v, will_be_alive_torus);
    } else {
      evolve(&v, will_be_alive);
    }
  }

  if (!oname) {
    // if no name specified, default to stdout
    write_out_file(stdout, &v);
  } else {
    FILE *fp = fopen(oname, "w"); // Open the file for writing
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open file to write!\n");
      exit(1);
    }
    write_out_file(fp, &v);
    fclose(fp);
  }

  // Print statistics when finished
  if (stats) {
    print_statistics(&v);
  }

  return 0;
}
