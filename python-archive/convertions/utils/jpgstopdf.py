#!/usr/bin/env python3
from PIL import Image
import sys
import os

def convert_images_to_pdf(image_paths, output_pdf_path):
    images = []

    # Validate and open each image
    for image_path in image_paths:
        if not os.path.isfile(image_path):
            print(f"Error: The input image file '{image_path}' does not exist.")
            continue
        
        try:
            img = Image.open(image_path)
            if img.mode == 'RGBA':
                img = img.convert('RGB')
            images.append(img)
        except Exception as e:
            print(f"Error processing image '{image_path}': {e}")
            continue

    # Check if any valid images were loaded
    if not images:
        print("No valid images to convert. Please check your input files.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_pdf_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Save the images as a PDF
        images[0].save(output_pdf_path, save_all=True, append_images=images[1:])
        print(f"PDF created at '{output_pdf_path}'")
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_pdf_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while saving the PDF: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: jpgstopdf <output_pdf_path> <input_jpg_path1> <input_jpg_path2> ...")
        sys.exit(1)

    output_pdf_path = sys.argv[1]
    image_paths = sys.argv[2:]

    convert_images_to_pdf(image_paths, output_pdf_path)

