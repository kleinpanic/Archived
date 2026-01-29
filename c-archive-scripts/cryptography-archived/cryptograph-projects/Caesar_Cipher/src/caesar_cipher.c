#include "caesar_cipher.h"
#include <stdio.h>
#include <ctype.h>

void encrypt(char *plaintext, int shift) {
    for (int i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            char base = islower(plaintext[i]) ? 'a' : 'A';
            plaintext[i] = (plaintext[i] - base + shift) % 26 + base;
        }
    }
}

void decrypt(char *ciphertext, int shift) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = islower(ciphertext[i]) ? 'a' : 'A';
            ciphertext[i] = (ciphertext[i] - base - shift + 26) % 26 + base;
        }
    }
}
