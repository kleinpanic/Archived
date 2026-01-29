#!/usr/bin/env python3
import fitz  # PyMuPDF
import sys
import os

def convert_pdf_to_text(input_pdf_path, output_txt_path):
    # Check if the input PDF file exists
    if not os.path.isfile(input_pdf_path):
        print(f"Error: The input PDF file '{input_pdf_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_txt_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Open the PDF document
        pdf_document = fitz.open(input_pdf_path)
        if pdf_document.page_count == 0:
            print("Error: The PDF file contains no pages.")
            pdf_document.close()
            sys.exit(1)

        # Extract text from each page and write it to the output file
        with open(output_txt_path, 'w', encoding='utf-8') as txt_file:
            for page_num in range(pdf_document.page_count):
                page = pdf_document.load_page(page_num)
                text = page.get_text()
                txt_file.write(f"--- Page {page_num + 1} ---\n")
                txt_file.write(text + "\n")

        pdf_document.close()
        print(f"Text extraction complete. Check the '{output_txt_path}' file.")
    except Exception as e:
        print(f"Error during PDF to text conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python pdftotext.py <input_pdf_path> <output_txt_path>")
        sys.exit(1)

    input_pdf_path = sys.argv[1]
    output_txt_path = sys.argv[2]

    convert_pdf_to_text(input_pdf_path, output_txt_path)

