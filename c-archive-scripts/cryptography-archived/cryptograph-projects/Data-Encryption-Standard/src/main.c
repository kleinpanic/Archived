#include "des.h"
#include <stdio.h>

int main() {
    unsigned long long message;
    unsigned long long key;

    // Ask for a message and key (both as integers for simplicity)
    printf("Enter a message to encrypt (as a 64-bit integer): ");
    scanf("%llu", &message);

    printf("Enter a key (as a 64-bit integer): ");
    scanf("%llu", &key);

    // Encrypt the message
    des_encrypt(&message, key);
    printf("Encrypted message: %llu\n", message);

    // Decrypt the message
    des_decrypt(&message, key);
    printf("Decrypted message: %llu\n", message);

    return 0;
}

