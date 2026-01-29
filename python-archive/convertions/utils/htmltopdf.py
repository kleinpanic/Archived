#!/usr/bin/env python3
import pdfkit
import sys
import os

def convert_html_to_pdf(input_html_path, output_pdf_path):
    # Check if the input HTML file exists
    if not os.path.isfile(input_html_path):
        print(f"Error: The file '{input_html_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_pdf_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Specify the correct path to wkhtmltopdf
        config = pdfkit.configuration(wkhtmltopdf='/usr/bin/wkhtmltopdf')
        pdfkit.from_file(input_html_path, output_pdf_path, configuration=config)
        
        print(f"Conversion complete. Check the '{output_pdf_path}' file.")
    except Exception as e:
        print(f"Error during HTML to PDF conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python htmltopdf.py <input_html_path> <output_pdf_path>")
        sys.exit(1)

    input_html_path = sys.argv[1]
    output_pdf_path = sys.argv[2]

    convert_html_to_pdf(input_html_path, output_pdf_path)

