#ifndef AES_H
#define AES_H

#include <stdint.h> 

void aes_encrypt(uint8_t *message, uint8_t *key);
void aes_decrypt(uint8_t *message, uint8_t *key);

#endif // AES_H
