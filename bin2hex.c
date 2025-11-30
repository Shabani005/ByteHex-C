#define NB_IMPLEMENTATION
#include "nb.h"

int main(int argc, char **argv){
  if (argc < 2 || argc >= 3){
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }
  
  if (!nb_does_file_exist(argv[1])){
    fprintf(stderr, "File: '%s' does not exist\n", argv[1]);
    return 1;
  }

  printf("%s\n", nb_hexdump(argv[1]));
  
  return 0;
}

