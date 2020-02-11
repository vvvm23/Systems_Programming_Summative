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

  for (unsigned int i = 1; (int)i < argc; i++) {
    if (!(argv[i][0] == '-')) {
      fprintf(stderr, "ERROR: Invalid command line argument. ");
      exit(1);
    }

    switch(argv[i][1]) {
      case 'i':
        i++;
        iname = argv[i];
        break;
      case 'o':
        i++;
        oname = argv[i];
        break;
      case 'g':
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

  // TODO: Release all memory! (Should technically do automatically..)

  return 0;
}
