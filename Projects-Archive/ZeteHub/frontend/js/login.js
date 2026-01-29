// login.js (in /frontend/js/login.js)
document.addEventListener("DOMContentLoaded", () => {
  const form      = document.getElementById("loginForm");
  const errBox    = document.getElementById("loginError");

  form.addEventListener("submit", async e => {
    e.preventDefault();
    errBox.textContent = "";

    const user = document.getElementById("loginUsername").value.trim();
    const pass = document.getElementById("loginPassword").value;
    console.debug("[login.js] submit:", { user, passLen: pass.length });

    if (!user || !pass) {
      errBox.textContent = "Please fill in all fields.";
      return;
    }

    const body = new URLSearchParams({ username: user, password: pass });
    let resp, text;
    try {
      resp = await fetch("/api/login", {
        method: "POST",
        body,
        credentials: "include"
      });
    } catch (networkErr) {
      console.error("[login.js] fetch failed", networkErr);
      errBox.textContent = "Network error — try again.";
      return;
    }

    console.debug("[login.js] status:", resp.status);
    console.debug("[login.js] headers:");
    for (let [k,v] of resp.headers.entries()) console.debug(`  ${k}: ${v}`);

    try {
      text = await resp.text();
    } catch (readErr) {
      console.error("[login.js] .text() failed", readErr);
      errBox.textContent = "Failed to read response.";
      return;
    }

    console.debug("[login.js] raw response text:", JSON.stringify(text));

    let data;
    try {
      data = JSON.parse(text.trim());
    } catch (parseErr) {
      console.error("[login.js] JSON.parse failed", parseErr);
      errBox.textContent = `Invalid server response: ${text.trim()}`;
      return;
    }

    console.debug("[login.js] parsed JSON:", data);

    if (data.error) {
      errBox.textContent = data.error;
    } else {
      window.location.href = "index.html";
    }
  });
});

