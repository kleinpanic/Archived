#include "caesar_cipher.h"
#include <stdio.h>

int main() {
    char message[256];
    int shift;

    printf("Enter a message to encrypt: ");
    fgets(message, sizeof(message), stdin);

    printf("Enter shift amount (1-25): ");
    scanf("%d", &shift);

    // Encrypt the message
    encrypt(message, shift);
    printf("Encrypted message: %s\n", message);

    // Decrypt the message
    decrypt(message, shift);
    printf("Decrypted message: %s\n", message);

    return 0;
}

