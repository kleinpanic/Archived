/*
 * main.c - Entry point for the Zeta Psi backend server.
 *
 * Parses a --development flag to pick dev vs prod DB,
 * ensures the db/ directory exists, initializes the schema via init_db.c,
 * then starts the HTTP server (libmicrohttpd) with
 * request_handler + login_request_completed callbacks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <microhttpd.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "auth.h"
#include "api.h"
#include "common.h"
#include "utils.h"
#include "init_db.h"

#define PORT       8080
#define STATIC_DIR "../frontend"  /* serves both /public and /js subdirs */

static sqlite3 *db = NULL;

/*
 * serve_file: send a static file (HTML/CSS/JS) or return a 404 page
 */
static enum MHD_Result serve_file(struct MHD_Connection *conn, const char *filepath) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        const char *not_found_html =
          "<html><head><title>404 Not Found</title></head>"
          "<body><h1>404 - Page Not Found</h1>"
          "<p>The requested page could not be found.</p></body></html>";
        struct MHD_Response *r = MHD_create_response_from_buffer(
          strlen(not_found_html),
          (void*)not_found_html,
          MHD_RESPMEM_PERSISTENT
        );
        MHD_add_response_header(r, "Content-Type", "text/html");
        int ret = MHD_queue_response(conn, MHD_HTTP_NOT_FOUND, r);
        MHD_destroy_response(r);
        return ret;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buf = malloc(size);
    if (!buf) {
        fclose(fp);
        return send_json_response(
          conn,
          "{\"error\":\"Memory allocation error\"}",
          MHD_HTTP_INTERNAL_SERVER_ERROR
        );
    }
    fread(buf, 1, size, fp);
    fclose(fp);

    struct MHD_Response *r = MHD_create_response_from_buffer(
      size,
      buf,
      MHD_RESPMEM_MUST_FREE
    );

    const char *ext = strrchr(filepath, '.');
    if (ext && strcmp(ext, ".html") == 0) {
        MHD_add_response_header(r, "Content-Type", "text/html");
    } else if (ext && strcmp(ext, ".css") == 0) {
        MHD_add_response_header(r, "Content-Type", "text/css");
    } else if (ext && strcmp(ext, ".js") == 0) {
        MHD_add_response_header(r, "Content-Type", "application/javascript");
    } else {
        MHD_add_response_header(r, "Content-Type", "application/octet-stream");
    }

    int ret = MHD_queue_response(conn, MHD_HTTP_OK, r);
    MHD_destroy_response(r);
    return ret;
}

/*
 * request_handler: routes URLs beginning with /api/ to C handlers,
 * and everything else to static files.
 */
static enum MHD_Result request_handler(void *cls,
    struct MHD_Connection *conn,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls)
{
    /* Handle API calls first */
    if (strncmp(url, "/api/", 5) == 0) {
        if (strcmp(url, "/api/login") == 0 && strcmp(method, "POST") == 0) {
            return handle_login(
              NULL, conn, url, method, version,
              upload_data, upload_data_size, con_cls, db
            );
        }
        if (strcmp(url, "/api/logout") == 0 && strcmp(method, "POST") == 0) {
            return handle_logout(conn);
        }
        if (strcmp(url, "/api/signup") == 0 && strcmp(method, "POST") == 0) {
            return handle_signup(
              NULL, conn, url, method, version,
              upload_data, upload_data_size, con_cls, db
            );
        }
        if (strcmp(url, "/api/user/me") == 0 && strcmp(method, "GET") == 0) {
            return handle_get_user(conn, db);
        }
        if (strncmp(url, "/api/events", 11) == 0 && strcmp(method, "GET") == 0) {
            return handle_get_events(conn, db);
        }
        return send_json_response(conn, "{\"error\":\"Not found\"}", MHD_HTTP_NOT_FOUND);
    }

    /* Serve static files:
     *  - URLs starting /js/ → frontend/js
     *  - everything else → frontend/public
     */
    char path[512];
    if (strncmp(url, "/js/", 4) == 0) {
        snprintf(path, sizeof(path), "%s%s", STATIC_DIR, url);
    } else {
        if (strcmp(url, "/") == 0) {
            snprintf(path, sizeof(path), "%s/public/index.html", STATIC_DIR);
        } else {
            snprintf(path, sizeof(path), "%s/public%s", STATIC_DIR, url);
        }
    }
    return serve_file(conn, path);
}

int main(int argc, char *argv[]) {
    bool dev_mode = false;
    const char *db_dir  = "db";
    const char *db_path = "db/fraternity.prod.db";

    /* Parse --development flag */
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--development") == 0) {
            dev_mode = true;
            db_path = "db/fraternity.dev.db";
        }
    }

    /* Ensure the db/ directory exists */
    {
        struct stat st;
        if (stat(db_dir, &st) == -1) {
            if (mkdir(db_dir, 0755) != 0) {
                perror("Failed to create db directory");
                return EXIT_FAILURE;
            }
        }
    }

    /* Open (or create) the chosen database */
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database '%s': %s\n",
                db_path, sqlite3_errmsg(db));
        return EXIT_FAILURE;
    }

    /* Initialize schema & seed dev data if requested */
    if (initialize_database(db, dev_mode) != 0) {
        fprintf(stderr, "Failed to initialize database schema\n");
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    /* Start the HTTP server */
    struct MHD_Daemon *daemon = MHD_start_daemon(
        MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD,
        PORT,
        NULL, NULL,                    /* accept-policy */
        &request_handler, NULL,       /* our request handler */
        MHD_OPTION_NOTIFY_COMPLETED,
          login_request_completed, NULL,
        MHD_OPTION_END
    );

    if (!daemon) {
        fprintf(stderr, "Failed to start HTTP server\n");
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    printf("Server started (%s) on port %d. Press Enter to stop.\n",
           dev_mode ? "development" : "production", PORT);
    (void)getchar();

    MHD_stop_daemon(daemon);
    sqlite3_close(db);
    return EXIT_SUCCESS;
}

