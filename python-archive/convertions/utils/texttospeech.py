from gtts import gTTS
from pydub import AudioSegment
import sys
import os

def convert_text_to_speech(input_text_path, output_audio_path, lang='en'):
    # Check if the input text file exists
    if not os.path.isfile(input_text_path):
        print(f"Error: The file '{input_text_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_audio_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the text file content
        with open(input_text_path, 'r', encoding='utf-8') as file:
            text = file.read()
        
        # Convert text to speech
        tts = gTTS(text=text, lang=lang, slow=False)
        temp_mp3_path = output_audio_path.replace('.wav', '.mp3')
        tts.save(temp_mp3_path)
        
        # Convert the mp3 file to wav (if needed)
        if output_audio_path.endswith('.wav'):
            sound = AudioSegment.from_mp3(temp_mp3_path)
            sound.export(output_audio_path, format="wav")
            os.remove(temp_mp3_path)
            print(f"Converted '{input_text_path}' to '{output_audio_path}' as WAV format.")
        else:
            print(f"Converted '{input_text_path}' to '{temp_mp3_path}' as MP3 format.")
    except Exception as e:
        print(f"An unexpected error occurred during text-to-speech conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python texttospeech.py <input_text_path> <output_audio_path>")
        sys.exit(1)

    input_text_path = sys.argv[1]
    output_audio_path = sys.argv[2]

    convert_text_to_speech(input_text_path, output_audio_path)

