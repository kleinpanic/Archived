import yaml
import sys
import os

def dict_to_markdown(data, indent=0):
    markdown = ""
    for key, value in data.items():
        if isinstance(value, dict):
            markdown += " " * indent + f"- **{key}**:\n"
            markdown += dict_to_markdown(value, indent + 2)
        elif isinstance(value, list):
            markdown += " " * indent + f"- **{key}**:\n"
            for item in value:
                if isinstance(item, dict):
                    markdown += dict_to_markdown(item, indent + 4)
                else:
                    markdown += " " * (indent + 4) + f"- {item}\n"
        else:
            markdown += " " * indent + f"- **{key}**: {value}\n"
    return markdown

def convert_yaml_to_markdown(input_yaml_path, output_md_path):
    # Check if the input YAML file exists
    if not os.path.isfile(input_yaml_path):
        print(f"Error: The file '{input_yaml_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_md_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Load the YAML content
        with open(input_yaml_path, 'r', encoding='utf-8') as file:
            yaml_content = yaml.safe_load(file)
    except yaml.YAMLError as e:
        print(f"Error parsing YAML file: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error reading YAML file: {e}")
        sys.exit(1)

    # Convert YAML to Markdown
    markdown_content = dict_to_markdown(yaml_content)

    try:
        # Write the Markdown content to the output file
        with open(output_md_path, 'w', encoding='utf-8') as file:
            file.write(markdown_content)
        print(f"Conversion complete. Check the '{output_md_path}' file.")
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_md_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"Error writing Markdown file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python yamltomd.py <input_yaml_path> <output_md_path>")
        sys.exit(1)

    input_yaml_path = sys.argv[1]
    output_md_path = sys.argv[2]

    convert_yaml_to_markdown(input_yaml_path, output_md_path)
