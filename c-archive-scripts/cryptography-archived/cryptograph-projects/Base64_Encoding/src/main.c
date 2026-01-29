#include "base64_encoder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Include this for memory management functions like free

int main() {
    char message[256];
    size_t output_length;

    printf("Enter a message to encode: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // Remove newline character

    // Encode the message
    char *encoded = base64_encode((unsigned char *)message, strlen(message), &output_length);
    if (encoded == NULL) {
        printf("Failed to encode the message.\n");
        return 1;
    }

    printf("Encoded message: %s\n", encoded);

    // Decode the message
    size_t decoded_length;
    unsigned char *decoded = base64_decode(encoded, output_length, &decoded_length);
    if (decoded == NULL) {
        printf("Failed to decode the message.\n");
        free(encoded);
        return 1;
    }

    printf("Decoded message: %.*s\n", (int)decoded_length, decoded);

    // Clean up
    free(encoded);
    free(decoded);

    return 0;
}
