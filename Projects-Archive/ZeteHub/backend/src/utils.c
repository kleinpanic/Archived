/*
 * utils.c - Implements helper functions for token generation, password hashing,
 * database initialization, and session management.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include <sqlite3.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

void generate_random_token(char *token, size_t size) {
    unsigned char buffer[(size-1)/2];
    if (RAND_bytes(buffer, sizeof(buffer)) != 1) {
        srand(time(NULL));
        for (size_t i = 0; i < sizeof(buffer); i++) {
            buffer[i] = rand() % 256;
        }
    }
    for (size_t i = 0; i < sizeof(buffer); i++) {
        sprintf(token + (i * 2), "%02x", buffer[i]);
    }
    token[size-1] = '\0';
}

/*
 * hash_password - Computes the SHA-256 hash of a password and returns the result as a hex string.
 */
void hash_password(const char *password, char *output, size_t output_size) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    // compute SHA-256
    SHA256((const unsigned char*)password, strlen(password), hash);
    // write each byte as two hex chars
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        // we know output_size >= 65
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    // null-terminate
    output[SHA256_DIGEST_LENGTH * 2] = '\0';
}

/*
 * verify_password - Hashes the provided password and compares it to the stored hash.
 * Returns 1 if they match, 0 otherwise.
 */
int verify_password(const char *password, const char *stored_hash) {
    char hashed[65];
    hash_password(password, hashed, sizeof(hashed));
    return strcmp(hashed, stored_hash) == 0;
}

/* Session management implementation remains unchanged */
typedef struct Session {
    char token[65];
    int user_id;
    char role[32];
    struct Session *next;
} Session;

static Session *session_store = NULL;

void store_session(const char *token, int user_id, const char *role) {
    Session *new_session = malloc(sizeof(Session));
    if (!new_session)
      return;
    strncpy(new_session->token, token, sizeof(new_session->token));
    new_session->token[sizeof(new_session->token)-1] = '\0';
    new_session->user_id = user_id;
    strncpy(new_session->role, role, sizeof(new_session->role));
    new_session->role[sizeof(new_session->role)-1] = '\0';
    new_session->next = session_store;
    session_store = new_session;
}

int validate_session(const char *token) {
    if (!token) return -1;
    Session *current = session_store;
    while (current) {
        if (strcmp(current->token, token) == 0)
            return current->user_id;
        current = current->next;
    }
    return -1;
}

void remove_session(const char *token) {
    if (!token) return;
    Session **curr = &session_store;
    while(*curr) {
         if (strcmp((*curr)->token, token) == 0) {
              Session *to_delete = *curr;
              *curr = to_delete->next;
              free(to_delete);
              return;
         }
         curr = &((*curr)->next);
    }
}

