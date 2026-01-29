import json
import csv
import sys
import os

def convert_json_to_csv(input_json_path, output_csv_path):
    # Check if the input JSON file exists
    if not os.path.isfile(input_json_path):
        print(f"Error: The input JSON file '{input_json_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_csv_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Load JSON data
        with open(input_json_path, 'r', encoding='utf-8') as json_file:
            data = json.load(json_file)
            
            if not isinstance(data, list) or not data:
                print("Error: The JSON file must contain a list of objects.")
                sys.exit(1)
    except json.JSONDecodeError:
        print(f"Error: Failed to decode JSON file '{input_json_path}'. Please check if the file is valid JSON.")
        sys.exit(1)
    except Exception as e:
        print(f"Error reading JSON file: {e}")
        sys.exit(1)

    try:
        # Write data to CSV
        with open(output_csv_path, 'w', newline='', encoding='utf-8') as csv_file:
            writer = csv.writer(csv_file)
            
            # Write headers
            headers = data[0].keys()
            writer.writerow(headers)
            
            # Write data rows
            for row in data:
                writer.writerow([row.get(header, "") for header in headers])
                
        print(f"Conversion complete. Check the '{output_csv_path}' file.")
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_csv_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"Error writing CSV file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python jsontocsv.py <input_json_path> <output_csv_path>")
        sys.exit(1)

    input_json_path = sys.argv[1]
    output_csv_path = sys.argv[2]

    convert_json_to_csv(input_json_path, output_csv_path)
