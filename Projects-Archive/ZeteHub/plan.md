# Zeta Psi Alpha Pi Fraternity Website Design and Technical Plan

## Overview and Site Map

This project is to create a comprehensive website for **Zeta Psi – Alpha Pi** chapter, with a polished public frontend and a secure members-only backend portal. The site will reflect Zeta Psi’s collegiate brand and colors while providing robust functionality for fraternity operations. Below is a proposed site map and feature breakdown:

- **Public Frontend (Unauthenticated)** – Accessible to anyone:
  - **Home** – Chapter introduction, history, and welcome message.
  - **About Us** – Information about Zeta Psi Alpha Pi (mission, values, maybe the chapter’s founding, notable alumni).
  - **Rush/Join** – Details for prospective members on how to join.
  - **Events** – Public events calendar or upcoming activities open to campus.
  - **Gallery/Media** – Photos or videos of fraternity events.
  - **Contact** – Contact form or contact info for chapter officers or recruitment chair.
  - **Member Login** – Link to sign in to the brotherhood portal (secured backend).

- **Members-Only Backend Portal (Authenticated)** – Accessible only after login:
  - **Dashboard** – Overview of upcoming events, announcements, and quick links.
  - **Member Directory** – List of brothers with profiles (if desired).
  - **Finances** – Dues & fines tracking:
    - View your dues status and any fines.
    - Officers (e.g. Treasurer) can record payments, update fines, and see outstanding balances.
  - **Elections/Voting** – Secure voting system for chapter elections or polls:
    - Create a new vote (restricted to authorized roles, e.g. the President or Election Chair).
    - Cast vote interface for members (one vote per member).
    - Automated tallying of results, with anonymity and integrity.
  - **Calendar** – Full calendar view of all chapter events, meetings, deadlines (internal events viewable only to members, possibly including mandatory events).
  - **Documents** – Repository of important files:
    - Bylaws, meeting minutes, house rules, contact lists, etc., stored in a secure document library.
    - Ability to upload/download documents (with role-based upload permissions).
  - **Admin Tools** – (Restricted to officer roles) User management (add/edit members, assign roles/committees), site settings, etc.

Each section of the members-only portal will be accessible based on roles. For example, only the **President** or designated admins can manage users, only the **Treasurer** can modify finances, etc., while regular members can view relevant info.

## Visual Design and Zeta Psi Branding

The visual design will align with Zeta Psi’s official branding to give a professional yet collegiate feel. Zeta Psi’s official colors are **“Zeta Psi Gold”, “Pure White”, and “Pure Black”**. We will leverage these colors in the site’s color scheme:

- **Color Palette:** A rich **gold** as the primary accent (for headers, highlights, and buttons), balanced by plenty of **white** for backgrounds and **black** for text. This matches Zeta Psi’s colors (gold/white/black). For example, the navigation bar might use Zeta Psi Gold with white or black text for contrast, and headings or call-to-action buttons can also be in gold. Body text will mostly be black on white for readability. We can derive exact color codes from brand guidelines (e.g. a Pantone gold converted to hex, such as an **old gold** tone like `#EAAA00`, and pure white `#FFFFFF`, black `#000000` for web use).

- **Typography & Imagery:** Use clean, legible fonts that convey professionalism. A sans-serif font (e.g. **Arial**, **Helvetica**, or a Google Font like **Roboto**) can give a modern look, while perhaps a serif or a stylized collegiate font for titles could add a classic academic touch. We should include the fraternity’s Greek letters **ΖΨ** (Zeta Psi) prominently in the header/logo. Official logos or crest can be used if available, adhering to any usage guidelines. Images of chapter events, the university campus, and group photos of brothers will be used on the public pages to make the site inviting.

- **Layout and Style:** The frontend will use a **responsive design** to ensure it looks good on desktops, tablets, and phones. We’ll follow modern web design conventions – a clean **header navigation bar**, a hero section on the homepage with an inspiring tagline, content sections with ample whitespace, and a consistent footer. All pages will be mobile-friendly with a collapsible menu on small screens, touch-friendly buttons, and fluid content that resizes gracefully.

- **Collegiate Aesthetic:** The design will tastefully incorporate fraternity symbols and possibly subtle textures or patterns (for example, using the fraternity’s crest as a faint watermark in the background of sections, or using a subtle parchment texture for a classical feel). However, we will keep the overall look modern and not too cluttered. The gold/white/black palette itself provides a classic collegiate contrast. We will ensure adequate contrast and accessibility (e.g., gold text on white might be hard to read, so we’ll mostly use gold as a solid background or accent with white/black text on it or black text on gold).

In summary, the frontend will visually communicate Zeta Psi’s identity while remaining professional and easy to navigate. The use of official colors and logos maintains brand consistency, and a responsive layout ensures the site is accessible from any device.

## Frontend Architecture and Layout

The frontend will be built using **standard web technologies (HTML, CSS, and JavaScript)**, structured for both maintainability and performance. We have two primary parts of the frontend: the public-facing pages and the internal portal pages. 

**Framework / Library:** We can implement the frontend either as a **traditional multi-page website** (with separate HTML files for each section plus some JS for interactivity) or as a **single-page application (SPA)** for the member portal. Given that the public site is mostly informational, a multi-page approach works well there. The member portal, however, involves more complex interactions (voting, forms, etc.), so using a JavaScript framework (such as **React** or **Vue**) or at least modular JS would be beneficial. That said, to keep within the requirement of using just JS/HTML (and considering our backend is not Node.js), we might choose a lighter approach:
- Use standard HTML/CSS for structure and presentation.
- Use JavaScript (possibly with a library like **jQuery** or modern ES6+ vanilla JS) to handle dynamic behaviors and AJAX calls to the backend API.
- (Optionally) Use a front-end toolkit like **Bootstrap** or **Tailwind CSS** to speed up responsive design, ensuring consistency across browsers.

**Responsive Layout:** We’ll use a mobile-first CSS design. This includes a flexible grid or flexbox layout for content sections, and media queries to adjust styling for smaller screens. Navigation will likely collapse into a “hamburger” menu on mobile. We will also ensure images and embedded content scale down nicely on small screens.

**Frontend File Structure:** We can organize the frontend files as follows:
- `index.html`, `about.html`, `rush.html`, etc. for public pages.
- A protected `portal.html` (or an SPA entry point) for the members portal (this might be loaded after login or dynamically served after authentication).
- `styles.css` for global styles (colors, typography, layout). Possibly additional CSS for the portal if its styling is significantly different.
- JavaScript files, e.g. `main.js` for public site interactions (like the contact form or simple effects), and `portal.js` for the logged-in portal functionalities (handling form submissions, fetching data for calendar, etc.).
- Assets like images (logos, photos) in an `assets/` folder.

**Using the Backend API:** The members portal pages will interact with the backend via HTTP requests (likely AJAX calls returning JSON). For instance, when the portal loads, it might fetch the list of events in JSON to populate the calendar, or fetch the user’s dues status to display on the dashboard. We will structure our JavaScript to call specific API endpoints (which we will define in the backend section). 

**Sample Frontend Layout (Code):** Below is a snippet of an example `index.html` showcasing the general structure and usage of Zeta Psi colors, as well as a responsive navigation bar and a content section. This demonstrates the HTML/CSS approach for the public-facing part of the site:

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Zeta Psi Alpha Pi – Home</title>
  <style>
    body {
      margin: 0; padding: 0;
      font-family: Arial, sans-serif;
      background-color: #FFFFFF; /* white background */
      color: #000000;            /* black text for readability */
    }
    header {
      background-color: #EAAA00; /* Zeta Psi Gold */
      padding: 1em;
    }
    header h1 {
      margin: 0;
      color: #FFFFFF;           /* White text on gold */
      text-align: center;
    }
    nav {
      background-color: #333333; /* a dark grey/black nav bar */
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
    }
    nav a {
      color: #FFFFFF;
      padding: 0.75em 1em;
      text-decoration: none;
      font-weight: bold;
    }
    nav a:hover {
      background-color: #EAAA00; /* hover highlight in gold */
      color: #000;
    }
    /* Responsive: stack nav links on small screens */
    @media (max-width: 600px) {
      nav {
        flex-direction: column;
      }
      nav a {
        border-top: 1px solid #555;
      }
    }
    main {
      padding: 2em;
    }
    .hero {
      text-align: center;
      margin-bottom: 2em;
    }
    .hero h2 {
      color: #EAAA00; /* Gold accent for subheading */
      font-size: 1.5em;
    }
    .cta-button {
      display: inline-block;
      background-color: #EAAA00;
      color: #000;
      padding: 0.5em 1em;
      text-decoration: none;
      border-radius: 5px;
      margin-top: 1em;
    }
    .cta-button:hover {
      background-color: #cf8f00; /* a slightly darker gold on hover */
    }
    footer {
      background-color: #333333;
      color: #fff;
      text-align: center;
      padding: 1em;
    }
  </style>
</head>
<body>
  <header>
    <h1>ΖΨ Alpha Pi – Zeta Psi Fraternity</h1>
  </header>
  <nav>
    <a href="index.html">Home</a>
    <a href="about.html">About Us</a>
    <a href="rush.html">Rush</a>
    <a href="events.html">Events</a>
    <a href="contact.html">Contact</a>
    <a href="login.html">Member Login</a>
  </nav>
  <main>
    <section class="hero">
      <h2>Welcome to Zeta Psi – Alpha Pi Chapter</h2>
      <p>Building lifelong bonds of brotherhood since 18xx at XYZ University.</p>
      <a class="cta-button" href="rush.html">Join Us</a>
    </section>
    <section>
      <h3>Our Brotherhood</h3>
      <p>Alpha Pi Chapter of Zeta Psi is a fraternity that prides itself on ... [introductory content] ...</p>
    </section>
    <!-- Additional content sections... -->
  </main>
  <footer>
    &copy; 2025 Zeta Psi Alpha Pi. All rights reserved.
  </footer>
</body>
</html>
```

**Explanation:** In this code, the `<header>` uses a gold background with a white title text (the fraternity name including Greek letters). The navigation bar is styled in a dark color with white links that turn gold on hover – reflecting the fraternity colors in an elegant way. The `.hero` section is a welcome banner with a gold subheading and a call-to-action button styled in gold with black text (reminiscent of Zeta Psi’s palette). CSS media queries ensure that on narrow screens, the navigation links stack vertically for easier tapping. This sample demonstrates the **responsive, branded design** approach.

For the **member portal frontend**, we would have a similar HTML structure but loaded after login. For example, upon successful login, the user might be directed to `portal.html` which includes scripts to fetch and display dynamic data (like the calendar, financial info, etc.). We can use front-end routing or simple tabbed content for sub-sections of the portal. If using a JS framework, we could also implement the portal as a single-page app that pulls data via APIs and updates the view.

**Dynamic Components:** Some parts of the frontend will rely on JavaSc:
- **Calendar:** We can integrate a JavaScript calendar library (such as **FullCalendar**) to display events in a monthly/weekly view. FullCalendar is a popular choice that supports pure JS usage and offers a full-sized interactive calendar widget. This would allow officers to create events (via forms) and members to view them in a nice calendar UI (and possibly even sync or export to their personal calendars if needed).
- **Charts (optional):** If we want to visualize things like dues paid vs pending, we might include a small chart library (like Chart.js) to display graphs on the dashboard.
- **AJAX for interactivity:** The portal pages will use AJAX (XMLHttpRequest or Fetch API) to communicate with backend endpoints for actions like submitting a vote, uploading a document, or updating profile info. This avoids full page reloads and provides a smoother user experience.

With this architecture, the frontend remains **decoupled** from the backend implementation. It communicates via HTTP requests, which is useful for future expansion (e.g., a mobile app could use the same endpoints). The design choices (use of HTML/JS/CSS) ensure that any web developer can easily maintain the frontend without specialized proprietary tools, and the responsive design ensures users can access it from phones or laptops seamlessly.

## Backend Architecture (Engine in C)

The backend of the site will be a **self-contained web server application written in a compiled language (C)**. This approach means we are not using an external web server (like Apache or Node.js); instead, the application itself will listen for HTTP requests and respond accordingly. Building it in a low-level language like C gives us control over performance and resource usage, and we can compile it into a single binary that is easy to deploy on our server hardware.

**Technology Stack:**
- **Language:** C (for maximal control and efficiency). Alternatively, one could consider C++ or Rust for more modern safety features, but C is explicitly allowed and well-understood for system-level programming.
- **HTTP Server:** We will implement a lightweight HTTP server within the application. Rather than coding an entire HTTP parser from scratch (which is error-prone), we can use a small embeddable web server library such as **GNU libmicrohttpd** or **CivetWeb** to handle HTTP protocol details. CivetWeb, for example, is an embeddable C/C++ web server library that can be compiled into our application, enabling it to run as a standalone web server with support for features like SSL and request routing. Using such a library lets us focus on our application logic (authentication, database, etc.) while it handles socket listening, HTTP parsing, and connection management in the background. The result is a **single executable** that runs the web service (CivetWeb even allows a stand-alone mode where no installation is required, just run the binary and it serves content).

- **Database & Storage:** We need to store persistent data for users, payments, votes, events, and documents. For simplicity and ease of deployment, we can use **SQLite** (an embedded SQL database that stores data in a single file) for structured data. SQLite can be integrated with C easily and doesn’t require running a separate database server process. It will handle tables for members, transactions, votes, etc. For documents (file storage), we can store files on disk (in a secure folder) and save the file paths and metadata in the database. If the chapter’s data grows or if multiple chapters were to be served by one system in the future, we could switch to a client-server DB (like PostgreSQL or MySQL), but for one chapter’s needs (likely a few hundred records at most in each table), SQLite is very sufficient and is ACID-compliant for safe transactions.

**Backend Features Implementation:**

1. **Member Authentication & Roles:**  
   - The backend will expose endpoints for login (and logout). We’ll store members’ usernames, hashed passwords, roles, and committee info in a `members` table. Passwords will be stored securely using a strong hash (e.g., **bcrypt** or **SHA-256 with salt**). On login requests, the backend will verify the password hash. If valid, a session is established – likely via a **session cookie** (a unique session ID stored in memory or database) or a token. Since we are not using a full web framework, we might implement a simple session management: e.g., generate a random token on login, store it server-side mapped to the user, and set it as a cookie in the HTTP response so subsequent requests include it for authentication.
   - Roles (President, Treasurer, Chair, etc.) and committee membership will be fields in the user record (or a linked table). The backend will enforce authorization checks based on role: for example, only users with the President role can access the “create election” API, only Treasurer can post a new fine or mark a payment, etc. We’ll define role constants and check privileges in each relevant request handler.
   - We also include a registration or admin interface to add new members. Likely, only an admin (President or Secretary) can create new user accounts or reset passwords. This can be an admin page in the portal or even done via the database by an admin for simplicity.

2. **Payment Tracking (Dues & Fines):**  
   - We’ll have a `payments` (or `transactions`) table that logs charges (dues, fines) and payments made. Key fields might include member_id, type (due or fine, or payment), amount, date, description, and a status (paid/unpaid).
   - The **Treasurer** or an appointed officer can use the portal to record when a member pays their dues or if a fine is issued. For example, if a member owes \$100 dues each semester, the Treasurer could mark that as a charge, and when paid, mark it paid or record a payment entry.
   - Members logging in can view their own financial summary: total due, fines outstanding, payments made, etc. The backend would have an API like `GET /api/finances/me` to retrieve the logged-in user’s financial records. It might also allow downloading receipts or invoices (we could generate PDFs on the fly if needed, or just display the info).
   - Because money is involved, **security is paramount**: all such data is behind login. If we integrate actual online payments (e.g., via PayPal or Stripe), we would do so through their secure APIs – likely by redirecting to PayPal or using a client-side integration – so that **our server never stores sensitive credit card info**. In many cases, chapters might handle dues offline or via established payment portals, so our system might focus on tracking rather than processing payments.

3. **Secure Election Voting System:**  
   - The backend will provide functionality to create and conduct elections. We can have tables like `elections` (with fields: id, title, description, status open/closed, etc.) and `candidates` (or options) for each election, and a `votes` table to record votes.
   - When an election is created (by an officer), the system will record all eligible voters (likely all active members) and ensure each member can vote at most once. The voting endpoint (e.g., `POST /api/vote`) will check the user’s session and ensure they haven’t voted already for that election (we might track in the votes table by user id and election id).
   - To keep votes secret, we can design it such that individual votes are not tied to a user identity in an obvious way. For instance, once a user votes, we mark them as voted and store their vote, but we might not store their user ID with the vote (store only an anonymous token or just mark in a separate table that user X has voted, without linking to which option they chose). This way, when tallying, we get counts per candidate but cannot easily reconstruct who voted for whom – ensuring anonymity.
   - The election creation could allow options like secret vs open ballot, but likely all chapter elections should be secret. We will also enforce that once the election is closed, results can be viewed by authorized roles. The data integrity can be protected by database constraints (unique vote per user per election). We might also include basic cryptographic checks for tampering (for example, hashing the votes or using a simple checksum to detect if any vote record was altered).
   - **Example flow:** President creates an election for “New Treasurer 2025” with candidates A, B, and C. The backend sets election status to open. Members log in, the frontend calls `GET /api/elections/current` to get the list of open elections and candidates, then each member submits their vote via `POST /api/vote` with their choice. The backend records it and prevents further voting from that account. After a deadline, the President closes the election (perhaps via `POST /api/elections/{id}/close`), and results are tallied (the backend can compute totals from the votes table). The results could be stored or just computed on the fly when an authorized user requests them. Security considerations include making sure the endpoint cannot be spammed (we may lock it after one vote, and perhaps use a CAPTCHA if we worried about bots – though this is an internal system so unlikely an issue). Additionally, all communication is over HTTPS so votes aren’t intercepted in transit.

4. **Calendar of Events:**  
   - We will maintain an `events` table (with fields: id, title, description, date/time, location, visibility/public_or_private, etc.). Chapter officers can add or edit events (perhaps certain roles like Secretary or Social Chair can do this). 
   - The backend provides `GET /api/events` which can return events in JSON. For public pages, we might expose only public events (flagged as such) so that the public “Events” page on the website can list things like philanthropic events open to campus. For members, the API would return all events including chapter meetings, retreats, etc.
   - If using a library like FullCalendar on the frontend, we’ll format the JSON according to what it expects (likely an array of event objects with `title`, `start` date, etc. fields). The calendar API might support filtering by date range (e.g., the front-end couldContinuing the Backend Architecture and Features:

4. **Calendar of Events (continued):**  
   The backend’s calendar API will allow:
   - **Create Event:** `POST /api/events` (restricted to roles like President, Social Chair, or any designated role). The request would include event details (title, datetime, location, description, visibility). The backend will validate and insert into the `events` table.
   - **List Events:** `GET /api/events` for members (returns all events or those the user has access to; e.g., if we mark some events as “officers only” or “pledges only”, we’d filter based on user’s group). We might also have `GET /api/events/public` which returns only publicly visible events for the public site’s event page.
   - **Edit Event:** `PUT /api/events/{id}` and **Delete Event:** `DELETE /api/events/{id}` (again restricted to event creators or admins).
   - The response will be JSON data that can easily be fed into the frontend’s calendar component. For example, an event might be returned as:
     ```json
     {
       "id": 42,
       "title": "Brotherhood Retreat",
       "start": "2025-09-10T18:00:00",
       "end": "2025-09-12T12:00:00",
       "location": "Camp Wilderness",
       "description": "Annual fall retreat for bonding and planning."
     }
     ```
     If using FullCalendar or similar, we might adjust field names to what the library expects (like `start` and `end` as above).
   - This approach decouples the calendar rendering (frontend) from data management (backend). It also opens up the possibility of syncing with external calendars if needed in the future (e.g., export events to Google Calendar by providing an iCal feed endpoint).

5. **Document Storage and Reading:**  
   - The system will provide a secure file repository for members to access important documents (such as chapter bylaws, meeting minutes, guides, etc.). These could be PDFs, Word docs, etc.
   - We’ll have a `documents` table storing metadata: document id, title, filename, upload date, and possibly who uploaded it and an access level (some documents might be officer-only, others for all brothers).
   - Uploaded files will be stored on the server’s filesystem in a designated folder (outside of the web root, since our C server will explicitly handle sending these files; we won’t rely on static file serving from the OS web server). The backend will likely store files in a directory like `uploads/documents/` with filenames prefixed or suffixed by the document ID to avoid name collisions.
   - **Upload API:** `POST /api/documents` to upload a new file. This is a bit complex since file uploads (especially via an AJAX call) need multipart form data handling. Libraries like libmicrohttpd provide hooks for processing file uploads. We must ensure only authorized users (e.g., President, Secretary, or a special Document Chair role) can upload or delete documents. During upload, the backend will receive the file stream, save it to disk, and create a DB entry. It should also validate file type/size to prevent someone uploading a malicious executable or something extremely large.
   - **Download API:** `GET /api/documents/{id}` to fetch a document. The backend will verify the user has access, then read the file from disk and send it with the correct MIME type. We will also set appropriate headers to force download if needed. This endpoint should stream the file to handle large documents without loading everything into memory at once.
   - Optionally, we could implement a simple UI to preview documents (for PDFs, one could embed a PDF viewer or rely on browser to preview, for images display in browser, etc.). But a straightforward approach is to let members download the files.
   - **Security:** Document access requires login. We will not expose the uploads directory publicly; all access goes through the C backend which checks permissions. We also should ensure that file paths are sanitized (to prevent directory traversal attacks). Each file could be named on disk with its document ID plus extension to avoid any user-controlled path elements.

**Backend Structure and Code Organization:**

Inside our C project, we’ll structure the code into modules:
- **HTTP Server Initialization** – code to start the web server (using libmicrohttpd or CivetWeb). This includes setting up listening port (e.g., 80 for HTTP, and possibly 443 for HTTPS if we handle TLS – though we might also run behind a reverse proxy for SSL).
- **Routing / Request Handling** – a function that libmicrohttpd calls for each request, where we inspect the URL and HTTP method to dispatch to appropriate handler function. For example, if a request comes in for `/api/login` with POST, we call the `handle_login` function; if `/api/events` with GET, call `handle_get_events`; if `/api/vote` with POST, call `handle_vote`, etc. We can implement a simple router by matching URL prefixes.
- **Handlers** – Each feature (auth, events, finances, etc.) will have its own handler function(s). These will parse inputs (e.g., JSON body or form fields), interact with the database, and produce an output (JSON, HTML, or file).
- **Database Access** – using an SQLite C library (the C API for SQLite). We might write a small abstraction layer, or use direct SQL statements. For maintainability, separate the SQL queries into clearly named functions (e.g., `db_get_user_by_username`, `db_insert_event`, `db_get_events_for_user`, etc.).
- **Security Utilities** – functions for hashing passwords (we could use a library like OpenSSL’s EVP for SHA-256 or a bcrypt library for C), and random token generation for sessions. Also, input validation routines to sanitize and check lengths on incoming data to avoid injection attacks (though using parameterized SQL queries with SQLite’s API is better than string concatenation).

**Sample Backend Code Snippet (Pseudo-C Code for illustration):**

Below is a simplified conceptual example using GNU libmicrohttpd to handle a couple of routes. This is for demonstration – a real implementation would be more robust and modular:

```c
#include <microhttpd.h>
#include <sqlite3.h>
#include <string.h>
#include <stdio.h>

#define PORT 8080

// Global (or passed around) database handle and maybe an in-memory session store
sqlite3 *db;

static int send_json_response(struct MHD_Connection *conn, const char *json, unsigned int status) {
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(json), (void*)json, MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(response, "Content-Type", "application/json");
    int ret = MHD_queue_response(conn, status, response);
    MHD_destroy_response(response);
    return ret;
}

// A helper to parse login credentials from a request (for brevity, assuming form-encoded body)
static int handle_login(struct MHD_Connection *conn, const char *username, const char *password) {
    // Query DB for user
    const char *sql = "SELECT id, password_hash, role FROM members WHERE username = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return send_json_response(conn, "{\"error\":\"DB error\"}", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    int result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
        // No such user
        sqlite3_finalize(stmt);
        return send_json_response(conn, "{\"error\":\"Invalid credentials\"}", MHD_HTTP_UNAUTHORIZED);
    }
    const unsigned char *stored_hash = sqlite3_column_text(stmt, 1);
    // TODO: verify the provided password against stored_hash (e.g., bcrypt or SHA256 check)
    bool pw_ok = verify_password(password, (const char*)stored_hash);
    if (!pw_ok) {
        sqlite3_finalize(stmt);
        return send_json_response(conn, "{\"error\":\"Invalid credentials\"}", MHD_HTTP_UNAUTHORIZED);
    }
    int user_id = sqlite3_column_int(stmt, 0);
    const unsigned char *role = sqlite3_column_text(stmt, 2);
    sqlite3_finalize(stmt);
    // Generate a session token and store it (in memory or DB table)
    char token[64];
    generate_random_token(token, sizeof(token));
    store_session(token, user_id, (const char*)role);
    // Set token as cookie in response
    char header_value[128];
    snprintf(header_value, sizeof(header_value), "session=%s; HttpOnly", token);
    struct MHD_Response *response = MHD_create_response_from_buffer(0, "", MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, "Set-Cookie", header_value);
    MHD_add_response_header(response, "Content-Type", "application/json");
    int ret = MHD_queue_response(conn, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

static int handle_get_events(struct MHD_Connection *conn, int user_id) {
    // Query events visible to this user (for simplicity, assume all events visible to all logged in members)
    const char *sql = "SELECT id, title, start, end, location, description FROM events";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    // Construct JSON array of events
    char json[4096] = "{\"events\":[";
    bool first = true;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!first) strcat(json, ",");
        first = false;
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *title = sqlite3_column_text(stmt, 1);
        const unsigned char *start = sqlite3_column_text(stmt, 2);
        const unsigned char *end = sqlite3_column_text(stmt, 3);
        const unsigned char *location = sqlite3_column_text(stmt, 4);
        const unsigned char *desc = sqlite3_column_text(stmt, 5);
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

// Dispatcher for incoming HTTP requests
static enum MHD_Result request_handler(void *cls, struct MHD_Connection *conn,
                                       const char *url, const char *method,
                                       const char *version, const char *upload_data,
                                       size_t *upload_data_size, void **con_cls) {
    // Check URL and method to route appropriately
    if (strcmp(url, "/api/login") == 0 && strcmp(method, "POST") == 0) {
        // For brevity, using simple query params (in real case, parse body)
        const char *user = MHD_lookup_connection_value(conn, MHD_POSTDATA_KIND, "username");
        const char *pass = MHD_lookup_connection_value(conn, MHD_POSTDATA_KIND, "password");
        if (!user || !pass) {
            return send_json_response(conn, "{\"error\":\"Missing fields\"}", MHD_HTTP_BAD_REQUEST);
        }
        return handle_login(conn, user, pass);
    }
    if (strcmp(url, "/api/events") == 0 && strcmp(method, "GET") == 0) {
        // Verify session (cookie)
        const char *token = MHD_lookup_connection_value(conn, MHD_COOKIE_KIND, "session");
        int user_id = validate_session(token);
        if (user_id <= 0) {
            return send_json_response(conn, "{\"error\":\"Not authenticated\"}", MHD_HTTP_UNAUTHORIZED);
        }
        return handle_get_events(conn, user_id);
    }
    // ... other routes like /api/vote, /api/finances, etc.
    // If no route matches:
    return send_json_response(conn, "{\"error\":\"Not found\"}", MHD_HTTP_NOT_FOUND);
}

int main() {
    // Open database
    if (sqlite3_open("fraternity.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database\n");
        return 1;
    }
    // Possibly set up DB with tables if not exists.
    // Start HTTP server
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD,
                               PORT, NULL, NULL, &request_handler, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "Failed to start server\n");
        return 1;
    }
    printf("Server started on port %d\n", PORT);
    getchar(); // keep running until Enter is pressed (for demo purposes)
    MHD_stop_daemon(daemon);
    sqlite3_close(db);
    return 0;
}
```

This code outlines how the C backend could operate:
- It uses **libmicrohttpd** to start an HTTP server on a specified port.
- `request_handler` function examines the request URL and method to route to the appropriate handler.
- The `handle_login` function queries the SQLite database for the user and verifies the password (the actual password verification logic would use hashing). If successful, it creates a session token, stores it (the `store_session` could be as simple as an in-memory map or a database table of active sessions), and returns an HTTP response with a **Set-Cookie header** to set the session token in the user’s browser.
- The `handle_get_events` function fetches events from the database and constructs a JSON string to return. (In a real scenario, we’d want to use a proper JSON library or careful string construction to avoid errors or security issues with special characters; but this pseudo-example demonstrates the concept).
- Each API endpoint first validates the user’s session (except login). For example, to get events, we look up the session cookie, verify it (via `validate_session` which checks if the token is in our session store and not expired), and retrieve the associated user id.
- We would implement similar handlers for `/api/vote` (reading the vote choice from request, checking user hasn’t voted, then inserting a vote record), `/api/finances` (returning the user’s own financial records, or if an officer, maybe returning summaries of all members), etc., always with permission checks.

**Security Considerations in Backend:**
- **SQL Injection:** We will use prepared statements (as shown with `sqlite3_bind_text`) instead of string concatenation for queries. This prevents injection attacks.
- **Password Security:** As mentioned, always store hashed passwords with a salt. We can use an established library for bcrypt or Argon2 in C for better security than plain SHA256. The verify_password function should use the same hash algorithm that was used to store the password.
- **Session Security:** Tokens should be long, random, and stored in an HTTP-only cookie so that JavaScript cannot read them (mitigating XSS risk). We should also consider using the Secure flag on cookies (if site is HTTPS) to prevent them from being sent over plain HTTP. We should have an expiration for sessions (e.g., auto logout after X hours or days of inactivity).
- **HTTPS:** While developing on localhost or within a LAN can be HTTP, for deployment we absolutely want to use HTTPS, especially since sensitive data (passwords, votes) are transmitted. We can either add TLS support in our C server (libraries like CivetWeb or OpenSSL can integrate) or more simply deploy behind a reverse proxy like **Nginx** or **Apache** that terminates SSL and forwards to our application on a local port. Given the "self-contained" requirement, an easier approach might be to use **CivetWeb** since it has built-in SSL support and can serve HTTPS directly by providing the certificate files.
- **Input Validation:** For any data that goes into the database (user profiles, event details, document metadata), we will validate length (to prevent extremely large inputs or buffer overflow in C code), and possibly content (e.g., filenames should not contain path separators).
- **Authorization Checks:** As noted, every API must verify not just authentication but also that the user has the right role for the action. If a non-Treasurer tries to mark a payment, the system should reject it with a 403 Forbidden. We can maintain a simple mapping of roles to allowed endpoints or check role within the handler logic.

**Scalability Considerations:**
- Even though we are using C for performance, we must consider the concurrency model. libmicrohttpd as used in the example above is using an internal polling thread (non-blocking mode) which is suitable for moderate load. It can handle multiple requests in parallel by using select/poll under the hood. We can configure it to have a thread pool if needed or use an event loop. CivetWeb similarly can handle many connections (it’s used in embedded devices often).
- The site is for a fraternity, so the traffic is expected to be relatively low (dozens to a few hundred users, and primarily within the chapter). A single instance on a Dell PowerEdge server (which is typically quite powerful) will easily handle this. However, if needed, we could run multiple instances (behind a load balancer) since our design is stateless except the DB. With sessions in the DB or a shared store, any instance can handle any request. Containerization (discussed below) also helps scale out by deploying multiple containers if needed.
- SQLite is not designed for high-concurrency writes, but it will be fine for our use (mostly reading, few writes for an election or payment logging). If down the line the site needs to scale to a multi-chapter system with heavier load, migrating to a client-server DB like PostgreSQL would be wise. Our code should abstract the DB operations enough that this is feasible without total rewrite.
- We should also consider caching frequently accessed data (maybe cache the events list or the member directory in memory to reduce DB hits). Because the dataset is small, this might not be necessary, but is an option for optimization.

## Deployment and Containerization Strategy

Deploying on local hardware (like a **Dell PowerEdge** server) suggests we will run this on a dedicated machine, possibly without a full cloud stack. We want our application to be **easily deployable as a container or a standalone binary**:
- **Containerization with Docker:** We can create a Docker image that contains our compiled C backend and all required runtime dependencies (which is minimal, basically the C runtime and any libraries we linked dynamically). If we link everything statically (including libmicrohttpd, SQLite, etc.), we could even have a scratch or alpine-based container with just the binary and database file. This container can be run on the PowerEdge (assuming it runs a modern OS with Docker support, which most do). The container encapsulates the app, making it easy to start, stop, and manage (and if needed, to deploy multiple instances or roll out updates).
  - The Dockerfile would start from an Alpine Linux base, install build tools, compile our code, then copy the binary into a smaller base image.
  - We’ll expose the necessary port (e.g., 80 or 8080) and possibly mount a volume for persistent storage (for the SQLite database file and the documents directory). This way, updating the container doesn’t wipe out our data.
  - Example Docker deployment steps: `docker build -t zetapsi-website .` then `docker run -d -p 80:80 -v /opt/zeta_data:/app/data zetapsi-website`. The volume mapping assumes our app writes the database and documents to `/app/data` inside the container, and we map it to a host directory for persistence.
- **Self-contained Binary on Host:** Alternatively, we could simply compile the application as a Linux binary and run it directly on the server. This would require manually setting it up as a service (systemd service or similar) to start on boot. We’d have to manage our own logging, and ensure required libraries are installed on the server (if we dynamically link). The advantage is slightly less overhead than Docker, but the disadvantage is less isolation.
- The container approach is more modern and “infrastructure-as-code” friendly, so I recommend that.

**Reverse Proxy and SSL:** 
- If using Docker, one might also deploy a separate Nginx container configured with the certificates for the site’s domain (e.g., alphapi.zetapsi.org) that proxies to the C app. This is a common pattern to handle HTTPS and domain virtual hosting. But it’s optional if we embed HTTPS in the app. 
- In either case, we need to obtain an SSL certificate (from Let’s Encrypt or the university if they provide one for student organizations). Let’s Encrypt could be used via a certbot on the host or an automated companion container.

**Scalability in Deployment:** 
- For now, one instance on one server is enough. But if we wanted to scale, Docker containers could be orchestrated (using docker-compose or Kubernetes) to run multiple instances behind a load balancer. This likely won’t be necessary for a chapter website, but the design doesn’t preclude it.
- Running on a PowerEdge suggests we might have ample resources, but also we need to think about maintenance: The container can be configured to auto-restart on failure (via Docker’s restart policy), which adds reliability.

**Backup and Maintenance:** 
- Because data is stored in a local SQLite and filesystem, we should set up backups (maybe a cron job to copy the database file and documents to some offsite location or at least another drive).
- Log files from the app (we should have the C app log important events to stdout or a file) should be rotated and monitored.
- Containerization helps here too: one could use a volume for logs or use Docker logging drivers to forward logs to a central system.

## Security and Access Considerations

Security is woven throughout the above sections, but to summarize and add:
- **Authentication Security:** Use strong password policies, hashed storage. Possibly implement two-factor authentication (2FA) if desired for important roles (maybe overkill for a frat site, but an interesting future addition).
- **Authorization:** We will explicitly code the allowed actions per role. For instance, an enum or set of constants for roles and a function `canPerform(user, ACTION)` to check permissions. This avoids scattering role strings around and centralizes the logic.
- **Data Encryption:** If the server is handling any particularly sensitive data (e.g., personal addresses, etc.), we could encrypt those fields in the database. But likely not needed beyond standard practice.
- **Input Sanitization:** Always treat user input as untrusted. That includes not only form fields but also uploaded files. We will only allow certain file types for documents (pdf, docx, maybe images). We will scan filenames, and possibly even scan file content for viruses using a tool if we want to be thorough (could integrate ClamAV in the pipeline).
- **Sessions and CSRF:** We should consider CSRF (Cross-Site Request Forgery) protection. Using same-site cookies (set the cookie attribute `SameSite=Lax` or `Strict`) can mitigate CSRF by not sending cookies on cross-origin requests. Also, since our API is not supposed to be open to other sites, this might be sufficient. Alternatively, use anti-CSRF tokens in forms (which our JS can include).
- **XSS:** Make sure any data rendered on the frontend that originated from users is properly escaped. For example, if we ever display a user’s input (like profile info, or event descriptions which could have been input by officers), we should strip or escape HTML. The backend can do this or the frontend when injecting into the DOM.
- **Server Hardening:** Since this is a custom server, ensure it only listens on needed ports, runs as a non-root user if possible, and is firewalled from unnecessary access. The simpler the service (one binary), the smaller the attack surface compared to a full LAMP stack, but we must still be vigilant.
- **Testing:** Before going live, we would test with tools or checklists for OWASP Top 10 vulnerabilities to ensure we didn’t miss anything glaring like open redirects, verbose error messages leaking info, etc.

## Future Expansion and Integration

The plan should allow for growth. Some future expansions could be:
- **Mobile App Integration:** Perhaps down the line, the fraternity wants a native mobile app. Since our backend is essentially a RESTful API (even if not fully REST purist, it serves JSON and accepts JSON), a mobile app (Android/iOS) could be built to log in and use the same API to retrieve data. We’d then just need to ensure CORS or API authentication tokens to allow the app to communicate. We might add JWT (JSON Web Token) support or similar for mobile clients instead of cookie-based sessions. This is feasible with our architecture, as the logic is all in the backend and not tied to the web frontend.
- **University Single Sign-On:** If the school offers a single sign-on system (and if desired), we could integrate that (for example, allow logging in with university credentials via SAML or OAuth). That’s a complex feature but our modular approach to auth could incorporate it by adding an alternate authentication endpoint or tying into the session creation with an external identity provider.
- **Payment Processing Integration:** Right now, we plan to track payments manually. In future, we might integrate a payment API so members can pay dues online. For example, integrate Stripe Checkout or PayPal. This would involve front-end changes (to include a payment button/form that goes to the payment gateway) and back-end webhook handling (e.g., when Stripe confirms payment, it calls our webhook endpoint and we then mark the dues as paid in our database). We’d design our data structures now with maybe a field to indicate a payment transaction ID or method to ease this later.
- **Additional Features:** Perhaps add a **forum or discussion board** for brothers, or a **messaging system** (though most might use GroupMe/Slack etc., but an internal message board could be nice for record-keeping). Another idea is **attendance tracking** (for events and meetings, maybe via the app). Our system could be extended with such modules.
- **Multi-Chapter Support:** If this system is really good, maybe other chapters of Zeta Psi want to use it. We could generalize the platform to support multiple chapters (each with their own data, possibly in one database with chapter_id separating, or separate DBs). We’d then rebrand accordingly. The initial design is chapter-specific, but by abstracting some things, we could expand it.

- **Scaling and Cloud:** If at some point hosting on a local server is not ideal (power, maintenance issues), the containerized app could be moved to a cloud service (like AWS, DigitalOcean, etc.). The container can run on any Linux VM or as part of Kubernetes. This would also make it easier to integrate managed DB services if needed.

Lastly, maintainability is crucial. Future developers (perhaps new brothers in the chapter who take over the project) should be able to read documentation and extend the code. We will write clear README and inline comments, and perhaps use an API-first approach (document the JSON endpoints). Also, using widely-known libraries (like libmicrohttpd or CivetWeb, SQLite) means others can find community support if they need to modify the system.

## Sample Code and Proof-of-Concept Implementation

Below, we provide a couple of sample code files – one for the **frontend (HTML/JS)** and one for the **backend (C)** – as a proof of concept to illustrate some key functionality. These are simplified due to space, but they show how things might be implemented:

**1. Frontend Sample (Login Page + Dashboard HTML/JS):**

```html
<!-- File: login.html (public page for member login) -->
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Member Login – Zeta Psi Alpha Pi</title>
  <link rel="stylesheet" href="styles.css"> <!-- Assume styles.css has base styles -->
  <style>
    /* Additional styles specific to login page */
    .login-container { max-width: 400px; margin: 10% auto; padding: 2em; border: 1px solid #ccc; border-radius: 5px; }
    .login-container h2 { text-align: center; margin-bottom: 1em; }
    .login-container label { display: block; margin: 0.5em 0 0.2em; }
    .login-container input { width: 100%; padding: 0.5em; margin-bottom: 1em; }
    .login-container button { width: 100%; padding: 0.5em; background: #EAAA00; border: none; font-size: 1em; cursor: pointer; }
    .login-container button:hover { background: #cf8f00; }
    .error-msg { color: red; text-align: center; }
  </style>
</head>
<body>
  <div class="login-container">
    <h2>Member Login</h2>
    <div class="error-msg" id="errorMsg"></div>
    <label for="username">Username</label>
    <input type="text" id="username" placeholder="Username" required />
    <label for="password">Password</label>
    <input type="password" id="password" placeholder="Password" required />
    <button onclick="login()">Login</button>
  </div>

  <script>
    function login() {
      const username = document.getElementById('username').value;
      const password = document.getElementById('password').value;
      document.getElementById('errorMsg').innerText = "";
      // Basic validation
      if (!username || !password) {
        document.getElementById('errorMsg').innerText = "Please enter username and password.";
        return;
      }
      // Prepare data for API call
      const formData = new URLSearchParams();
      formData.append('username', username);
      formData.append('password', password);
      // Send AJAX request to login API
      fetch('/api/login', { method: 'POST', body: formData })
        .then(response => {
          if (response.status === 200) {
            // Login successful
            window.location.href = 'portal.html'; // redirect to members portal
          } else {
            return response.json().then(data => { throw new Error(data.error || 'Login failed'); });
          }
        })
        .catch(err => {
          document.getElementById('errorMsg').innerText = err.message;
        });
    }
  </script>
</body>
</html>
```

```html
<!-- File: portal.html (simplified members-only portal page) -->
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Brotherhood Portal – Zeta Psi Alpha Pi</title>
  <link rel="stylesheet" href="styles.css">
  <style>
    /* Basic layout for portal: a sidebar and content area */
    body { display: flex; min-height: 100vh; margin: 0; }
    nav#sidebar { width: 200px; background: #333; color: #fff; flex-shrink: 0; }
    nav#sidebar ul { list-style: none; padding: 0; margin: 0; }
    nav#sidebar li { border-bottom: 1px solid #444; }
    nav#sidebar a { color: #fff; text-decoration: none; display: block; padding: 1em; }
    nav#sidebar a:hover { background: #444; }
    main#content { flex: 1; padding: 1em; }
    .section { margin-bottom: 2em; }
    .section h3 { border-bottom: 2px solid #EAAA00; color: #EAAA00; }
    table { width: 100%; border-collapse: collapse; margin-top: 0.5em; }
    table, th, td { border: 1px solid #ccc; }
    th, td { padding: 0.5em; text-align: left; }
  </style>
</head>
<body>
  <!-- Sidebar Navigation for portal -->
  <nav id="sidebar">
    <ul>
      <li><a href="#" onclick="showSection('dashboard')">Dashboard</a></li>
      <li><a href="#" onclick="showSection('events')">Events Calendar</a></li>
      <li><a href="#" onclick="showSection('finances')">My Finances</a></li>
      <li><a href="#" onclick="showSection('documents')">Documents</a></li>
      <li><a href="#" onclick="logout()">Logout</a></li>
    </ul>
  </nav>

  <!-- Main content sections -->
  <main id="content">
    <section id="dashboard" class="section">
      <h3>Dashboard</h3>
      <p>Welcome, <span id="welcomeName">Brother</span>!</p>
      <p>Upcoming event: <span id="nextEvent">Loading...</span></p>
    </section>
    <section id="events" class="section" style="display:none;">
      <h3>Events Calendar</h3>
      <!-- Simplified: we'll just list events here for now -->
      <div id="eventsList">Loading events...</div>
    </section>
    <section id="finances" class="section" style="display:none;">
      <h3>My Finances</h3>
      <div id="financeStatus">Loading financial info...</div>
    </section>
    <section id="documents" class="section" style="display:none;">
      <h3>Documents</h3>
      <ul id="docList">Loading documents...</ul>
    </section>
  </main>

  <script>
    // On page load, fetch initial data for dashboard (like user name and next event)
    document.addEventListener('DOMContentLoaded', () => {
      fetch('/api/user/me')
        .then(res => res.json())
        .then(data => {
          document.getElementById('welcomeName').innerText = data.firstName || 'Brother';
        });
      fetch('/api/events?limit=1')
        .then(res => res.json())
        .then(data => {
          if (data.events && data.events.length > 0) {
            const e = data.events[0];
            document.getElementById('nextEvent').innerText = `${e.title} on ${new Date(e.start).toLocaleDateString()}`;
          } else {
            document.getElementById('nextEvent').innerText = 'No upcoming events.';
          }
        });
    });
    // Navigation functions
    function showSection(sec) {
      ['dashboard','events','finances','documents'].forEach(s => {
        document.getElementById(s).style.display = (s === sec ? 'block' : 'none');
      });
      if (sec === 'events') loadEvents();
      if (sec === 'finances') loadFinances();
      if (sec === 'documents') loadDocuments();
    }
    function logout() {
      // Clear session by calling a logout API or simply redirect to login (session cookie will die on browser close if not persistent)
      // Let's call a logout API that clears server session.
      fetch('/api/logout', { method: 'POST' }).finally(() => {
        window.location.href = 'login.html';
      });
    }
    // Load events from API
    function loadEvents() {
      fetch('/api/events')
        .then(res => res.json())
        .then(data => {
          if (!data.events) {
            document.getElementById('eventsList').innerText = 'Error loading events.';
            return;
          }
          let html = '<table><tr><th>Date</th><th>Event</th><th>Location</th></tr>';
          data.events.forEach(ev => {
            const dateStr = new Date(ev.start).toLocaleDateString();
            html += `<tr><td>${dateStr}</td><td>${ev.title}</td><td>${ev.location}</td></tr>`;
          });
          html += '</table>';
          document.getElementById('eventsList').innerHTML = html;
        });
    }
    // Load finances from API
    function loadFinances() {
      fetch('/api/finances/me')
        .then(res => res.json())
        .then(data => {
          if (data.error) {
            document.getElementById('financeStatus').innerText = 'Error: ' + data.error;
            return;
          }
          // Suppose API returns something like { duesOwed: 200, duesPaid: 100, fines: [ ... ] }
          let text = `Dues Owed: $${data.duesOwed}, Paid: $${data.duesPaid}. Outstanding Balance: $${(data.duesOwed - data.duesPaid)}`;
          if (data.fines && data.fines.length > 0) {
            text += "\nFines:\n";
            data.fines.forEach(f => {
              text += ` - $${f.amount} for ${f.reason} (${f.status})\n`;
            });
          }
          document.getElementById('financeStatus').innerText = text;
        });
    }
    // Load documents from API
    function loadDocuments() {
      fetch('/api/documents')
        .then(res => res.json())
        .then(data => {
          if (!data.documents) {
            document.getElementById('docList').innerText = 'Error loading documents.';
            return;
          }
          let listHtml = '';
          data.documents.forEach(doc => {
            listHtml += `<li><a href="/api/documents/${doc.id}" target="_blank">${doc.title}</a></li>`;
          });
          document.getElementById('docList').innerHTML = listHtml;
        });
    }
  </script>
</body>
</html>
```

**Explanation (Frontend POC):**  
- The `login.html` provides a simple login form and uses the Fetch API to send credentials to `/api/login`. On success (HTTP 200), it redirects to `portal.html`. On failure, it displays an error. We use an `URLSearchParams` to encode form data for simplicity (libmicrohttpd can parse that as shown in our C code).
- The `portal.html` has a sidebar for navigation and sections for dashboard, events, finances, and documents. It loads basic info when opened: the user’s name and the next event. Navigating to a section triggers a function that fetches the relevant data from the backend and populates the section. 
- Notably, for documents, it expects `GET /api/documents` to return a list of documents (with id and title), and then it creates links that point to `/api/documents/{id}` which will prompt download or viewing of the file. We set `target="_blank"` so it opens in a new tab (for PDFs that might just open in browser).
- The `logout()` function calls an API to invalidate the session (or we could just delete the cookie, but better to also tell server to invalidate). After that, it goes back to login page.
- The frontend code is kept straightforward without any frameworks, which is fine for a moderate complexity app. As it grows, one might introduce more structure or even convert parts to use a framework, but given the audience and maintainers (college students, possibly with varying levels of web dev expertise), sticking to simple, clear JavaScript is a reasonable choice.

**2. Backend Sample (C Code for a minimal server with a couple of endpoints):**

The snippet previously provided in the explanation is essentially the sample code for backend. For brevity, we won't repeat it verbatim. But as a quick recap of what a **functional proof-of-concept in C** might look like:
- A `main()` function that sets up an HTTP server on a port and connects to SQLite.
- A callback for requests that handles at least login and a simple GET (like events or a hello world).
- Using libmicrohttpd (a very small-footprint HTTP server library) to avoid dealing with raw sockets directly.
- The code would be compiled into an executable (say, `frat_server`) which we can run. We can test it by running the binary and using `curl` to simulate login or by opening the `login.html` in a browser (if served by the same server or via a simple file serve) and trying to login.

**Proof-of-Concept Test:**
We could compile and run this on a development machine, then:
- Use a tool like `curl`:
  - `curl -X POST -d "username=test&password=wrong" http://localhost:8080/api/login` should return `{"error":"Invalid credentials"}`.
  - `curl -X POST -d "username=treasurer&password=correctpw" -c cookies.txt http://localhost:8080/api/login` should set a cookie (saved to `cookies.txt`). Then `curl -b cookies.txt http://localhost:8080/api/events` should return the JSON of events for that user.
- These tests ensure that sessions and basic logic are working.

If we deliver the code, it would include:
- `frontend/` directory with HTML/JS/CSS.
- `backend/` directory with `.c` files and maybe a build script or Makefile.
- A `Dockerfile` if containerization is part of delivery.
- Possibly a README with setup instructions.

Given the comprehensive nature of this project, delivering fully working code for every feature is a tall order within one document, but we have outlined samples and the architecture such that a developer (or a savvy fraternity brother) could take this and implement the full system.

In conclusion, this plan covers the full spectrum: site structure, visual design with Zeta Psi’s branding, a robust frontend with modern JS and responsive design, a powerful backend in C tailored for security and performance, and practical considerations for deployment and future growth. By following this blueprint, Zeta Psi Alpha Pi can have a professional and functional website that not only serves current needs but can evolve with the fraternity. 


