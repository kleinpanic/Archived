#!/usr/bin/env python3
import sys
import os
from PyPDF2 import PdfMerger

def merge_pdfs(output_pdf, input_pdfs):
    merger = PdfMerger()

    # Validate and add each input PDF
    for pdf in input_pdfs:
        if not os.path.isfile(pdf):
            print(f"Error: The input file '{pdf}' does not exist.")
            continue
        try:
            merger.append(pdf)
            print(f"Added '{pdf}' to the merger.")
        except Exception as e:
            print(f"Error processing file '{pdf}': {e}")

    # Check if any valid PDFs were loaded
    if not merger.pages:
        print("No valid PDFs to merge. Please check your input files.")
        sys.exit(1)

    # Attempt to write to the output PDF
    try:
        merger.write(output_pdf)
        print(f"PDFs merged successfully into '{output_pdf}'")
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_pdf}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while saving the PDF: {e}")
        sys.exit(1)
    finally:
        merger.close()

def main(args):
    if len(args) < 3:
        print("Usage: mergepdfs <output_pdf> <input_pdf1> <input_pdf2> ...")
        sys.exit(1)

    output_pdf = args[0]
    input_pdfs = args[1:]

    merge_pdfs(output_pdf, input_pdfs)

if __name__ == "__main__":
    main(sys.argv[1:])

