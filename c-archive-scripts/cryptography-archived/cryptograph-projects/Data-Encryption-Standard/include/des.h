#ifndef DES_H
#define DES_H

void des_encrypt(unsigned long long *message, unsigned long long key);
void des_decrypt(unsigned long long *message, unsigned long long key);

#endif // DES_H
