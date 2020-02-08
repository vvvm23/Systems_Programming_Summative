#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include"gol.h"
int main(int argc, char **argv){
  struct universe v; 

  int torus = 0;
  int stats = 0;
  int c;
  char *iname = NULL;
  char *oname = NULL;
  unsigned int nb_generations;

  while ((c = getopt(argc, argv, "i:o:g:st")) != -1) {
    switch(c) {
      case 'i':
        printf("Reading iname!\n");
        iname = optarg;
        printf("Read! %s\n", iname);
        break;
      case 'o':
        printf("Reading oname!\n");
        oname = optarg;
        printf("Read! %s\n", oname);
        break;
      case 'g':
        printf("Reading nb_gen\n");
        nb_generations = atoi(optarg); // SEGFAULT HERE
        printf("Read! %d\n", nb_generations);
        break;
      case 's':
        printf("Statistics on.\n");
        stats = 1;
        break;
      case 't':
        printf("Torus on.\n");
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
