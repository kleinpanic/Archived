import pandas as pd
import sys
import os

def main():
    if len(sys.argv) != 3:
        print("Usage: python excelto_csv.py <input_excel_path> <output_csv_path>")
        sys.exit(1)

    input_excel_path = sys.argv[1]
    output_csv_path = sys.argv[2]

    # Check if input Excel file exists
    if not os.path.isfile(input_excel_path):
        print(f"Error: The input Excel file '{input_excel_path}' does not exist.")
        sys.exit(1)

    # Check if the output directory is writable
    output_dir = os.path.dirname(output_csv_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read the Excel file
        df = pd.read_excel(input_excel_path, engine='openpyxl')
        # Convert to CSV
        df.to_csv(output_csv_path, index=False)
        print(f"Conversion complete. Check the '{output_csv_path}' file.")
    except FileNotFoundError:
        print(f"Error: The input Excel file '{input_excel_path}' was not found.")
        sys.exit(1)
    except pd.errors.EmptyDataError:
        print(f"Error: The input Excel file '{input_excel_path}' is empty.")
        sys.exit(1)
    except pd.errors.ExcelFileError as e:
        print(f"Error reading the Excel file '{input_excel_path}': {e}")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_csv_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred during conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
