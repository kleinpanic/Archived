import speech_recognition as sr

import sys

import os

import time

import subprocess

import requests

import zipfile

from pathlib import Path



try:

    from vosk import Model, KaldiRecognizer

except ImportError:

    vosk_installed = False

else:

    vosk_installed = True



VOSK_MODEL_URL = "https://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip"

VOSK_MODEL_DIR = os.path.abspath("model")



def download_and_setup_vosk():

    """Download and extract the Vosk model."""

    print("Downloading Vosk model... This may take a few minutes.")

    response = requests.get(VOSK_MODEL_URL, stream=True)

    zip_path = "vosk_model.zip"



    with open(zip_path, "wb") as file:

        for chunk in response.iter_content(chunk_size=8192):

            file.write(chunk)



    print("Extracting Vosk model...")

    with zipfile.ZipFile(zip_path, "r") as zip_ref:

        zip_ref.extractall(VOSK_MODEL_DIR)



    os.remove(zip_path)

    print(f"Vosk model downloaded and set up in '{VOSK_MODEL_DIR}'.")



    # Verify that the model directory contains necessary files

    verify_model_files()



    # Add model directory to .gitignore

    with open(".gitignore", "a") as gitignore:

        gitignore.write(f"\\n# Ignore Vosk model directory\\n{VOSK_MODEL_DIR}\\n")



def verify_model_files():
    """Verify that all required files are present in the model directory."""
    required_files = ["conf/model.conf", "am/final.mdl"]
    missing_files = [file for file in required_files if not Path(VOSK_MODEL_DIR, file).exists()]
    if missing_files:
        raise Exception(f"Model file(s) missing: {', '.join(missing_files)}. Re-download the Vosk model manually.")

def convert_to_vosk_compatible_wav(input_file):

    """Convert the WAV file to a Vosk-compatible format using ffmpeg."""

    output_file = "converted.wav"

    try:

        print(f"Converting {input_file} to Vosk-compatible format...")

        subprocess.run([

            "ffmpeg", "-i", input_file, "-ar", "16000", "-ac", "1", "-c:a", "pcm_s16le", output_file

        ], check=True)

        print(f"Converted file saved as {output_file}.")

        return output_file

    except subprocess.CalledProcessError as e:

        print(f"Error converting file: {e}")

        return None



def online_wav_to_text(input_file):

    recognizer = sr.Recognizer()

    with sr.AudioFile(input_file) as source:

        print("Processing audio for online recognition...")

        audio_data = recognizer.record(source)

        for attempt in range(3):  # Retry up to 3 times

            try:

                return recognizer.recognize_google(audio_data)

            except sr.RequestError as e:

                print(f"API error on attempt {attempt + 1}: {e}")

                time.sleep(2 ** attempt)  # Exponential backoff

            except sr.UnknownValueError:

                print("Speech recognition could not understand the audio.")

                return None

        return None



def offline_wav_to_text(input_file):

    import wave  # Ensure wave is imported before using offline recognition



    model_path = Path(VOSK_MODEL_DIR)

    if not model_path.exists():

        print("Offline model not found. Would you like to set it up now? [y/N]")

        choice = input().strip().lower()

        if choice == 'y':

            download_and_setup_vosk()

        else:

            print("Skipping offline setup. Exiting.")

            return None



    try:

        verify_model_files()

    except Exception as e:

        print(e)

        return None



    wf = wave.open(input_file, "rb")

    if wf.getnchannels() != 1 or wf.getsampwidth() != 2 or wf.getframerate() not in [8000, 16000]:

        print("Audio file must be WAV format mono PCM.")

        wf.close()

        return None



    model = Model(VOSK_MODEL_DIR)

    recognizer = KaldiRecognizer(model, wf.getframerate())



    print("Processing audio for offline recognition...")

    results = []

    while True:

        data = wf.readframes(4000)

        if len(data) == 0:

            break

        if recognizer.AcceptWaveform(data):

            results.append(recognizer.Result())

    wf.close()



    # Combine results into a single text

    return " ".join([result["text"] for result in map(eval, results)])



def wav_to_text(input_file, output_file):

    if not input_file.lower().endswith('.wav'):

        raise ValueError("Input file must be a WAV file.")



    # Check and convert file format if necessary

    converted_file = convert_to_vosk_compatible_wav(input_file)

    if not converted_file:

        print("File conversion failed. Unable to proceed.")

        return False



    text = online_wav_to_text(converted_file)

    if text is None:  # Fallback to offline if online fails

        print("Online recognition failed. Switching to offline recognition...")

        text = offline_wav_to_text(converted_file)



    if text:

        with open(output_file, 'w') as f:

            f.write(text)

        print(f"Transcription completed successfully. Output saved to '{output_file}'.")

        return True



    print("Transcription failed. Please check the error message above.")

    return False



if __name__ == "__main__":

    if len(sys.argv) != 3:

        print("Usage: python wav_to_text.py <input_file.wav> <output_file.txt>")

    else:

        input_file = sys.argv[1]

        output_file = sys.argv[2]

        if not output_file.lower().endswith('.txt'):

            print("Output file must have a .txt extension.")

        elif not os.path.exists(input_file):

            print(f"Input file {input_file} does not exist.")

        else:

            success = wav_to_text(input_file, output_file)

            if not success:

                sys.exit(1)  # Exit with a non-zero status code on failure

