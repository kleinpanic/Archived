from pydub import AudioSegment

import sys

import os



def mp3_to_wav(input_file, output_file):

    if not input_file.lower().endswith('.mp3'):

        raise ValueError("Input file must be an MP3 file.")

    audio = AudioSegment.from_file(input_file, format="mp3")

    audio.export(output_file, format="wav")

    print(f"Converted {input_file} to {output_file}.")



if __name__ == "__main__":

    if len(sys.argv) != 3:

        print("Usage: python mp3_to_wav.py <input_file.mp3> <output_file.wav>")

    else:

        input_file = sys.argv[1]

        output_file = sys.argv[2]

        if not output_file.lower().endswith('.wav'):

            print("Output file must have a .wav extension.")

        elif not os.path.exists(input_file):

            print(f"Input file {input_file} does not exist.")

        else:

            mp3_to_wav(input_file, output_file)

