#ifndef DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_H

unsigned long long generate_public_key(unsigned long long base, unsigned long long private_key, unsigned long long prime);
unsigned long long generate_shared_secret(unsigned long long received_public_key, unsigned long long private_key, unsigned long long prime);

#endif // DIFFIE_HELLMAN_H
