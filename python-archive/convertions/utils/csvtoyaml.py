#!/usr/bin/env python3
import pandas as pd
import yaml
import sys
import os

def convert_csv_to_yaml(input_csv_path, output_yaml_path):
    # Check if the input CSV file exists
    if not os.path.isfile(input_csv_path):
        print(f"Error: The file '{input_csv_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_yaml_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the CSV file into a DataFrame
        df = pd.read_csv(input_csv_path)
        
        # Convert the DataFrame to a list of dictionaries
        data = df.to_dict(orient='records')
        
        # Write the data as YAML to the output file
        with open(output_yaml_path, 'w', encoding='utf-8') as file:
            yaml.dump(data, file, default_flow_style=False, sort_keys=False, allow_unicode=True)
        
        print(f"Conversion complete. Check the '{output_yaml_path}' file.")
    except Exception as e:
        print(f"Error during CSV to YAML conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python csvtoyaml.py <input_csv_path> <output_yaml_path>")
        sys.exit(1)

    input_csv_path = sys.argv[1]
    output_yaml_path = sys.argv[2]

    convert_csv_to_yaml(input_csv_path, output_yaml_path)
