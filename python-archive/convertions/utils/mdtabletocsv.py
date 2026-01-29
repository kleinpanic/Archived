import pandas as pd
import re
import sys
import os

def extract_table_from_markdown(md_content):
    # Adjust regex pattern to be more flexible for matching Markdown tables
    table_pattern = re.compile(r"(\|[^\n]+\|\n?)+")
    match = table_pattern.search(md_content)

    if not match:
        return None
    
    table_str = match.group(0).strip()
    rows = table_str.splitlines()

    # Extract headers
    headers = [col.strip() for col in rows[0].split('|')[1:-1]]

    # Extract the actual data rows, skipping the header separator line
    data_rows = []
    for row in rows[2:]:
        values = [value.strip() for value in row.split('|')[1:-1]]
        data_rows.append(values)

    return pd.DataFrame(data_rows, columns=headers)

def convert_md_table_to_csv(input_md_path, output_csv_path):
    # Check if the input Markdown file exists
    if not os.path.isfile(input_md_path):
        print(f"Error: The file '{input_md_path}' does not exist.")
        sys.exit(1)

    try:
        # Read the Markdown file content
        with open(input_md_path, 'r', encoding='utf-8') as md_file:
            md_content = md_file.read()

        # Extract the table and convert to DataFrame
        df = extract_table_from_markdown(md_content)

        if df is None:
            print("Error: No table found in the Markdown file.")
            sys.exit(1)

        # Write the DataFrame to CSV
        df.to_csv(output_csv_path, index=False)
        print(f"Conversion complete. Check the '{output_csv_path}' file.")
    except Exception as e:
        print(f"Error during Markdown table to CSV conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python mdtabletocsv.py <input_md_path> <output_csv_path>")
        sys.exit(1)

    input_md_path = sys.argv[1]
    output_csv_path = sys.argv[2]

    convert_md_table_to_csv(input_md_path, output_csv_path)
