#!/usr/bin/env python3
import pypandoc
import sys
import os

def convert_md_to_docx(input_md_path, output_docx_path):
    # Check if the input Markdown file exists
    if not os.path.isfile(input_md_path):
        print(f"Error: The file '{input_md_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_docx_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Convert Markdown to DOCX using pypandoc
        pypandoc.convert_file(input_md_path, 'docx', outputfile=output_docx_path)
        print(f"Conversion complete. Check the '{output_docx_path}' file.")
    except Exception as e:
        print(f"Error during Markdown to DOCX conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python mdtodocx.py <input_md_path> <output_docx_path>")
        sys.exit(1)

    input_md_path = sys.argv[1]
    output_docx_path = sys.argv[2]

    convert_md_to_docx(input_md_path, output_docx_path)
