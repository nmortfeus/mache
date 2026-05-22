#ifndef MACHING_H
#define MACHING_H

#define SIZE_OF_MACHED 32
#define HASH_ROUNDS 10000

unsigned char **create_blocks(const unsigned char *text, size_t len,
                            size_t numberBlocks);
void destroy_blocks(unsigned char **blocks);
void block_byte_rotate_shift(unsigned char **blocks);
void diffuse_last_byte(unsigned char **blocks);
void byte_rotate_left(unsigned char **blocks);
void accumulate_xor(unsigned char **blocks, unsigned char *mached);
void cross_block_mix(unsigned char **blocks, size_t numberBlocks);

unsigned char* maching(const unsigned char* text, size_t len);

#endif /* ! MACHING_H */
