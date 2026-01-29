import csv
import json
import sys
import os

def main():
    if len(sys.argv) != 3:
        print("Usage: python csvtojson.py <input_csv_path> <output_json_path>")
        sys.exit(1)

    input_csv_path = sys.argv[1]
    output_json_path = sys.argv[2]

    # Check if the input CSV file exists
    if not os.path.isfile(input_csv_path):
        print(f"Error: The input CSV file '{input_csv_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_json_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the CSV file
        with open(input_csv_path, 'r', newline='', encoding='utf-8') as csv_file:
            reader = csv.DictReader(csv_file)
            data = list(reader)
    except FileNotFoundError:
        print(f"Error: The input CSV file '{input_csv_path}' was not found.")
        sys.exit(1)
    except csv.Error as e:
        print(f"Error reading CSV file: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while reading the CSV file: {e}")
        sys.exit(1)

    try:
        # Write data to JSON file
        with open(output_json_path, 'w', encoding='utf-8') as json_file:
            json.dump(data, json_file, indent=4, ensure_ascii=False)
        print(f"Conversion complete. Check the '{output_json_path}' file.")
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_json_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred while writing the JSON file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

