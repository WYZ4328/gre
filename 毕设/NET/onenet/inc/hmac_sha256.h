#ifndef _HMAC_SHA256_H_
#define _HMAC_SHA256_H_

#include "sha256.h"

void hmac_sha256(
    unsigned char *key,
    int key_length,
    unsigned char *data,
    int data_length,
    unsigned char *digest
);

#endif
