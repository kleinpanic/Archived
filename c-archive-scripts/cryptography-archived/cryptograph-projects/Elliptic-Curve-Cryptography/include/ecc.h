#ifndef ECC_H
#define ECC_H

#include <stdint.h>

// Struct for representing a point on the elliptic curve
typedef struct {
    uint64_t x;
    uint64_t y;
} ECPoint;

// Define an elliptic curve
typedef struct {
    uint64_t a;
    uint64_t b;
    uint64_t p;  // Prime modulus
    ECPoint generator;  // Generator point
    uint64_t n;  // Order of the curve
} EllipticCurve;

// Key pair
typedef struct {
    uint64_t private_key;
    ECPoint public_key;
} ECCKeyPair;

// Function declarations
ECPoint ecc_point_add(ECPoint P, ECPoint Q, EllipticCurve curve);
ECPoint ecc_point_double(ECPoint P, EllipticCurve curve);
ECPoint ecc_scalar_mult(uint64_t k, ECPoint P, EllipticCurve curve);
ECCKeyPair ecc_generate_keypair(EllipticCurve curve);
void ecc_encrypt(uint64_t plaintext, ECPoint public_key, EllipticCurve curve, ECPoint *C1, uint64_t *C2);
uint64_t ecc_decrypt(ECPoint C1, uint64_t C2, uint64_t private_key, EllipticCurve curve);

#endif
