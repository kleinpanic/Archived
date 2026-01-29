#include "vigenere_cipher.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encrypt(char *plaintext, const char *key) {
    int keyLen = strlen(key);
    for (int i = 0, j = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            char base = islower(plaintext[i]) ? 'a' : 'A';
            int shift = (tolower(key[j % keyLen]) - 'a');
            plaintext[i] = (plaintext[i] - base + shift) % 26 + base;
            j++;
        }
    }
}

void decrypt(char *ciphertext, const char *key) {
    int keyLen = strlen(key);
    for (int i = 0, j = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = islower(ciphertext[i]) ? 'a' : 'A';
            int shift = (tolower(key[j % keyLen]) - 'a');
            ciphertext[i] = (ciphertext[i] - base - shift + 26) % 26 + base;
            j++;
        }
    }
}
