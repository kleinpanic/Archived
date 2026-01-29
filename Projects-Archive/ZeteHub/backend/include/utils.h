#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <microhttpd.h>
#include <sqlite3.h>

/* Token generation */
void generate_random_token(char *token, size_t size);

/* Password hashing (SHA-256) - output buffer ≥65 bytes */
void hash_password(const char *password, char *output, size_t output_size);

/* Verify password vs. stored hash */
int verify_password(const char *password, const char *stored_hash);

/* Session management */
void store_session(const char *token, int user_id, const char *role);
int  validate_session(const char *token);
void remove_session(const char *token);

#endif // UTILS_H

