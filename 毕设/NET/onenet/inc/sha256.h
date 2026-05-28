#ifndef _SHA256_H_
#define _SHA256_H_

#include <stdint.h>

#define SHA256_BLOCK_SIZE 32
#define SHA256_DIGEST_SIZE 32

void sha256_init(uint32_t *state);
void sha256_update(uint32_t *state, const uint8_t *data, uint32_t len);
void sha256_final(uint32_t *state, uint8_t *digest);

#endif
