#include "hmac_sha256.h"
#include <string.h>
#include <stdlib.h>

void hmac_sha256(unsigned char *key, int key_length, unsigned char *data, int data_length, unsigned char *digest)
{
    uint32_t state[8];
    unsigned char k[64];
    unsigned char ipad[64];
    unsigned char opad[64];
    unsigned char temp_hash[32];
    int i;

    if (key_length > 64) {
        sha256_init(state);
        sha256_update(state, key, key_length);
        sha256_final(state, k);
        key_length = 32;
    } else {
        memcpy(k, key, key_length);
    }

    memset(k + key_length, 0, 64 - key_length);

    for (i = 0; i < 64; i++) {
        ipad[i] = k[i] ^ 0x36;
        opad[i] = k[i] ^ 0x5c;
    }

    sha256_init(state);
    sha256_update(state, ipad, 64);
    sha256_update(state, data, data_length);
    sha256_final(state, temp_hash);

    sha256_init(state);
    sha256_update(state, opad, 64);
    sha256_update(state, temp_hash, 32);
    sha256_final(state, digest);
}
