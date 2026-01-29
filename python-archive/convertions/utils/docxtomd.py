#!/usr/bin/env python3
import docx
import sys
import os

def convert_docx_to_md(input_docx_path, output_md_path):
    # Check if the input DOCX file exists
    if not os.path.isfile(input_docx_path):
        print(f"Error: The file '{input_docx_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_md_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Load the DOCX file
        doc = docx.Document(input_docx_path)
        
        # Extract text and convert to Markdown
        md_content = ""
        for para in doc.paragraphs:
            md_content += para.text + "\n\n"
        
        # Write the Markdown content to the output file
        with open(output_md_path, 'w', encoding='utf-8') as file:
            file.write(md_content)
        
        print(f"Conversion complete. Check the '{output_md_path}' file.")
    except Exception as e:
        print(f"Error during DOCX to Markdown conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python docxtomd.py <input_docx_path> <output_md_path>")
        sys.exit(1)

    input_docx_path = sys.argv[1]
    output_md_path = sys.argv[2]

    convert_docx_to_md(input_docx_path, output_md_path)
