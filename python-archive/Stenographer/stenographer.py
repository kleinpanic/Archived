import threading
import queue
import tkinter as tk
from tkinter.scrolledtext import ScrolledText
import pyaudio
import json
import os
import time

# Import Vosk and pypandoc for conversion
from vosk import Model, KaldiRecognizer
try:
    import pypandoc
except ImportError:
    print("pypandoc is not installed. Run 'pip install pypandoc' to enable Markdown to DOCX conversion.")
    pypandoc = None

# -------------------------------
# Configuration & Model Setup
# -------------------------------
# Update this path to the absolute location of your downloaded Vosk model.
MODEL_PATH = "/home/klein/codeWS/Dependencies/vosk-model-en-us-0.42-gigaspeech" # <-- Change this!

AUDIO_RATE = 16000
CHUNK = 1024

# Initialize Vosk model (ensure MODEL_PATH is correct)
try:
    model = Model(MODEL_PATH)
except Exception as e:
    print(f"Error initializing Vosk model: {e}")
    exit(1)

recognizer = KaldiRecognizer(model, AUDIO_RATE)

# Queue for passing transcript text from audio thread to GUI
transcript_queue = queue.Queue()

# Global variable for storing the session transcript
session_transcript = []

# -------------------------------
# Audio Listener Thread
# -------------------------------
def audio_listener():
    """Captures audio from microphone, transcribes using Vosk, and queues the result."""
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16,
                    channels=1,
                    rate=AUDIO_RATE,
                    input=True,
                    frames_per_buffer=CHUNK)
    stream.start_stream()

    while True:
        data = stream.read(CHUNK, exception_on_overflow=False)
        if recognizer.AcceptWaveform(data):
            result = json.loads(recognizer.Result())
            text = result.get("text", "").strip()
            if text:
                transcript_queue.put(text)
        else:
            # Optionally handle partial results here
            pass

# -------------------------------
# Markdown and DOCX Conversion
# -------------------------------
def save_transcript_to_files():
    """
    Saves the session transcript to a Markdown file and converts it to DOCX.
    The Markdown file is preserved.
    """
    md_filename = "session_notes.md"
    docx_filename = "session_notes.docx"

    # Write transcript to Markdown file with simple formatting
    with open(md_filename, "w", encoding="utf-8") as f_md:
        f_md.write("# Session Notes\n\n")
        for entry in session_transcript:
            f_md.write(f"- {entry}\n")
    print(f"Markdown notes saved to {md_filename}")

    # Convert Markdown to DOCX if pypandoc is available
    if pypandoc is not None:
        try:
            output = pypandoc.convert_file(md_filename, 'docx', outputfile=docx_filename)
            print(f"DOCX file generated: {docx_filename}")
        except Exception as e:
            print(f"Error converting to DOCX: {e}")
    else:
        print("pypandoc not available; skipping DOCX conversion.")

# -------------------------------
# GUI Setup with Tkinter
# -------------------------------
class StenoGUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Advanced AI Stenographer")
        self.geometry("800x600")
        self.configure(bg="#f0f0f0")
        
        # Transcript display
        self.transcript_display = ScrolledText(self, height=15, wrap=tk.WORD)
        self.transcript_display.pack(pady=10, padx=10, fill=tk.BOTH, expand=True)
        
        # Speakers list placeholder
        self.speaker_label = tk.Label(self, text="Detected Speakers:", bg="#f0f0f0", font=("Helvetica", 12))
        self.speaker_label.pack(pady=(10, 0))
        self.speaker_list = tk.Listbox(self, height=5)
        self.speaker_list.pack(padx=10, fill=tk.X)
        
        # Notes panel (advanced note structuring can be added here)
        self.notes_display = ScrolledText(self, height=10, wrap=tk.WORD, bg="#e8f5e9")
        self.notes_display.pack(pady=10, padx=10, fill=tk.BOTH, expand=True)
        
        # Volume visualization canvas (placeholder)
        self.canvas = tk.Canvas(self, height=100, bg="black")
        self.canvas.pack(pady=10, padx=10, fill=tk.X)
        
        # Save Notes Button
        self.save_button = tk.Button(self, text="Save Notes", command=save_transcript_to_files)
        self.save_button.pack(pady=10)
        
        # Start periodic GUI updates
        self.after(100, self.update_gui)

    def update_gui(self):
        """Fetch new transcript data and update displays."""
        while not transcript_queue.empty():
            text = transcript_queue.get()
            session_transcript.append(text)
            self.transcript_display.insert(tk.END, text + "\n")
            self.transcript_display.see(tk.END)
            
            # For demonstration, simply append the text to the notes display.
            note = f"Note: {text}"
            self.notes_display.insert(tk.END, note + "\n")
            self.notes_display.see(tk.END)
            
            # Update speakers list (placeholder implementation)
            if "Speaker A" not in self.speaker_list.get(0, tk.END):
                self.speaker_list.insert(tk.END, "Speaker A")
        
        # Update live volume visualization (this is just a dummy bar)
        self.canvas.delete("all")
        # In a full implementation, compute the current amplitude for dynamic visualization.
        self.canvas.create_rectangle(10, 50, 50, 90, fill="green")
        
        # Repeat update
        self.after(100, self.update_gui)

# -------------------------------
# Main Execution
# -------------------------------
if __name__ == "__main__":
    # Start the audio listener thread (daemon thread so it stops with the GUI)
    audio_thread = threading.Thread(target=audio_listener, daemon=True)
    audio_thread.start()
    
    # Initialize and start the GUI
    app = StenoGUI()
    app.mainloop()

