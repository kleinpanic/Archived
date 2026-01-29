#!/usr/bin/env python3
import fitz  # PyMuPDF
import sys

if len(sys.argv) != 3:
    print("Usage: pdftojpg <input_pdf_path> <output_jpg_path>")
    sys.exit(1)

input_path = sys.argv[1]
output_path = sys.argv[2]

try:
    pdf_document = fitz.open(input_path)
    for page_num in range(len(pdf_document)):
        page = pdf_document.load_page(page_num)
        pix = page.get_pixmap()
        output_file = f"{output_path}_page{page_num+1}.jpg"
        pix.save(output_file)
        print(f"Saved {output_file}")
    pdf_document.close()
except Exception as e:
    print(f"Error during conversion: {e}")
    sys.exit(1)
