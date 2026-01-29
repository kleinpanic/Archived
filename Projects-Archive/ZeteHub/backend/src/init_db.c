// src/init_db.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "init_db.h"
#include "utils.h"

// Run a SQL statement, printing any error.
static int exec_sql(sqlite3 *db, const char *sql) {
    char *errmsg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n  => %s\n", errmsg, sql);
        sqlite3_free(errmsg);
    }
    return rc;
}

int initialize_database(sqlite3 *db, bool dev_mode) {
    int rc;

    // Enable foreign keys
    if ((rc = exec_sql(db, "PRAGMA foreign_keys = ON;")) != SQLITE_OK) return rc;

    // 1) members: drop NOT NULL on email so signup INSERT without email works
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS members ("
      "  id            INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  username      TEXT    NOT NULL UNIQUE,"
      "  password_hash TEXT    NOT NULL,"
      "  salt          TEXT    NOT NULL,"
      "  first_name    TEXT    NOT NULL,"
      "  last_name     TEXT    NOT NULL,"
      "  role          TEXT    NOT NULL DEFAULT 'member',"
      "  email         TEXT    UNIQUE,"
      "  phone_number  TEXT,"
      "  last_login    DATETIME"
      ");"
    )) != SQLITE_OK) return rc;

    // 2) signup_requests (unused for now, but created)
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS signup_requests ("
      "  id             INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  first_name     TEXT    NOT NULL,"
      "  last_name      TEXT    NOT NULL,"
      "  email          TEXT    NOT NULL UNIQUE,"
      "  phone_number   TEXT,"
      "  requested_code TEXT    NOT NULL,"
      "  created_at     DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP"
      ");"
    )) != SQLITE_OK) return rc;

    // 2b) settings
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS settings ("
      "  key        TEXT PRIMARY KEY,"
      "  value      TEXT    NOT NULL,"
      "  updated_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP"
      ");"
    )) != SQLITE_OK) return rc;

    // 3) sessions
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS sessions ("
      "  token      TEXT    PRIMARY KEY,"
      "  user_id    INTEGER NOT NULL,"
      "  issued_at  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
      "  expires_at DATETIME NOT NULL,"
      "  FOREIGN KEY(user_id) REFERENCES members(id) ON DELETE CASCADE"
      ");"
    )) != SQLITE_OK) return rc;

    // 4) notifications
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS notifications ("
      "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  user_id     INTEGER NOT NULL,"
      "  type        TEXT    NOT NULL,"
      "  message     TEXT    NOT NULL,"
      "  is_read     INTEGER NOT NULL DEFAULT 0,"
      "  created_at  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
      "  FOREIGN KEY(user_id) REFERENCES members(id) ON DELETE CASCADE"
      ");"
    )) != SQLITE_OK) return rc;

    // 5) audit_log
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS audit_log ("
      "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  user_id      INTEGER,"
      "  action       TEXT    NOT NULL,"
      "  target_table TEXT,"
      "  target_id    INTEGER,"
      "  ip_address   TEXT,"
      "  details      TEXT,"
      "  created_at   DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
      "  FOREIGN KEY(user_id) REFERENCES members(id)"
      ");"
    )) != SQLITE_OK) return rc;

    // 6) events
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS events ("
      "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  title        TEXT    NOT NULL,"
      "  start        DATETIME NOT NULL,"
      "  end          DATETIME,"
      "  location     TEXT,"
      "  description  TEXT,"
      "  created_by   INTEGER,"
      "  created_at   DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
      "  FOREIGN KEY(created_by) REFERENCES members(id)"
      ");"
    )) != SQLITE_OK) return rc;

    // 7) financials
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS financials ("
      "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  user_id      INTEGER NOT NULL,"
      "  category     TEXT    NOT NULL,"
      "  amount       REAL    NOT NULL,"
      "  date         DATETIME NOT NULL,"
      "  description  TEXT,"
      "  status       TEXT    NOT NULL DEFAULT 'pending',"
      "  created_at   DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
      "  FOREIGN KEY(user_id) REFERENCES members(id)"
      ");"
    )) != SQLITE_OK) return rc;

    // 8) votes
    if ((rc = exec_sql(db,
      "CREATE TABLE IF NOT EXISTS votes ("
      "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  election_id  INTEGER NOT NULL,"
      "  user_id      INTEGER NOT NULL,"
      "  choice       TEXT    NOT NULL,"
      "  cast_at      DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
      "  FOREIGN KEY(user_id) REFERENCES members(id)"
      ");"
    )) != SQLITE_OK) return rc;

    // --- DEV MODE: seed a known‐good user & code ---
    if (dev_mode) {
        // seed the signup_code
        exec_sql(db,
          "INSERT OR REPLACE INTO settings(key,value) "
          "VALUES('signup_code','devcode123');"
        );

        // the SHA-256 hex of "devcode123"
        const char *dev_hash =
          "e5829d7ee6aaf87f43f5ac851c6a723fef1ad5291f88a1bb7b91a8f660fc8dec";

        sqlite3_stmt *st;
        // insert testuser
        sqlite3_prepare_v2(db,
          "INSERT OR IGNORE INTO members"
          "(username,password_hash,salt,first_name,last_name,role,email,phone_number)"
          " VALUES('testuser',?,'','Test','User','admin','test@local','555-0100');",
          -1, &st, NULL);
        sqlite3_bind_text(st, 1, dev_hash, -1, SQLITE_STATIC);
        sqlite3_step(st);
        sqlite3_finalize(st);

        // find testuser's id
        int uid = -1;
        sqlite3_prepare_v2(db,
          "SELECT id FROM members WHERE username='testuser';",
          -1, &st, NULL);
        if (sqlite3_step(st)==SQLITE_ROW) {
          uid = sqlite3_column_int(st,0);
        }
        sqlite3_finalize(st);

        if (uid>0) {
          // seed one event
          char esql[512];
          snprintf(esql,sizeof(esql),
            "INSERT OR IGNORE INTO events(title,start,end,location,description,created_by)"
            " VALUES('Dev Event','2025-05-15T18:00:00','2025-05-15T20:00:00',"
            "'DevHall','Test event',%d);", uid);
          exec_sql(db, esql);

          // seed one finance
          char fsql[512];
          snprintf(fsql,sizeof(fsql),
            "INSERT OR IGNORE INTO financials(user_id,category,amount,date,description,status)"
            " VALUES(%d,'dues',50.0,'2025-05-10','Dev dues','paid');", uid);
          exec_sql(db, fsql);
        }
    }

    return SQLITE_OK;
}

