#include "xor_cipher.h"
#include <string.h>
#include <stdio.h>

void xor_encrypt_decrypt(char *data, const char *key) {
    int keyLength = strlen(key);
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] ^= key[i % keyLength];
    }
}
