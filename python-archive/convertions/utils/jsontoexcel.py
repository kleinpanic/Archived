#!/usr/bin/env python3
import pandas as pd
import sys
import os

def convert_json_to_excel(input_json_path, output_excel_path):
    # Check if the input JSON file exists
    if not os.path.isfile(input_json_path):
        print(f"Error: The file '{input_json_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_excel_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the JSON file
        df = pd.read_json(input_json_path)
        
        # Write the DataFrame to Excel
        df.to_excel(output_excel_path, index=False, engine='openpyxl')
        
        print(f"Conversion complete. Check the '{output_excel_path}' file.")
    except ValueError:
        print(f"Error: Invalid JSON format in the file '{input_json_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"Error during JSON to Excel conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python jsontoexcel.py <input_json_path> <output_excel_path>")
        sys.exit(1)

    input_json_path = sys.argv[1]
    output_excel_path = sys.argv[2]

    convert_json_to_excel(input_json_path, output_excel_path)
