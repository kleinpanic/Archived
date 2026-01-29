#ifndef INIT_DB_H
#define INIT_DB_H

#include <sqlite3.h>
#include <stdbool.h>

/**
 * initialize_database
 *
 * Creates all tables (members, signup_requests, settings, sessions,
 * notifications, audit_log, events, financials, votes) if missing,
 * enforces foreign keys, and—if dev_mode is true—seeds a test user
 * and a default signup code.
 *
 * @param db        An open sqlite3 handle.
 * @param dev_mode  True to seed dev data into a separate DB.
 * @return  0 on success, nonzero on error.
 */
int initialize_database(sqlite3 *db, bool dev_mode);

#endif // INIT_DB_H

