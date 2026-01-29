#include "aes.h"
#include <stdio.h>
#include <stdint.h>

int main() {
    // Example 128-bit key and plaintext (16 bytes each)
    uint8_t message[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                           0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                       0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    printf("Original message: ");
    for (int i = 0; i < 16; i++) printf("%02x ", message[i]);
    printf("\n");

    // Encrypt the message
    aes_encrypt(message, key);

    printf("Encrypted message: ");
    for (int i = 0; i < 16; i++) printf("%02x ", message[i]);
    printf("\n");

    // Decrypt the message
    aes_decrypt(message, key);

    printf("Decrypted message: ");
    for (int i = 0; i < 16; i++) printf("%02x ", message[i]);
    printf("\n");

    return 0;
}
