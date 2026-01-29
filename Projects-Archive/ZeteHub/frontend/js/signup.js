/**
 * signup.js - Handles the signup form submission via AJAX,
 * with full debug logging to console.
 */

document.addEventListener("DOMContentLoaded", () => {
  const signupForm  = document.getElementById("signupForm");
  const signupError = document.getElementById("signupError");

  signupForm.addEventListener("submit", async (e) => {
    e.preventDefault();
    signupError.textContent = "";

    const username = document.getElementById("signupUsername").value.trim();
    const password = document.getElementById("signupPassword").value;
    const code     = document.getElementById("signupCode").value.trim();

    console.debug("[signup.js] Submitting signup:", { username, passwordLen: password.length, code });

    if (!username || !password || !code) {
      signupError.textContent = "Please fill in all fields.";
      console.warn("[signup.js] Missing signup fields");
      return;
    }

    const formData = new URLSearchParams();
    formData.append("username", username);
    formData.append("password", password);
    formData.append("code", code);

    try {
      const response = await fetch("/api/signup", {
        method: "POST",
        body: formData
      });

      console.debug("[signup.js] HTTP status:", response.status);
      const text = await response.text();
      console.debug("[signup.js] Raw response text:", text);

      let data;
      try {
        data = JSON.parse(text);
      } catch (parseErr) {
        console.error("[signup.js] JSON parse error:", parseErr);
        signupError.textContent = "Invalid server response.";
        return;
      }

      console.debug("[signup.js] Parsed JSON:", data);

      if (data.error) {
        signupError.textContent = data.error;
        console.warn("[signup.js] Signup failed:", data.error);
      } else {
        console.info("[signup.js] Signup succeeded, redirecting to login.html");
        alert("Signup successful! Redirecting to login…");
        window.location.href = "login.html";
      }
    } catch (networkErr) {
      console.error("[signup.js] Network/fetch error:", networkErr);
      signupError.textContent = "An error occurred. Please try again.";
    }
  });
});

