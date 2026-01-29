#ifndef COMMON_H
#define COMMON_H

#include <microhttpd.h>

/*
 * Sends a JSON response using the given connection.
 * Parameters:
 *   conn   - The libmicrohttpd connection object.
 *   json   - The JSON string to send.
 *   status - HTTP status code.
 * Returns:
 *   The result from MHD_queue_response.
 */
int send_json_response(struct MHD_Connection *conn, const char *json, unsigned int status);

#endif // COMMON_H

