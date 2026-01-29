/*
 * api.c - Implements API endpoint functions.
 * Provides functionality for fetching user information and events.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>
#include <sqlite3.h>
#include "api.h"
#include "auth.h"
#include "common.h"
#include "utils.h"  // Added to get the declaration for validate_session

/*
 * send_json_response: Declaration from main (or could be moved to a common utility file).
 * For simplicity, we re-declare it here.
 */
extern int send_json_response(struct MHD_Connection *conn, const char *json, unsigned int status);

/*
 * handle_get_user: Returns information about the current user.
 * It validates the session and then queries the database for the user record.
 */
int handle_get_user(struct MHD_Connection *conn, sqlite3 *db) {
    const char *token = MHD_lookup_connection_value(conn, MHD_COOKIE_KIND, "session");
    int user_id = validate_session(token);
    if (user_id < 0) {
        return send_json_response(conn, "{\"error\":\"Not authenticated\"}", MHD_HTTP_UNAUTHORIZED);
    }
    
    // Query user information from the database.
    const char *sql = "SELECT first_name, last_mame, role FROM members WHERE id = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return send_json_response(conn, "{\"error\":\"DB error\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }
    sqlite3_bind_int(stmt, 1, user_id);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return send_json_response(conn, "{\"error\":\"User not found\"}", MHD_HTTP_NOT_FOUND);
    }
    
    const char *firstName = (const char *)sqlite3_column_text(stmt, 0);
    const char *lastName = (const char *)sqlite3_column_text(stmt, 1);
    const char *role = (const char *)sqlite3_column_text(stmt, 2);
    sqlite3_finalize(stmt);
    
    // Construct JSON response with user details.
    char json[256];
    snprintf(json, sizeof(json),
             "{\"firstName\":\"%s\", \"lastName\":\"%s\", \"role\":\"%s\"}",
             firstName, lastName, role);
    return send_json_response(conn, json, MHD_HTTP_OK);
}

/*
 * handle_get_events: Returns a JSON list of events.
 * This is a simplified example returning all events.
 */
int handle_get_events(struct MHD_Connection *conn, sqlite3 *db) {
    const char *token = MHD_lookup_connection_value(conn, MHD_COOKIE_KIND, "session");
    int user_id = validate_session(token);
    if (user_id < 0) {
        return send_json_response(conn, "{\"error\":\"Not authenticated\"}", MHD_HTTP_UNAUTHORIZED);
    }
    
    const char *sql = "SELECT id, title, start, end, location, description FROM events";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return send_json_response(conn, "{\"error\":\"DB error\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }
    
    // Build the JSON array string.
    char json[4096] = "{\"events\":[";
    int first = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!first)
            strcat(json, ",");
        first = 0;
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char *)sqlite3_column_text(stmt, 1);
        const char *start = (const char *)sqlite3_column_text(stmt, 2);
        const char *end = (const char *)sqlite3_column_text(stmt, 3);
        const char *location = (const char *)sqlite3_column_text(stmt, 4);
        const char *desc = (const char *)sqlite3_column_text(stmt, 5);
        char eventJson[512];
        snprintf(eventJson, sizeof(eventJson),
                 "{\"id\":%d,\"title\":\"%s\",\"start\":\"%s\",\"end\":\"%s\",\"location\":\"%s\",\"description\":\"%s\"}",
                 id, title, start, end, location, desc);
        strcat(json, eventJson);
    }
    sqlite3_finalize(stmt);
    strcat(json, "]}");
    return send_json_response(conn, json, MHD_HTTP_OK);
}

