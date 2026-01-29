# Convertions

The `convertions` project is a comprehensive suite of Python scripts designed to handle various file format conversions and content extractions. It supports operations on CSV, JSON, Excel, HTML, Markdown, YAML, PNG, JPG, PDF, audio, video, and more.

This tool is modular, highly extensible, and packaged for ease of use. You can run it as a Python script or compile it into a standalone binary for seamless deployment.

---

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
  - [Global Options](#global-options)
  - [Available Commands](#available-commands)
- [Adding New Scripts](#adding-new-scripts)
- [Virtual Environment](#virtual-environment)
- [Compilation into a Binary](#compilation-into-a-binary)
- [Dependencies](#dependencies)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/convertions.git
cd convertions
```

### 2. Set Up the Virtual Environment
Create and activate a Python virtual environment to isolate dependencies:
```bash
python3 -m venv venv
source venv/bin/activate
```

### 3. Install Dependencies
Install the required Python packages using the virtual environment's pip:
```bash
./venv/bin/pip install -r requirements.txt
```

### 4. Optional: Add Convertions to Your PATH
To make the `convertions` command globally accessible:
- For Bash:
  ```bash
  echo 'export PATH="$HOME/convertions:$PATH"' >> ~/.bashrc
  source ~/.bashrc
  ```
- For Zsh:
  ```bash
  echo 'export PATH="$HOME/convertions:$PATH"' >> ~/.zshrc
  source ~/.zshrc
  ```

---

## Usage

The `convertions` tool accepts various commands corresponding to supported operations. Each command is mapped to a specific utility script.

### Global Options
- `--help` or `-h`: Display help and a list of available commands.
- `--version` or `-v`: Print the version of the `convertions` tool.

```bash
convertions --help
convertions --version
```

### Available Commands

Each command has its usage syntax. Below is the full list:

#### General Format
```bash
convertions <command> <input_path> <output_path>
```

#### File Operations
- **CSV to Excel**: `csvtoexcel <input_csv_path> <output_excel_path>`
- **CSV to JSON**: `csvtojson <input_csv_path> <output_json_path>`
- **CSV to YAML**: `csvtoyaml <input_csv_path> <output_yaml_path>`
- **Excel to CSV**: `exceltocsv <input_excel_path> <output_csv_path>`
- **Excel to JSON**: `exceltojson <input_excel_path> <output_json_path>`
- **HTML to Markdown**: `htmltomd <input_html_path> <output_md_path>`
- **HTML to PDF**: `htmltopdf <input_html_path> <output_pdf_path>`
- **Image to Markdown (OCR)**: `imagetomd <input_image_path> <output_md_path>`
- **JPG to PNG**: `jpgtopng <input_jpg_path> <output_png_path>`
- **JSON to CSV**: `jsontocsv <input_json_path> <output_csv_path>`
- **JSON to Excel**: `jsontoexcel <input_json_path> <output_excel_path>`
- **JSON to YAML**: `jsontoyaml <input_json_path> <output_yaml_path>`
- **Markdown Table to CSV**: `mdtabletocsv <input_md_path> <output_csv_path>`
- **Markdown to DOCX**: `mdtodocx <input_md_path> <output_docx_path>`
- **Markdown to HTML**: `mdtohtml <input_md_path> <output_html_path>`
- **Markdown to PDF**: `mdtopdf <input_md_path> <output_pdf_path>`
- **Markdown to YAML**: `mdtoyaml <input_md_path> <output_yaml_path>`
- **Merge PDFs**: `mergepdfs <output_pdf_path> <input_pdf1> <input_pdf2> ...`
- **PDF to JPG**: `pdftojpg <input_pdf_path> <output_jpg_base_path>`
- **PDF to Markdown**: `pdftomd <input_pdf_path> <output_md_path>`
- **PDF to Text**: `pdftotext <input_pdf_path> <output_txt_path>`
- **PNG to JPG**: `pngtojpg <input_png_path> <output_jpg_path>`
- **Text to Speech**: `texttospeech <input_text_path> <output_audio_path>`
- **Video to Audio**: `videotoaudio <input_video_path> <output_audio_path>`
- **YAML to JSON**: `yamltojson <input_yaml_path> <output_json_path>`
- **YAML to Markdown**: `yamltomd <input_yaml_path> <output_md_path>`

---

## Adding New Scripts

To extend `convertions` with additional commands:

1. **Add the Script**: Place the script in the `utils` directory.
2. **Make It Executable**: Ensure the script has execute permissions:
   ```bash
   chmod +x <script_name>.py
   ```
3. **Update the Mapping**: Edit `convertions.py` and add a new entry to the `SCRIPT_MAP` dictionary:
   ```python
   "newcommand": "new_script.py"
   ```

---

## Virtual Environment

To ensure proper dependency management, activate the virtual environment before running or modifying the `convertions` tool:
```bash
source venv/bin/activate
```

To deactivate the virtual environment:
```bash
deactivate
```

---

## Compilation into a Binary

The `install.sh` script simplifies the process of creating a standalone binary using PyInstaller. It also allows for optional installation to `/usr/local/bin` for system-wide usage.

### Steps:
1. Run the `install.sh` script:
   ```bash
   ./install.sh
   ```
2. Follow the prompts to decide whether to move the compiled binary to `/usr/local/bin`. If skipped, the binary will remain in the `./dist` directory.

### Key Features of `install.sh`:
- Checks and installs dependencies in the virtual environment.
- Compiles all scripts and utilities into a single binary.
- Optionally installs the binary globally for ease of use.

---

## Dependencies

This project relies on several Python libraries and external tools. Ensure the following are installed:
- Python 3.11+
- `wkhtmltopdf` (for HTML to PDF conversion)
- External dependencies in `requirements.txt`, managed by the virtual environment:
  - `pandas`
  - `PyMuPDF`
  - `pytesseract`
  - `markdown`
  - `pdfkit`
  - `gTTS`
  - `pillow`
  - `moviepy`
  - `PyYAML`

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Acknowledgments

The following libraries and tools power `convertions`:
- [Pillow (PIL)](https://python-pillow.org/)
- [PyMuPDF](https://pymupdf.readthedocs.io/)
- [pytesseract](https://pypi.org/project/pytesseract/)
- [pdfkit](https://pypi.org/project/pdfkit/)
- [Markdown](https://pypi.org/project/Markdown/)
- [MoviePy](https://zulko.github.io/moviepy/)
- [gTTS](https://pypi.org/project/gTTS/)
- [PyYAML](https://pypi.org/project/PyYAML/)
- [wkhtmltopdf](https://wkhtmltopdf.org/)

