#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Helper function to calculate the greatest common divisor (GCD)
static unsigned long gcd(unsigned long a, unsigned long b) {
    while (b != 0) {
        unsigned long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Helper function to find the modular inverse using the extended Euclidean algorithm
static unsigned long mod_inverse(unsigned long e, unsigned long phi) {
    long t = 0, newt = 1;
    unsigned long r = phi, newr = e;
    
    while (newr != 0) {
        unsigned long quotient = r / newr;

        // Update t and newt
        long temp = t;
        t = newt;
        newt = temp - quotient * newt;

        // Update r and newr
        unsigned long temp_r = r;
        r = newr;
        newr = temp_r - quotient * newr;
    }

    if (r > 1) return 0;  // e is not invertible
    if (t < 0) t += phi;  // Ensure t is positive
    
    return (unsigned long)t;
}


// Generate RSA keys (public and private)
void rsa_generate_keys(unsigned long *e, unsigned long *d, unsigned long *n) {
    // Two prime numbers (for simplicity, using small values)
    unsigned long p = 61, q = 53;
    *n = p * q;
    unsigned long phi = (p - 1) * (q - 1);

    // Choose e such that 1 < e < phi(n) and gcd(e, phi(n)) = 1
    *e = 17;  // Commonly chosen small prime number
    if (gcd(*e, phi) != 1) {
        printf("Error: e and phi(n) are not coprime.\n");
        exit(1);
    }

    // Calculate d, the modular inverse of e mod phi(n)
    *d = mod_inverse(*e, phi);
    if (*d == 0) {
        printf("Error: Could not find modular inverse of e.\n");
        exit(1);
    }
}

// Encrypt message using public key (e, n)
unsigned long rsa_encrypt(unsigned long message, unsigned long e, unsigned long n) {
    unsigned long result = 1;
    for (unsigned long i = 0; i < e; i++) {
        result = (result * message) % n;
    }
    return result;
}

// Decrypt ciphertext using private key (d, n)
unsigned long rsa_decrypt(unsigned long ciphertext, unsigned long d, unsigned long n) {
    unsigned long result = 1;
    for (unsigned long i = 0; i < d; i++) {
        result = (result * ciphertext) % n;
    }
    return result;
}

