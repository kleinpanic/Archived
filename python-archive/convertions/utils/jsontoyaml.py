import json
import yaml
import sys
import os

def convert_json_to_yaml(input_json_path, output_yaml_path):
    # Check if the input JSON file exists
    if not os.path.isfile(input_json_path):
        print(f"Error: The file '{input_json_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_yaml_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Load the JSON content
        with open(input_json_path, 'r', encoding='utf-8') as json_file:
            json_content = json.load(json_file)

        # Write the YAML content
        with open(output_yaml_path, 'w', encoding='utf-8') as yaml_file:
            yaml.dump(json_content, yaml_file, default_flow_style=False, sort_keys=False)
        
        print(f"Conversion complete. Check the '{output_yaml_path}' file.")
    except Exception as e:
        print(f"Error during JSON to YAML conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python jsontoyaml.py <input_json_path> <output_yaml_path>")
        sys.exit(1)

    input_json_path = sys.argv[1]
    output_yaml_path = sys.argv[2]

    convert_json_to_yaml(input_json_path, output_yaml_path)
