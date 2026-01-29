#include "rsa.h"
#include <stdio.h>

int main() {
    unsigned long e, d, n;

    // Generate public and private keys
    rsa_generate_keys(&e, &d, &n);
    printf("Public Key: (e = %lu, n = %lu)\n", e, n);
    printf("Private Key: (d = %lu, n = %lu)\n", d, n);

    // Ask for a message to encrypt (assuming it's an integer for simplicity)
    unsigned long message;
    printf("Enter a message to encrypt (as an integer smaller than %lu): ", n);
    scanf("%lu", &message);

    // Ensure message is smaller than n
    if (message >= n) {
        printf("Error: Message must be smaller than %lu.\n", n);
        return 1;
    }

    // Encrypt the message
    unsigned long ciphertext = rsa_encrypt(message, e, n);
    printf("Encrypted message: %lu\n", ciphertext);

    // Decrypt the message
    unsigned long decrypted_message = rsa_decrypt(ciphertext, d, n);
    printf("Decrypted message: %lu\n", decrypted_message);

    return 0;
}

