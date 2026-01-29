import speech_recognition as sr
from pydub import AudioSegment
import os
import sys

def convert_audio_to_text(input_audio_path, output_text_path, chunk_length_ms=30000):
    # Check if the input audio file exists
    if not os.path.isfile(input_audio_path):
        print(f"Error: The file '{input_audio_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_text_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    # Convert the audio to WAV format if needed
    wav_audio_path = input_audio_path
    if input_audio_path.lower().endswith('.mp3'):
        try:
            sound = AudioSegment.from_mp3(input_audio_path)
            wav_audio_path = input_audio_path.replace('.mp3', '.wav')
            sound.export(wav_audio_path, format="wav")
            print(f"Converted '{input_audio_path}' to WAV format.")
        except Exception as e:
            print(f"Error converting MP3 to WAV: {e}")
            sys.exit(1)

    # Initialize the recognizer
    recognizer = sr.Recognizer()

    try:
        # Load the full audio file using pydub
        audio = AudioSegment.from_wav(wav_audio_path)
        
        # Split audio into chunks and transcribe each chunk
        num_chunks = len(audio) // chunk_length_ms + 1
        full_text = ""
        
        for i in range(num_chunks):
            start_time = i * chunk_length_ms
            end_time = min((i + 1) * chunk_length_ms, len(audio))
            audio_chunk = audio[start_time:end_time]
            chunk_path = f"temp_chunk_{i}.wav"
            audio_chunk.export(chunk_path, format="wav")

            with sr.AudioFile(chunk_path) as source:
                audio_data = recognizer.record(source)
                
                try:
                    # Transcribe the chunk
                    chunk_text = recognizer.recognize_google(audio_data)
                    full_text += f"Chunk {i + 1}:\n{chunk_text}\n\n"
                except sr.UnknownValueError:
                    print(f"Chunk {i + 1}: Unable to recognize speech.")
                except sr.RequestError as e:
                    print(f"Error with chunk {i + 1}: {e}")
                    sys.exit(1)
                finally:
                    # Clean up the temporary chunk file
                    os.remove(chunk_path)

        # Save the transcribed text to the output file
        with open(output_text_path, 'w', encoding='utf-8') as file:
            file.write(full_text)
        
        print(f"Transcription complete. Check the '{output_text_path}' file.")
    except Exception as e:
        print(f"An unexpected error occurred during transcription: {e}")
        sys.exit(1)
    finally:
        # Clean up the temporary WAV file if it was created
        if wav_audio_path != input_audio_path and os.path.exists(wav_audio_path):
            os.remove(wav_audio_path)
            print(f"Deleted temporary file '{wav_audio_path}'.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python audiototext.py <input_audio_path> <output_text_path>")
        sys.exit(1)

    input_audio_path = sys.argv[1]
    output_text_path = sys.argv[2]

    convert_audio_to_text(input_audio_path, output_text_path)
