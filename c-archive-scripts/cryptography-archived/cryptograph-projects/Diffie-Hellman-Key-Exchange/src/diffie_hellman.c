#include "diffie_hellman.h"
#include <stdio.h>
#include <math.h>

// Generate the public key: A = g^a mod p
unsigned long long generate_public_key(unsigned long long base, unsigned long long private_key, unsigned long long prime) {
    unsigned long long result = 1;
    for (unsigned long long i = 0; i < private_key; i++) {
        result = (result * base) % prime;
    }
    return result;
}

// Generate the shared secret: s = B^a mod p
unsigned long long generate_shared_secret(unsigned long long received_public_key, unsigned long long private_key, unsigned long long prime) {
    unsigned long long result = 1;
    for (unsigned long long i = 0; i < private_key; i++) {
        result = (result * received_public_key) % prime;
    }
    return result;
}
