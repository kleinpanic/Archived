#!/usr/bin/env python3
import pandas as pd
import sys
import os

def convert_excel_to_json(input_excel_path, output_json_path):
    # Check if the input Excel file exists
    if not os.path.isfile(input_excel_path):
        print(f"Error: The file '{input_excel_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_json_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the Excel file
        df = pd.read_excel(input_excel_path, engine='openpyxl')
        
        # Convert the DataFrame to JSON
        json_content = df.to_json(orient='records', indent=4)
        
        # Write the JSON content to the output file
        with open(output_json_path, 'w', encoding='utf-8') as file:
            file.write(json_content)
        
        print(f"Conversion complete. Check the '{output_json_path}' file.")
    except Exception as e:
        print(f"Error during Excel to JSON conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python exceltotojson.py <input_excel_path> <output_json_path>")
        sys.exit(1)

    input_excel_path = sys.argv[1]
    output_json_path = sys.argv[2]

    convert_excel_to_json(input_excel_path, output_json_path)
