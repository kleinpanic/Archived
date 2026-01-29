import html2text
import sys
import os

def main():
    if len(sys.argv) != 3:
        print("Usage: python html_to_md.py <input_html_path> <output_md_path>")
        sys.exit(1)

    input_html_path = sys.argv[1]
    output_md_path = sys.argv[2]

    # Check if input HTML file exists
    if not os.path.isfile(input_html_path):
        print(f"Error: The input HTML file '{input_html_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_md_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read HTML file content
        with open(input_html_path, 'r', encoding='utf-8') as html_file:
            html_content = html_file.read()
    except FileNotFoundError:
        print(f"Error: The input HTML file '{input_html_path}' was not found.")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied when reading '{input_html_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while reading the HTML file: {e}")
        sys.exit(1)

    try:
        # Convert HTML to Markdown
        md_content = html2text.html2text(html_content)
        # Write Markdown content to output file
        with open(output_md_path, 'w', encoding='utf-8') as md_file:
            md_file.write(md_content)
        print(f"Conversion complete. Check the '{output_md_path}' file.")
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_md_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while writing the Markdown file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

