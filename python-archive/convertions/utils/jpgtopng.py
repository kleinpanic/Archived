#!/usr/bin/env python3
from PIL import Image
import sys
import os

def convert_jpg_to_png(input_path, output_path):
    # Check if the input JPG file exists
    if not os.path.isfile(input_path):
        print(f"Error: The input JPG file '{input_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Open and convert the JPG image to PNG
        img = Image.open(input_path)
        img = img.convert("RGB")  # Ensure the image is in RGB mode
        img.save(output_path, "PNG")
        print(f"Conversion complete. Check the '{output_path}' file.")
    except FileNotFoundError:
        print(f"Error: The input JPG file '{input_path}' was not found.")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred during conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: jpgtopng <input_jpg_path> <output_png_path>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    convert_jpg_to_png(input_path, output_path)
