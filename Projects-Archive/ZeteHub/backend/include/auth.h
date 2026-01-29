#ifndef AUTH_H
#define AUTH_H

#include <microhttpd.h>
#include <sqlite3.h>

/*
 * handle_login: Processes login requests using a POST processor.
 * It accumulates POST data (username and password) and, when complete,
 * verifies credentials and creates a session.
 *
 * Parameters:
 *   cls              - Unused context pointer.
 *   conn             - The libmicrohttpd connection object.
 *   url              - The request URL.
 *   method           - The HTTP method (should be "POST").
 *   version          - HTTP version string.
 *   upload_data      - Pointer to a chunk of uploaded data.
 *   upload_data_size - Pointer to the size of the uploaded data chunk.
 *   con_cls          - Pointer to connection-specific data.
 *   db               - SQLite database handle.
 *
 * Returns:
 *   An MHD_Result value.
 */
int handle_login(void *cls, struct MHD_Connection *conn,
                 const char *url, const char *method,
                 const char *version, const char *upload_data,
                 size_t *upload_data_size, void **con_cls, sqlite3 *db);

/*
 * handle_logout: Processes logout requests by invalidating the session.
 */
int handle_logout(struct MHD_Connection *conn);

/*
 * login_request_completed: Callback to clean up connection-specific data
 * when a request is terminated.
 */
void login_request_completed(void *cls, struct MHD_Connection *conn,
                             void **con_cls, enum MHD_RequestTerminationCode toe);

/*
 * handle_signup: Processes signup requests using a POST processor.
 * It accumulates POST data (username, password, code) and, when complete,
 * verifies the signup code and inserts the new user into the database.
 */
int handle_signup(void *cls, struct MHD_Connection *conn,
                  const char *url, const char *method,
                  const char *version, const char *upload_data,
                  size_t *upload_data_size, void **con_cls, sqlite3 *db);

#endif // AUTH_H

