import pandas as pd
import sys
import os

def merge_csv_files(output_csv_path, common_key, *input_csv_paths):
    if len(input_csv_paths) < 2:
        print("Error: Please provide at least two CSV files to merge.")
        sys.exit(1)

    try:
        # Read the first CSV file
        merged_df = pd.read_csv(input_csv_paths[0])

        # Merge each subsequent CSV file based on the common key
        for csv_path in input_csv_paths[1:]:
            df = pd.read_csv(csv_path)
            merged_df = pd.merge(merged_df, df, on=common_key, how='outer')

        # Save the merged CSV to the output path
        merged_df.to_csv(output_csv_path, index=False)
        print(f"Merge complete. Check the '{output_csv_path}' file.")
    except Exception as e:
        print(f"Error during CSV merge: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: python csvmerge.py <output_csv_path> <common_key> <input_csv1> <input_csv2> ...")
        sys.exit(1)

    output_csv_path = sys.argv[1]
    common_key = sys.argv[2]
    input_csv_paths = sys.argv[3:]

    merge_csv_files(output_csv_path, common_key, *input_csv_paths)

