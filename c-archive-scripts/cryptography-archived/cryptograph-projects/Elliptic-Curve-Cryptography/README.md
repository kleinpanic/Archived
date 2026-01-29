# Elliptic Curve Cryptography (ECC) Project

## Introduction

This project implements a basic Elliptic Curve Cryptography (ECC) system in C. ECC is a type of public key encryption based on the mathematics of elliptic curves, which provides the same level of security as other public key cryptosystems (like RSA) but with smaller key sizes.

## How It Works

Elliptic curves are defined by equations of the form:

y² = x³ + ax + b (mod p)

vbnet


Where:
- `a` and `b` are constants defining the curve.
- `p` is the prime modulus.
- Points on the curve follow specific rules for addition and scalar multiplication.

### Encryption and Decryption Steps:
1. **Key Generation**: 
   - The private key is a random number.
   - The public key is computed as a point on the elliptic curve by multiplying the private key with a generator point.
2. **Encryption**:
   - A random number `k` is generated.
   - Two values are computed: `C1 = kG` (a point on the curve) and `C2 = plaintext XOR kP.x` (where `P` is the recipient's public key).
3. **Decryption**:
   - The recipient computes the shared secret `dC1`, where `d` is their private key.
   - The plaintext is recovered by XOR'ing `C2` with the shared secret's x-coordinate.

## Expected Output

Here is an example of the output:

Private Key: 3 Public Key: (38, 59) Plaintext: 42 Ciphertext: (C1: 1, 59), C2: 59 Decrypted: 42

markdown


The **decrypted message** should always match the **original plaintext**.

## How to Build and Run

1. To build the project, run:
    ```bash
    make
    ```

2. To execute the ECC encryption program:
    ```bash
    ./ecc
    ```

3. To clean the build:
    ```bash
    make clean
    ```

## File Structure
- `src/`: Source code for the ECC implementation.
- `include/`: Header files.
- `build/`: Compiled binary.
- `obj/`: Object files.

## Elliptic Curve Parameters

For simplicity, this implementation uses a small prime `p = 97` and a small generator point `(3, 6)` on the elliptic curve `y² = x³ + 2x + 3`. In real-world applications, much larger prime values and more complex elliptic curves are used for better security.

## Future Work

In the future, this project could be extended by:
- Supporting larger prime numbers and more complex elliptic curves.
- Implementing more advanced elliptic curve encryption algorithms such as Elliptic Curve Diffie-Hellman (ECDH) or Elliptic Curve Digital Signature Algorithm (ECDSA).

## License

This project is open-source and available under the MIT license.
