/**
 * On DOMContentLoaded, fetch initial data for the dashboard.
 */
document.addEventListener('DOMContentLoaded', () => {
  // Get user info for a personalized greeting.
  fetch('/api/user/me')
    .then(res => res.json())
    .then(data => {
      document.getElementById('welcomeName').innerText = data.firstName || 'Brother';
    });
  
  // Load the next event for the dashboard.
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

/**
 * Function to show a specific portal section and load its data if necessary.
 * @param {string} sec - The section to show (dashboard, events, finances, documents)
 */
function showSection(sec) {
  ['dashboard', 'events', 'finances', 'documents'].forEach(s => {
    document.getElementById(s).style.display = (s === sec ? 'block' : 'none');
  });
  if (sec === 'events') loadEvents();
  if (sec === 'finances') loadFinances();
  if (sec === 'documents') loadDocuments();
}

/**
 * Logout function calls the backend logout API and redirects to login page.
 */
function logout() {
  fetch('/api/logout', { method: 'POST' }).finally(() => {
    window.location.href = 'login.html';
  });
}

/**
 * Function to load events via an AJAX call to /api/events.
 */
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

/**
 * Function to load financial data for the logged-in member.
 */
function loadFinances() {
  fetch('/api/finances/me')
    .then(res => res.json())
    .then(data => {
      if (data.error) {
        document.getElementById('financeStatus').innerText = 'Error: ' + data.error;
        return;
      }
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

/**
 * Function to load document metadata and populate the document list.
 */
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

