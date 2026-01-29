from moviepy.editor import VideoFileClip
import sys
import os

def convert_video_to_audio(input_video_path, output_audio_path):
    # Check if the input video file exists
    if not os.path.isfile(input_video_path):
        print(f"Error: The file '{input_video_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_audio_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Load the video file
        video_clip = VideoFileClip(input_video_path)
        
        # Check if the video has an audio track
        if video_clip.audio is None:
            print("Error: The video file does not contain an audio track.")
            video_clip.close()
            sys.exit(1)

        # Extract audio
        audio_clip = video_clip.audio

        # Save audio to the output file
        if output_audio_path.endswith('.mp3'):
            audio_clip.write_audiofile(output_audio_path, codec='mp3')
        elif output_audio_path.endswith('.wav'):
            audio_clip.write_audiofile(output_audio_path, codec='pcm_s16le')
        else:
            print("Error: Unsupported output format. Please use .mp3 or .wav")
            sys.exit(1)

        # Close the clips to free resources
        audio_clip.close()
        video_clip.close()

        print(f"Conversion complete. Check the '{output_audio_path}' file.")
    except Exception as e:
        print(f"An unexpected error occurred during video-to-audio conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python videotoaudio.py <input_video_path> <output_audio_path>")
        sys.exit(1)

    input_video_path = sys.argv[1]
    output_audio_path = sys.argv[2]

    convert_video_to_audio(input_video_path, output_audio_path)
