#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maching.h"

char *hex_converter(unsigned char *mached) {
  size_t len = SIZE_OF_MACHED;
  char *hex_mached = malloc(len * 2 + 1);
  if (!hex_mached) {
    return NULL;
  }
  for (int i = 0; i < SIZE_OF_MACHED; i++) {
    sprintf(hex_mached + 2 * i, "%02x", mached[i]);
  }
  return hex_mached;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usage: %s <text>\n", argv[0]);
    return 1;
  }

  const unsigned char *input = (const unsigned char *)argv[1];

  unsigned char *mached = maching(input, strlen(argv[1]));
  if (mached == NULL) {
    fprintf(stderr, "Error: Something went wrong\n");
    return 1;
  }

  char *mached_hex = hex_converter(mached);
  printf("mached: %s\n", mached_hex);

  free(mached);
  free(mached_hex);
  return 0;
}
