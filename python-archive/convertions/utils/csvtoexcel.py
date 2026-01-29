import pandas as pd
import sys
import os

def main():
    if len(sys.argv) != 3:
        print("Usage: python csvtoexcel.py <input_csv_path> <output_excel_path.xlsx>")
        sys.exit(1)

    input_csv_path = sys.argv[1]
    output_excel_path = sys.argv[2]

    # Check if input CSV file exists
    if not os.path.isfile(input_csv_path):
        print(f"Error: The input CSV file '{input_csv_path}' does not exist.")
        sys.exit(1)

    # Check if output directory is writable
    output_dir = os.path.dirname(output_excel_path)
    if output_dir and not os.access(output_dir, os.W_OK):
        print(f"Error: The output directory '{output_dir}' is not writable.")
        sys.exit(1)

    try:
        # Read CSV and convert to Excel
        df = pd.read_csv(input_csv_path)
        df.to_excel(output_excel_path, index=False)
        print(f"Conversion complete. Check the '{output_excel_path}' file.")
    except pd.errors.EmptyDataError:
        print(f"Error: The input CSV file '{input_csv_path}' is empty.")
        sys.exit(1)
    except pd.errors.ParserError as e:
        print(f"Error: Failed to parse CSV file '{input_csv_path}': {e}")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied when writing to '{output_excel_path}'.")
        sys.exit(1)
    except Exception as e:
        print(f"Error during conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
