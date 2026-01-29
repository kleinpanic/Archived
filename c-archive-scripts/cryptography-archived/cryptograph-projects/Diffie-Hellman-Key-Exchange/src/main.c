#include "diffie_hellman.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Publicly agreed upon base (g) and prime (p)
    unsigned long long base = 5;
    unsigned long long prime = 23;

    // Private keys chosen by Alice and Bob (should be random in practice)
    unsigned long long alice_private_key, bob_private_key;
    srand(time(NULL));
    alice_private_key = rand() % (prime - 1) + 1;  // Random private key for Alice
    bob_private_key = rand() % (prime - 1) + 1;    // Random private key for Bob

    printf("Alice's private key: %llu\n", alice_private_key);
    printf("Bob's private key: %llu\n", bob_private_key);

    // Generate public keys
    unsigned long long alice_public_key = generate_public_key(base, alice_private_key, prime);
    unsigned long long bob_public_key = generate_public_key(base, bob_private_key, prime);

    printf("Alice's public key: %llu\n", alice_public_key);
    printf("Bob's public key: %llu\n", bob_public_key);

    // Exchange public keys and generate shared secrets
    unsigned long long alice_shared_secret = generate_shared_secret(bob_public_key, alice_private_key, prime);
    unsigned long long bob_shared_secret = generate_shared_secret(alice_public_key, bob_private_key, prime);

    printf("Alice's shared secret: %llu\n", alice_shared_secret);
    printf("Bob's shared secret: %llu\n", bob_shared_secret);

    // Verify if shared secrets match
    if (alice_shared_secret == bob_shared_secret) {
        printf("Key exchange successful! Shared secret: %llu\n", alice_shared_secret);
    } else {
        printf("Key exchange failed.\n");
    }

    return 0;
}
