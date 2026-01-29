// src/auth.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <microhttpd.h>
#include <sqlite3.h>
#include "auth.h"
#include "common.h"
#include "utils.h"

/* Per-connection state for parsing POST form fields */
struct connection_info_struct {
    struct MHD_PostProcessor *pp;
    char *username;
    char *password;
    char *code;
};

/* Allocate a NUL-terminated copy of data[0..size-1] */
static char *memdup_str(const char *data, size_t size) {
    char *s = malloc(size + 1);
    if (!s) {
        fprintf(stderr, "[auth] memdup_str: malloc failed (%s)\n", strerror(errno));
        return NULL;
    }
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

/* libmicrohttpd callback for each form field */
static enum MHD_Result iterate_post(void *cls,
    enum MHD_ValueKind kind,
    const char *key,
    const char *filename,
    const char *content_type,
    const char *transfer_encoding,
    const char *data, uint64_t off, size_t size)
{
    struct connection_info_struct *ci = cls;
    if (size == 0) return MHD_YES;

    fprintf(stderr, "[auth] iterate_post: key='%s' size=%zu\n", key, size);

    if (strcmp(key, "username") == 0) {
        free(ci->username);
        ci->username = memdup_str(data, size);
        fprintf(stderr, "[auth] got username='%s'\n", ci->username);
    }
    else if (strcmp(key, "password") == 0) {
        free(ci->password);
        ci->password = memdup_str(data, size);
        fprintf(stderr, "[auth] got password (len=%zu)\n", size);
    }
    else if (strcmp(key, "code") == 0) {
        free(ci->code);
        ci->code = memdup_str(data, size);
        fprintf(stderr, "[auth] got code='%s'\n", ci->code);
    }
    return MHD_YES;
}

/* Cleanup after request completes */
void login_request_completed(void *cls,
    struct MHD_Connection *conn,
    void **con_cls,
    enum MHD_RequestTerminationCode toe)
{
    struct connection_info_struct *ci = *con_cls;
    if (!ci) return;
    fprintf(stderr, "[auth] cleaning up connection\n");
    if (ci->pp) MHD_destroy_post_processor(ci->pp);
    free(ci->username);
    free(ci->password);
    free(ci->code);
    free(ci);
    *con_cls = NULL;
}

/*
 * handle_login - authenticate user, issue session cookie, return JSON
 */
int handle_login(void *cls,
    struct MHD_Connection *conn,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls,
    sqlite3 *db)
{
    fprintf(stderr, "[auth] handle_login: %s %s\n", method, url);

    if (*con_cls == NULL) {
        /* First call: allocate state and post processor */
        fprintf(stderr, "[auth] handle_login: allocating state\n");
        struct connection_info_struct *ci = calloc(1, sizeof(*ci));
        if (!ci) {
            fprintf(stderr, "[auth] OOM allocating state\n");
            return send_json_response(conn, "{\"error\":\"Server OOM\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
        }
        ci->pp = MHD_create_post_processor(conn, 4096, iterate_post, ci);
        *con_cls = ci;
        return MHD_YES;
    }

    struct connection_info_struct *ci = *con_cls;
    if (*upload_data_size) {
        /* Feed data to post processor */
        MHD_post_process(ci->pp, upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }

    /* All data received */
    fprintf(stderr, "[auth] handle_login: processing login for '%s'\n",
            ci->username ? ci->username : "<null>");

    if (!ci->username || !ci->password) {
        fprintf(stderr, "[auth] missing credentials\n");
        return send_json_response(conn, "{\"error\":\"Missing credentials\"}", MHD_HTTP_BAD_REQUEST);
    }

    /* 1) Query user record */
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id,password_hash,role FROM members WHERE username = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "[auth] sqlite3_prepare_v2 failed: %s\n", sqlite3_errmsg(db));
        return send_json_response(conn, "{\"error\":\"Database error\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }
    sqlite3_bind_text(stmt, 1, ci->username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        fprintf(stderr, "[auth] user '%s' not found\n", ci->username);
        sqlite3_finalize(stmt);
        return send_json_response(conn, "{\"error\":\"Invalid credentials\"}", MHD_HTTP_UNAUTHORIZED);
    }

    /* 2) Copy stored hash & role & user_id */
    char stored_hash[65];
    strncpy(stored_hash, (const char*)sqlite3_column_text(stmt,1), 64);
    stored_hash[64] = '\0';

    char role[32];
    strncpy(role, (const char*)sqlite3_column_text(stmt,2), 31);
    role[31] = '\0';

    int user_id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    fprintf(stderr, "[auth] stored_hash = %s\n", stored_hash);

    /* 3) Compute hash of supplied password */
    char computed_hash[65];
    hash_password(ci->password, computed_hash, sizeof(computed_hash));
    fprintf(stderr, "[auth] computed_hash = %s\n", computed_hash);

    /* 4) Compare */
    if (strcmp(computed_hash, stored_hash) != 0) {
        fprintf(stderr, "[auth] password mismatch\n");
        return send_json_response(conn, "{\"error\":\"Invalid credentials\"}", MHD_HTTP_UNAUTHORIZED);
    }

    fprintf(stderr, "[auth] login successful (id=%d, role=%s)\n", user_id, role);

    /* 5) Issue session token */
    char token[65];
    generate_random_token(token, sizeof(token));
    fprintf(stderr, "[auth] generated token %s\n", token);
    store_session(token, user_id, role);

    /* 6) Build JSON response + Set-Cookie */
    const char *body = "{\"message\":\"Logged in\"}";
    struct MHD_Response *resp = MHD_create_response_from_buffer(
        strlen(body), (void*)body, MHD_RESPMEM_MUST_COPY);
    if (!resp) {
        fprintf(stderr, "[auth] MHD_create_response failed\n");
        return send_json_response(conn, "{\"error\":\"Server error\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }
    char hdr[128];
    snprintf(hdr, sizeof(hdr), "session=%s; HttpOnly", token);
    MHD_add_response_header(resp, "Set-Cookie", hdr);
    MHD_add_response_header(resp, "Content-Type", "application/json");

    int ret = MHD_queue_response(conn, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
}

/*
 * handle_logout - invalidate the session
 */
int handle_logout(struct MHD_Connection *conn) {
    fprintf(stderr, "[auth] handle_logout\n");
    const char *token = MHD_lookup_connection_value(conn, MHD_COOKIE_KIND, "session");
    if (token) {
        fprintf(stderr, "[auth] removing session %s\n", token);
        remove_session(token);
    }
    return send_json_response(conn, "{\"message\":\"Logged out\"}", MHD_HTTP_OK);
}

/*
 * handle_signup - verify signup code & create new member
 */
int handle_signup(void *cls,
    struct MHD_Connection *conn,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls,
    sqlite3 *db)
{
    fprintf(stderr, "[auth] handle_signup: %s %s\n", method, url);

    if (*con_cls == NULL) {
        fprintf(stderr, "[auth] handle_signup: allocating state\n");
        struct connection_info_struct *ci = calloc(1, sizeof(*ci));
        if (!ci) {
            fprintf(stderr, "[auth] OOM in signup\n");
            return send_json_response(conn, "{\"error\":\"Server OOM\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
        }
        ci->pp = MHD_create_post_processor(conn, 4096, iterate_post, ci);
        *con_cls = ci;
        return MHD_YES;
    }

    struct connection_info_struct *ci = *con_cls;
    if (*upload_data_size) {
        MHD_post_process(ci->pp, upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }

    fprintf(stderr, "[auth] signup fields: user='%s' code='%s'\n",
            ci->username?ci->username:"<null>",
            ci->code?ci->code:"<null>");

    if (!ci->username || !ci->password || !ci->code) {
        fprintf(stderr, "[auth] missing signup fields\n");
        return send_json_response(conn, "{\"error\":\"Missing signup fields\"}", MHD_HTTP_BAD_REQUEST);
    }

    /* Fetch expected code */
    char expected[128] = {0};
    sqlite3_stmt *st;
    sqlite3_prepare_v2(db,
        "SELECT value FROM settings WHERE key='signup_code';",
        -1, &st, NULL);
    if (sqlite3_step(st) == SQLITE_ROW) {
        strncpy(expected, (char*)sqlite3_column_text(st,0), 127);
    }
    sqlite3_finalize(st);
    fprintf(stderr, "[auth] expected signup_code = '%s'\n", expected);

    if (strcmp(ci->code, expected) != 0) {
        fprintf(stderr, "[auth] invalid signup code\n");
        return send_json_response(conn, "{\"error\":\"Invalid signup code\"}", MHD_HTTP_FORBIDDEN);
    }

    /* Check uniqueness */
    sqlite3_prepare_v2(db,
        "SELECT 1 FROM members WHERE username = ?;", -1, &st, NULL);
    sqlite3_bind_text(st, 1, ci->username, -1, SQLITE_STATIC);
    if (sqlite3_step(st) == SQLITE_ROW) {
        fprintf(stderr, "[auth] username taken\n");
        sqlite3_finalize(st);
        return send_json_response(conn, "{\"error\":\"Username taken\"}", MHD_HTTP_CONFLICT);
    }
    sqlite3_finalize(st);

    /* Hash password */
    char hash_hex[65];
    hash_password(ci->password, hash_hex, sizeof(hash_hex));
    fprintf(stderr, "[auth] signup computed hash = %s\n", hash_hex);

    /* Insert member */
    sqlite3_prepare_v2(db,
        "INSERT INTO members(username,password_hash,salt,first_name,last_name,role) "
        "VALUES (?,?,'','', '', 'member');",
        -1, &st, NULL);
    sqlite3_bind_text(st, 1, ci->username, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 2, hash_hex,      -1, SQLITE_STATIC);
    if (sqlite3_step(st) != SQLITE_DONE) {
        fprintf(stderr, "[auth] signup sqlite error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(st);
        return send_json_response(conn, "{\"error\":\"Signup failed\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }
    sqlite3_finalize(st);

    fprintf(stderr, "[auth] signup successful for '%s'\n", ci->username);
    return send_json_response(conn, "{\"message\":\"Signup successful\"}", MHD_HTTP_OK);
}

