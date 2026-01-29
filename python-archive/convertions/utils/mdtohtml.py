import markdown
import sys
import os

def convert_md_to_html(input_md_path, output_html_path):
    # Check if the input Markdown file exists
    if not os.path.isfile(input_md_path):
        print(f"Error: The input Markdown file '{input_md_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_html_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the Markdown file content
        with open(input_md_path, 'r', encoding='utf-8') as md_file:
            md_content = md_file.read()
    except FileNotFoundError:
        print(f"Error: The input Markdown file '{input_md_path}' was not found.")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied when reading '{input_md_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while reading the Markdown file: {e}")
        sys.exit(1)

    try:
        # Convert Markdown content to HTML
        html_content = markdown.markdown(md_content)
        
        # Write the HTML content to the output file
        with open(output_html_path, 'w', encoding='utf-8') as html_file:
            html_file.write(html_content)
        
        print(f"Conversion complete. Check the '{output_html_path}' file.")
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_html_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while writing the HTML file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python mdtohtml.py <input_md_path> <output_html_path>")
        sys.exit(1)

    input_md_path = sys.argv[1]
    output_html_path = sys.argv[2]

    convert_md_to_html(input_md_path, output_html_path)
