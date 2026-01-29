#!/usr/bin/env python3
import sys
import glob
from PIL import Image

def main():
    if len(sys.argv) < 3:
        print("Usage: {} output.pdf image1 [image2 ...]".format(sys.argv[0]))
        print("Example: {} lab1.pdf ~/path/*".format(sys.argv[0]))
        sys.exit(1)

    output_pdf = sys.argv[1]
    input_args = sys.argv[2:]

    # Build the list of image files.
    file_list = []
    for arg in input_args:
        # If the argument contains a wildcard, expand it.
        if any(ch in arg for ch in '*?['):
            expanded = glob.glob(arg)
            file_list.extend(expanded)
        else:
            file_list.append(arg)

    # Filter for PNG and JPG files (case-insensitive)
    file_list = [f for f in file_list if f.lower().endswith(('.png', '.jpg', '.jpeg'))]

    if not file_list:
        print("No PNG/JPG images found.")
        sys.exit(1)

    # Sort the file list (optional, but usually helpful)
    file_list.sort()

    # Open and convert images to RGB (required for PDF conversion)
    images = []
    for file in file_list:
        try:
            img = Image.open(file)
        except Exception as e:
            print(f"Error opening {file}: {e}")
            continue

        # Convert image mode if necessary
        if img.mode != 'RGB':
            img = img.convert('RGB')
        images.append(img)

    if not images:
        print("No valid images to convert.")
        sys.exit(1)

    # Save the images as a single PDF. The first image is used as a base,
    # and the rest are appended.
    try:
        images[0].save(output_pdf, "PDF", resolution=100.0, save_all=True, append_images=images[1:])
        print(f"Saved PDF as {output_pdf}")
    except Exception as e:
        print(f"Error saving PDF: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main()

