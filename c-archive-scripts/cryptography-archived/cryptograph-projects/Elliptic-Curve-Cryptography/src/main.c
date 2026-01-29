#include "ecc.h"
#include <stdio.h>

int main() {
    // Define the elliptic curve
    EllipticCurve curve = {
        .a = 2,
        .b = 3,
        .p = 97,  // Small prime for simplicity
        .generator = {3, 6},  // Generator point
        .n = 5  // Order of the generator
    };

    // Generate keypair for ECC
    ECCKeyPair keypair = ecc_generate_keypair(curve);
    printf("Private Key: %lu\n", keypair.private_key);
    printf("Public Key: (%lu, %lu)\n", keypair.public_key.x, keypair.public_key.y);

    // Encrypt a message
    uint64_t plaintext = 42;
    printf("Plaintext: %lu\n", plaintext);

    ECPoint C1;
    uint64_t C2;
    ecc_encrypt(plaintext, keypair.public_key, curve, &C1, &C2);
    printf("Ciphertext: (C1: %lu, %lu), C2: %lu\n", C1.x, C1.y, C2);

    // Decrypt the message
    uint64_t decrypted = ecc_decrypt(C1, C2, keypair.private_key, curve);
    printf("Decrypted: %lu\n", decrypted);

    return 0;
}
