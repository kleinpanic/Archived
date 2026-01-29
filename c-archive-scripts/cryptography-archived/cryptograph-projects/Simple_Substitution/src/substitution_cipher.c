#include "substitution_cipher.h"
#include <string.h>
#include <ctype.h>  // for isalpha, toupper

// Helper function to find the position of a character in the alphabet
static int find_position(char c) {
    c = toupper(c);
    return c - 'A';
}

// Encrypt the message using the provided key
void substitution_encrypt(char *message, const char *key) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (isalpha(message[i])) {
            int pos = find_position(message[i]);
            //char base = isupper(message[i]) ? 'A' : 'a';
            message[i] = isupper(message[i]) ? key[pos] : tolower(key[pos]);
        }
    }
}

// Decrypt the message using the provided key
void substitution_decrypt(char *message, const char *key) {
    char reverse_key[26];
    for (int i = 0; i < 26; i++) {
        reverse_key[key[i] - 'A'] = 'A' + i;
    }

    for (int i = 0; message[i] != '\0'; i++) {
        if (isalpha(message[i])) {
            int pos = find_position(message[i]);
            //char base = isupper(message[i]) ? 'A' : 'a';
            message[i] = isupper(message[i]) ? reverse_key[pos] : tolower(reverse_key[pos]);
        }
    }
}
