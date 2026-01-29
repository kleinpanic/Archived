#include "ecc.h"
#include <stdio.h>
#include <stdlib.h>

// Modular inverse using extended Euclidean algorithm
static uint64_t mod_inverse(uint64_t a, uint64_t m) {
    int64_t m0 = m, t, q;
    int64_t x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

// Modular arithmetic: Add two points P + Q
ECPoint ecc_point_add(ECPoint P, ECPoint Q, EllipticCurve curve) {
    ECPoint R;
    if (P.x == 0 && P.y == 0) return Q;
    if (Q.x == 0 && Q.y == 0) return P;

    if (P.x == Q.x && P.y == Q.y) {
        return ecc_point_double(P, curve); // Doubling case
    }

    uint64_t lambda = ((Q.y + curve.p - P.y) * mod_inverse((Q.x + curve.p - P.x) % curve.p, curve.p)) % curve.p;
    R.x = (lambda * lambda + curve.p - P.x + curve.p - Q.x) % curve.p;
    R.y = (lambda * (P.x + curve.p - R.x) + curve.p - P.y) % curve.p;

    return R;
}

// Doubling case: 2P
ECPoint ecc_point_double(ECPoint P, EllipticCurve curve) {
    ECPoint R;
    uint64_t lambda = ((3 * P.x * P.x + curve.a) * mod_inverse(2 * P.y, curve.p)) % curve.p;
    R.x = (lambda * lambda + curve.p - 2 * P.x) % curve.p;
    R.y = (lambda * (P.x + curve.p - R.x) + curve.p - P.y) % curve.p;

    return R;
}

// Scalar multiplication: kP (repeated point addition)
ECPoint ecc_scalar_mult(uint64_t k, ECPoint P, EllipticCurve curve) {
    ECPoint R = {0, 0};  // Point at infinity
    ECPoint temp = P;

    while (k > 0) {
        if (k & 1) {
            R = ecc_point_add(R, temp, curve);
        }
        temp = ecc_point_double(temp, curve);
        k >>= 1;
    }

    return R;
}

// Key generation: Generate private and public key
ECCKeyPair ecc_generate_keypair(EllipticCurve curve) {
    uint64_t private_key = rand() % curve.n;
    ECPoint public_key = ecc_scalar_mult(private_key, curve.generator, curve);
    ECCKeyPair keypair = {private_key, public_key};

    return keypair;
}

// ECC encryption: Encrypts the plaintext and returns C1 (curve point) and C2 (encrypted message)
void ecc_encrypt(uint64_t plaintext, ECPoint public_key, EllipticCurve curve, ECPoint *C1, uint64_t *C2) {
    uint64_t k = rand() % curve.n;  // Random scalar for encryption
    *C1 = ecc_scalar_mult(k, curve.generator, curve);  // C1 = kG
    ECPoint shared_secret = ecc_scalar_mult(k, public_key, curve);  // kP

    *C2 = plaintext ^ (shared_secret.x % 256);  // XOR plaintext with shared secret's x-coordinate (mod 256)
}

// ECC decryption: Takes C1 and C2 to recover the original plaintext
uint64_t ecc_decrypt(ECPoint C1, uint64_t C2, uint64_t private_key, EllipticCurve curve) {
    ECPoint shared_secret = ecc_scalar_mult(private_key, C1, curve);  // dC1

    return C2 ^ (shared_secret.x % 256);  // XOR with shared secret's x-coordinate to recover plaintext
}

