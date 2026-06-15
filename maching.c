#include "maching.h"

#include <stdio.h>

static unsigned char PI_HEX[32] = {
    0x24, 0x3F, 0x6A, 0x88, 0x85, 0xA3, 0x08, 0xD3, 0x13, 0x19, 0x8A,
    0x2E, 0x03, 0x70, 0x73, 0x44, 0xA4, 0x09, 0x38, 0x22, 0x29, 0x9F,
    0x31, 0xD0, 0x08, 0x2E, 0xFA, 0x98, 0xEC, 0x4E, 0x6C, 0x89};

unsigned char **create_blocks(const unsigned char *text, size_t len,
                              size_t numberBlocks) {
  unsigned char **blocks = malloc((numberBlocks + 1) * sizeof(unsigned char *));
  if (blocks == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < numberBlocks - 1; i++) {
    unsigned char *block = malloc(SIZE_OF_MACHED * sizeof(unsigned char));
    blocks[i] = block;
    if (block == NULL) {
      destroy_blocks(blocks);
      return NULL;
    }
  }

  for (size_t i = 0; i < len; i++) {
    blocks[i / SIZE_OF_MACHED][i % SIZE_OF_MACHED] = text[i];
  }
  size_t padding_index = len;
  do {
    blocks[padding_index / SIZE_OF_MACHED][padding_index % SIZE_OF_MACHED] =
        padding_index - len + 1;
    padding_index++;
  } while (padding_index % SIZE_OF_MACHED != 0);
  blocks[numberBlocks - 1] = PI_HEX;
  blocks[numberBlocks] = NULL;
  return blocks;
}

void destroy_blocks(unsigned char **blocks) {
  for (size_t i = 0; blocks[i] != PI_HEX; i++)
    free(blocks[i]);
  free(blocks);
}

void block_byte_rotate_shift(unsigned char **blocks) {
  while (*blocks != NULL) {
    for (size_t i = 0; i < SIZE_OF_MACHED - 1; i++) {
      int shift = ((*blocks)[i + 1] + i) % 8;
      unsigned char c = (*blocks)[i];
      (*blocks)[i] = (c << shift) | (c >> (8 - shift));
    }
    blocks++;
  }
}

void diffuse_last_byte(unsigned char **blocks) {
  while (*blocks != NULL) {
    unsigned char c = (*blocks)[31];
    for (size_t i = 0; i < SIZE_OF_MACHED - 1; i++) {
      (*blocks)[i] = ((*blocks)[i] + c) % 256;
    }
    blocks++;
  }
}

void byte_rotate_left(unsigned char **blocks) {
  while (*blocks != NULL) {
    unsigned char c = (*blocks)[0];
    for (size_t i = 0; i < SIZE_OF_MACHED - 1; i++) {
      (*blocks)[i] = (*blocks)[i + 1];
    }
    (*blocks)[SIZE_OF_MACHED - 1] = c;
    blocks++;
  }
}

void accumulate_xor(unsigned char **blocks, unsigned char *mached) {
  while (*blocks != NULL) {
    for (size_t i = 0; i < SIZE_OF_MACHED; i++) {
      mached[i] ^= (*blocks)[i];
    }
    blocks++;
  }
}

void cross_block_mix(unsigned char **blocks, size_t numberBlocks) {
  unsigned char temp = blocks[0][(numberBlocks - 1) % SIZE_OF_MACHED];
  for (size_t i = 0; i < numberBlocks - 1; i++) {
    blocks[i][i % SIZE_OF_MACHED] ^= blocks[i + 1][i % SIZE_OF_MACHED];
  }
  blocks[numberBlocks - 1][(numberBlocks - 1) % SIZE_OF_MACHED] ^= temp;
}

unsigned char *maching(const unsigned char *text, size_t len) {
  unsigned char *mached = calloc(SIZE_OF_MACHED, sizeof(unsigned char));
  if (mached == NULL) {
    return NULL;
  }

  size_t numberBlocks = len / SIZE_OF_MACHED + 2;
  unsigned char **blocks = create_blocks(text, len, numberBlocks);
  if (blocks == NULL) {
    free(mached);
    return NULL;
  }
  for (int i = 0; i < HASH_ROUNDS; i++) {
    block_byte_rotate_shift(blocks);
    byte_rotate_left(blocks);
    cross_block_mix(blocks, numberBlocks);
    diffuse_last_byte(blocks);
    accumulate_xor(blocks, mached);
  }
  destroy_blocks(blocks);
  return mached;
}
