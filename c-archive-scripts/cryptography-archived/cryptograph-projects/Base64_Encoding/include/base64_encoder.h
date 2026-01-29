#ifndef BASE64_ENCODER_H
#define BASE64_ENCODER_H

#include <stddef.h> // Include this to define size_t

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);
unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);

#endif // BASE64_ENCODER_H
