#include "substitution_cipher.h"
#include <stdio.h>
#include <string.h>

int main() {
    char message[256];
    char key[27] = "QWERTYUIOPLKJHGFDSAZXCVBNM";  // Simple substitution key

    printf("Enter a message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';  // Remove the newline character

    // Encrypt the message
    substitution_encrypt(message, key);
    printf("Encrypted message: %s\n", message);

    // Decrypt the message to verify it works
    substitution_decrypt(message, key);
    printf("Decrypted message: %s\n", message);

    return 0;
}
