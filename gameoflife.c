#include<stdio.h>
#include<stdlib.h>
#include"gol.h"
int main(int argc, char **argv){
  struct universe v; 

  int torus:1 = 0;
  int stats:1 = 0;
  int c;
  char *iname;
  char *oname;
  unsigned int nb_generations;

  while ((c = getopt(argc, argv, "iogst")) != -1) {
    switch(c) {
      case 'i':
        iname = optarg;
        break;
      case 'o':
        oname = optarg;
        break;
      case 'g':
        nb_generations = atoi(optarg);
        break;
      case 's':
        stats = 1;
        break;
      case 't':
        torus = 1;
        break;
      case ':':
        fprintf(stderr, "Missing required value! Exiting..\n");
        exit(1);
      case '?':
        fprintf(stderr, "Unknown option: '%c' Exiting..\n", optopt);
        exit(1);
    }
  }

  nb_generations = nb_generations ? nb_generations : 5;


  if (!iname) {
    read_in_file(stdin, &v);
  } else {
    FILE *fp = fopen(iname, 'r');
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open file!");
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
    fp = fopen(oname, 'w'); 
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open file!");
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
