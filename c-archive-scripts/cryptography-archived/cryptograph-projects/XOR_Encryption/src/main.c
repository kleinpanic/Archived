#include "xor_cipher.h"
#include <stdio.h>
#include <string.h>

// Function to display encrypted message in hexadecimal format
void print_hex(const char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        printf("%02x ", (unsigned char)data[i]);
    }
    printf("\n");
}

// Function to safely print decrypted message, replacing nulls with spaces
void print_decrypted_message(const char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        // If the character is NULL, print a space instead
        putchar(data[i] == '\0' ? ' ' : data[i]);
    }
    printf("\n");
}

int main() {
    char message[256];
    char key[256];

    // Get the message to encrypt
    printf("Enter a message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // Remove newline character

    // Get the encryption key
    printf("Enter the encryption key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // Remove newline character

    // Check if message or key is empty
    if (strlen(message) == 0 || strlen(key) == 0) {
        printf("Error: Message or key cannot be empty.\n");
        return 1;
    }

    // Encrypt the message
    xor_encrypt_decrypt(message, key);
    printf("Encrypted message (hexadecimal): ");
    print_hex(message);

    // Decrypt the message (using the same function, since XOR is symmetric)
    xor_encrypt_decrypt(message, key);
    printf("Decrypted message: ");
    print_decrypted_message(message);

    return 0;
}

