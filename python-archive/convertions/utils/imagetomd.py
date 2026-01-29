#!/usr/bin/env python3
from PIL import Image, ImageEnhance
import pytesseract
import sys
import os

def image_to_markdown(image_path, output_md_path):
    # Check if the input image file exists
    if not os.path.isfile(image_path):
        print(f"Error: The input image file '{image_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_md_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Open and preprocess the image
        img = Image.open(image_path)
        img = img.convert('L')  # Convert to grayscale
        enhancer = ImageEnhance.Contrast(img)
        img = enhancer.enhance(2.0)  # Increase contrast

        # Use Tesseract to convert image to text
        text = pytesseract.image_to_string(img, lang='eng')  # Specify language

        # Create Markdown content
        md_content = "# Image Content\n\n"
        md_content += text
        
        # Write the Markdown content to the output file
        with open(output_md_path, 'w', encoding='utf-8') as file:
            file.write(md_content)
        
        print(f"Markdown file created at '{output_md_path}'")
    except FileNotFoundError:
        print(f"Error: The input image file '{image_path}' was not found.")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_md_path}'.")
        sys.exit(1)
    except pytesseract.pytesseract.TesseractError as e:
        print(f"Error processing image with Tesseract: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: imagetomd <input_image_path> <output_md_path>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    image_to_markdown(input_path, output_path)

