from pydub import AudioSegment

import sys

import os



def mp3_to_m4a(input_file, output_file):

    if not input_file.lower().endswith('.mp3'):

        raise ValueError("Input file must be an MP3 file.")

    audio = AudioSegment.from_file(input_file, format="mp3")

    audio.export(output_file, format="mp4")

    print(f"Converted {input_file} to {output_file}.")



if __name__ == "__main__":

    if len(sys.argv) != 3:

        print("Usage: python mp3_to_m4a.py <input_file.mp3> <output_file.m4a>")

    else:

        input_file = sys.argv[1]

        output_file = sys.argv[2]

        if not output_file.lower().endswith('.m4a'):

            print("Output file must have a .m4a extension.")

        elif not os.path.exists(input_file):

            print(f"Input file {input_file} does not exist.")

        else:

            mp3_to_m4a(input_file, output_file)
