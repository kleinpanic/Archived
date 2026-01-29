#include "base64_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789+/";

static int base64_char_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1; // Should never happen for valid Base64 strings
}

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
    *output_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = (char *)malloc(*output_length + 1);
    if (encoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        encoded_data[j++] = base64_chars[(triple >> 18) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 12) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 6) & 0x3F];
        encoded_data[j++] = base64_chars[triple & 0x3F];
    }

    for (size_t i = 0; i < ((3 - (input_length % 3)) % 3); i++) {
        encoded_data[*output_length - 1 - i] = '=';
    }

    encoded_data[*output_length] = '\0';
    return encoded_data;
}

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length) {
    if (input_length % 4 != 0) {
        printf("Error: Input length is not a multiple of 4.\n");
        return NULL;
    }

    // Calculate output length, ignoring padding characters '='
    *output_length = (input_length / 4) * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = (unsigned char *)malloc(*output_length);
    if (decoded_data == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < input_length) {
        if (data[i] == '=') {
            break;  // Stop if we hit padding
        }

        // Read four Base64 characters and map them to their 6-bit values
        int sextet_a = base64_char_value(data[i++]);
        int sextet_b = base64_char_value(data[i++]);
        int sextet_c = (i < input_length && data[i] != '=') ? base64_char_value(data[i++]) : 0;
        int sextet_d = (i < input_length && data[i] != '=') ? base64_char_value(data[i++]) : 0;

        if (sextet_a == -1 || sextet_b == -1 || sextet_c == -1 || sextet_d == -1) {
            printf("Error: Invalid Base64 character detected.\n");
            free(decoded_data);
            return NULL;
        }

        // Combine the four 6-bit sextets into three bytes
        uint32_t triple = (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

        if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = triple & 0xFF;

        // Break once we have decoded the expected number of bytes
        if (j >= *output_length) {
            break;
        }
    }

    return decoded_data;
}

