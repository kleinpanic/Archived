#!/usr/bin/env python3
import fitz  # PyMuPDF
import sys
import os

def pdf_to_markdown(pdf_path, output_md_path):
    # Check if the input PDF file exists
    if not os.path.isfile(pdf_path):
        print(f"Error: The input PDF file '{pdf_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_md_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Open the PDF document
        pdf_document = fitz.open(pdf_path)
        if pdf_document.page_count == 0:
            print("Error: The PDF file contains no pages.")
            pdf_document.close()
            sys.exit(1)

        # Initialize the Markdown content
        md_content = "# PDF Content\n\n"
        
        # Extract text from each page
        for page_num in range(len(pdf_document)):
            page = pdf_document.load_page(page_num)
            text = page.get_text()
            md_content += f"## Page {page_num + 1}\n\n{text}\n\n"
        
        pdf_document.close()
        
        # Write the Markdown content to the output file
        with open(output_md_path, 'w', encoding='utf-8') as file:
            file.write(md_content)
        
        print(f"Markdown file created at '{output_md_path}'")
    except FileNotFoundError:
        print(f"Error: The input PDF file '{pdf_path}' was not found.")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_md_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: pdftomd <input_pdf_path> <output_md_path>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    pdf_to_markdown(input_path, output_path)
