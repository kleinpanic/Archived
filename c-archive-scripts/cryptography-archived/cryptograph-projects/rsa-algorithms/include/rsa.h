#ifndef RSA_H
#define RSA_H

void rsa_generate_keys(unsigned long *e, unsigned long *d, unsigned long *n);
unsigned long rsa_encrypt(unsigned long message, unsigned long e, unsigned long n);
unsigned long rsa_decrypt(unsigned long ciphertext, unsigned long d, unsigned long n);

#endif // RSA_H
