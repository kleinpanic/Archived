import markdown
import pdfkit
import sys
import os

def convert_md_to_pdf(input_md_path, output_pdf_path):
    # Check if the input Markdown file exists
    if not os.path.isfile(input_md_path):
        print(f"Error: The file '{input_md_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_pdf_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the Markdown file content
        with open(input_md_path, 'r', encoding='utf-8') as file:
            markdown_content = file.read()
    except Exception as e:
        print(f"Error reading Markdown file: {e}")
        sys.exit(1)

    try:
        # Convert Markdown to HTML
        html_content = markdown.markdown(markdown_content)
        
        # Convert HTML to PDF using pdfkit
        pdfkit.from_string(html_content, output_pdf_path)
        print(f"Conversion complete. Check the '{output_pdf_path}' file.")
    except Exception as e:
        print(f"Error during conversion to PDF: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python mdtopdf.py <input_md_path> <output_pdf_path>")
        sys.exit(1)

    input_md_path = sys.argv[1]
    output_pdf_path = sys.argv[2]

    convert_md_to_pdf(input_md_path, output_pdf_path)
