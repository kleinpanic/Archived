#include <string.h>
#include <microhttpd.h>
#include "common.h"

int send_json_response(struct MHD_Connection *conn, const char *json, unsigned int status) {
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(json), (void*)json, MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(response, "Content-Type", "application/json");
    int ret = MHD_queue_response(conn, status, response);
    MHD_destroy_response(response);
    return ret;
}

