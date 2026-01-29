#!/usr/bin/env python3
import sys
import subprocess
import os
import logging
import argparse

# Configure logging
log_path = '/tmp/convertions.log'
logging.basicConfig(
    filename=log_path,
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)

VERSION = "0.0.1"

def display_version():
    """Display the current version."""
    print(f"Convertions version: {VERSION}")

# Mapping of commands to utility scripts
SCRIPT_MAP = {
    "csvtoexcel": "csvtoexcel.py",
    "csvtojson": "csvtojson.py",
    "docxtomd": "docxtomd.py",
    "exceltocsv": "exceltocsv.py",
    "htmltomd": "htmltomd.py",
    "jsontocsv": "jsontocsv.py",
    "mdtohtml": "mdtohtml.py",
    "yamltomd": "yamltomd.py",
    "pngtojpg": "pngtojpg.py",
    "jpgtopng": "jpgtopng.py",
    "pdftojpg": "pdftojpg.py",
    "imagetomd": "imagetomd.py",
    "jpgstopdf": "jpgstopdf.py",
    "pdftomd": "pdftomd.py",
    "csvmerge": "csvmerge.py",
    "mdtabletocsv": "mdtabletocsv.py",
    "yamlttojson": "yamlttojson.py",
    "jsontoyaml": "jsontoyaml.py",
    "audiototext": "audiototext.py",
    "texttospeech": "texttospeech.py",
    "videotoaudio": "videotoaudio.py",
    "pdftoexcel": "pdftoexcel.py",
    "mergepdfs": "mergepdfs.py",
    "m4atomp3": "m4atomp3.py",
    "mp3tom4a": "mp3tom4a.py",
    "mp3towav": "mp3towav.py",
    "wavtomp3": "wavtomp3.py",
    "wavtotext":"wavtotext.py",
    "mdtodocx":"mdtodocx.py"
}

# Display help for all commands
def display_help():
    help_text = """
Usage: convertions <command> [args...]
Available commands:
  csvtoexcel <input_csv_path> <output_excel_path>
  csvtojson <input_csv_path> <output_json_path>
  exceltocsv <input_excel_path> <output_csv_path>
  htmltomd <input_html_path> <output_md_path>
  jsontocsv <input_json_path> <output_csv_path>
  mdtohtml <input_md_path> <output_html_path>
  yamltomd <input_yaml_path> <output_md_path>
  pngtojpg <input_png_path> <output_jpg_path>
  jpgtopng <input_jpg_path> <output_png_path>
  pdftojpg <input_pdf_path> <output_jpg_path>
  imagetomd <input_image_path> <output_md_path>
  jpgstopdf <output_pdf_path> <input_jpg_path1> <input_jpg_path2> ...
  pdftomd <input_pdf_path> <output_md_path>
  csvmerge <output_csv_path> <common_key> <input_csv1> <input_csv2> ...
  mdtabletocsv <input_md_path> <output_csv_path>
  yamlttojson <input_yaml_path> <output_json_path>
  jsontoyaml <input_json_path> <output_yaml_path>
  audiototext <input_audio_path> <output_text_path>
  texttospeech <input_text_path> <output_audio_path>
  videotoaudio <input_video_path> <output_audio_path>
  pdftoexcel <input_pdf_path> <output_excel_path>
  docxtomd <input_docx_path> <output_md_path>
  mergepdfs <output_pdf_path> <input_pdf1> <input_pdf2> ...
  m4atomp3 <input_m4a_path> <output_mp3_path>
  mp3tom4a <input_mp3_path> <output_m4a_path>
  mp3towav <input_mp3_path> <output_wav_path>
  wavtomp3 <input_wav_path> <output_mp3_path>
  wavtotext <input_wav_path> <output_text_path>
  mdtodocx <input_md_path> <output_docx_path>
"""
    print(help_text)

def expand_path(path):
    """ Expand ~ to the user's home directory """
    return os.path.expanduser(path)

# Adjust script path to locate `utils` correctly in binary or development mode
def get_script_path(script_name):
    # When running as a PyInstaller binary, locate utils in the `_MEIPASS` path
    if getattr(sys, 'frozen', False):
        script_path = os.path.join(sys._MEIPASS, 'utils', script_name)
    else:
        # In development, locate utils relative to the script
        script_path = os.path.join(os.path.dirname(__file__), 'utils', script_name)
    return script_path

def validate_paths(command, args):
    """Validate paths for commands with specific argument requirements."""
    # Commands that require multiple input files and one output file
    if command in ["jpgstopdf", "csvmerge", "mergepdfs"]:
        if len(args) < 2:
            raise ValueError(f"Command '{command}' requires at least one output file and one or more input files.")
        
        output_path = expand_path(args[0])
        input_paths = [expand_path(path) for path in args[1:]]
        
        if not os.path.isfile(output_path) and not os.access(os.path.dirname(output_path) or '.', os.W_OK):
            raise ValueError(f"Output file '{output_path}' is not writable.")
        
        if not all(os.path.isfile(path) for path in input_paths):
            missing_files = [path for path in input_paths if not os.path.isfile(path)]
            raise FileNotFoundError(f"One or more input files do not exist: {', '.join(missing_files)}")
        
        return output_path
    
    # Commands that require exactly one input file and one output file
    if len(args) < 2:
        raise ValueError(f"Command '{command}' requires an input file and an output file.")
    
    input_path, output_path = expand_path(args[0]), expand_path(args[1])
    
    if not os.path.isfile(input_path):
        raise FileNotFoundError(f"Input file '{input_path}' does not exist.")
    
    if not os.access(os.path.dirname(output_path) or '.', os.W_OK):
        raise ValueError(f"Output directory for '{output_path}' is not writable.")
    
    return output_path

def run_command(command, args):
    """Run the specified command by calling the appropriate script."""
    if command not in SCRIPT_MAP:
        raise ValueError(f"Unknown command: {command}")

    # Validate paths and get output path
    output_path = validate_paths(command, args)
    
    # Locate the correct path for the utility script
    script_path = get_script_path(SCRIPT_MAP[command])
    
    try:
        result = subprocess.run([sys.executable, script_path] + args, check=True)
        logging.info(f"Successfully executed {command} on {args}.")
        print(f"Conversion completed successfully. Output saved to '{output_path}'.")
    except subprocess.CalledProcessError as e:
        logging.error(f"Error executing {command} on {args}: {e}")
        print(f"Error: Conversion failed. Check the log file at {log_path} for details.")

def main():
    parser = argparse.ArgumentParser(
        description="Convertions: A tool for various file format conversions."
    )
    parser.add_argument('--version', '-v', action='store_true', help="Display the program version and exit.")
    parser.add_argument('command', type=str, nargs='?', help="The command to run (e.g., csvtoexcel, htmltomd).")
    parser.add_argument('args', nargs=argparse.REMAINDER, help="Arguments for the specified command.")
    args = parser.parse_args()

    # Display version and exit if --version or -v is specified
    if args.version:
        display_version()
        sys.exit(0)

    # If no command is provided, display help and exit
    if not args.command:
        display_help()
        sys.exit(1)

    # If help is requested, display help and exit
    if args.command in ["help", "-h", "--help"]:
        display_help()
        sys.exit(0)

    # Run the command
    try:
        run_command(args.command, args.args)
    except (ValueError, FileNotFoundError) as e:
        print(f"Error: {e}")
        logging.error(e)
        display_help()
        sys.exit(1)

if __name__ == "__main__":
    main()

