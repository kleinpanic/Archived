#ifndef API_H
#define API_H

#include <microhttpd.h>
#include <sqlite3.h>

/*
 * handle_get_user: Returns current user information based on session.
 */
int handle_get_user(struct MHD_Connection *conn, sqlite3 *db);

/*
 * handle_get_events: Returns a list of events as JSON.
 */
int handle_get_events(struct MHD_Connection *conn, sqlite3 *db);

#endif // API_H

