from pydub import AudioSegment

import sys

import os



def wav_to_mp3(input_file, output_file):

    if not input_file.lower().endswith('.wav'):

        raise ValueError("Input file must be a WAV file.")

    audio = AudioSegment.from_file(input_file, format="wav")

    audio.export(output_file, format="mp3")

    print(f"Converted {input_file} to {output_file}.")



if __name__ == "__main__":

    if len(sys.argv) != 3:

        print("Usage: python wav_to_mp3.py <input_file.wav> <output_file.mp3>")

    else:

        input_file = sys.argv[1]

        output_file = sys.argv[2]

        if not output_file.lower().endswith('.mp3'):

            print("Output file must have a .mp3 extension.")

        elif not os.path.exists(input_file):

            print(f"Input file {input_file} does not exist.")

        else:

            wav_to_mp3(input_file, output_file)

