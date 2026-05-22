#include <stdio.h>
#include <stdlib.h>

#include "maching.h"

unsigned char **create_blocks(const unsigned char *text, size_t len,
                              size_t numberBlocks) {
    unsigned char **blocks =
        malloc((numberBlocks + 1) * sizeof(unsigned char *));
    if (blocks == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < numberBlocks; i++) {
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
    if (len % SIZE_OF_MACHED == 0) {
        for (size_t i = 0; i < SIZE_OF_MACHED; i++) {
            blocks[len / SIZE_OF_MACHED][i] = i + 1;
        }
    } else {
        for (size_t i = len; i % SIZE_OF_MACHED != 0; i++) {
            blocks[len / SIZE_OF_MACHED][i % SIZE_OF_MACHED] = i - len + 1;
        }
    }
    blocks[numberBlocks] = NULL;
    return blocks;
}

void destroy_blocks(unsigned char **blocks) {
    for (size_t i = 0; blocks[i] != NULL; i++)
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

    size_t numberBlocks = len / SIZE_OF_MACHED + 1;
    unsigned char **blocks = create_blocks(text, len, numberBlocks);
    if (blocks == NULL) {
        free(mached);
        return NULL;
    }

    for (int i = 0; i < HASH_ROUNDS; i++) {
        block_byte_rotate_shift(blocks);
        byte_rotate_left(blocks);
        if (numberBlocks > 1) {
            cross_block_mix(blocks, numberBlocks);
        }
        diffuse_last_byte(blocks);
        accumulate_xor(blocks, mached);
    }

    destroy_blocks(blocks);
    return mached;
}