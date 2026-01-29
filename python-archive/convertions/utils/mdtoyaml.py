import yaml
import sys
import os
import re

def parse_markdown_to_dict(markdown_content):
    lines = markdown_content.splitlines()
    yaml_dict = {}
    current_section = None
    current_list = None

    for line in lines:
        # Skip empty lines
        if not line.strip():
            continue
        
        # Check for Markdown headers
        header_match = re.match(r'^(#+)\s+(.*)', line)
        if header_match:
            level = len(header_match.group(1))
            header = header_match.group(2).strip()

            # Create a new section for the header
            current_section = {}
            yaml_dict[header] = current_section
            current_list = None  # Reset current list
        
        # Check for list items
        elif re.match(r'^\s*[\*-]\s+', line):
            item = line.strip().lstrip('*-').strip()
            if current_list is None:
                current_list = []
                current_section["items"] = current_list
            current_list.append(item)
        
        # Treat other lines as plain text under the current section
        else:
            if current_list is not None:
                # Add the line as part of the last list item if inside a list
                current_list[-1] += " " + line.strip()
            else:
                # Add the line as a paragraph
                if "paragraph" not in current_section:
                    current_section["paragraph"] = line.strip()
                else:
                    current_section["paragraph"] += " " + line.strip()

    return yaml_dict

def convert_md_to_yaml(input_md_path, output_yaml_path):
    # Check if the input Markdown file exists
    if not os.path.isfile(input_md_path):
        print(f"Error: The file '{input_md_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_yaml_path)
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

    # Convert Markdown to dictionary
    yaml_data = parse_markdown_to_dict(markdown_content)

    try:
        # Write the dictionary as a YAML file
        with open(output_yaml_path, 'w', encoding='utf-8') as file:
            yaml.dump(yaml_data, file, default_flow_style=False, sort_keys=False, allow_unicode=True)
        print(f"Conversion complete. Check the '{output_yaml_path}' file.")
    except Exception as e:
        print(f"Error writing YAML file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python mdtoyaml.py <input_md_path> <output_yaml_path>")
        sys.exit(1)

    input_md_path = sys.argv[1]
    output_yaml_path = sys.argv[2]

    convert_md_to_yaml(input_md_path, output_yaml_path)
