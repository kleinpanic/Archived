#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status,
# Treat unset variables as an error,
# Prevent errors in a pipeline from being masked
set -euo pipefail

# ============================
# Configuration Constants
# ============================

SOURCE_DIR="$HOME/vimwiki"                   # Source Vimwiki directory
TEMP_DIR="/tmp/vimwikihtml"                  # Temporary HTML output directory
CSS_FILE="$HOME/.local/share/nvim/style.css" # Path to the CSS file for styling
CONCURRENT_JOBS=4                            # Number of concurrent pandoc processes
LOG_FILE="$TEMP_DIR/conversion.log"          # Log file to track conversions
ERROR_LOG_FILE="$TEMP_DIR/error.log"         # Log file to track errors

# ============================
# Dependencies
# ============================

DEPENDENCIES=("pandoc" "sed" "qutebrowser" "find" "rsync" "grep" "mkdir" "basename" "diff")

# ============================
# Function Definitions
# ============================

# Function to check if all dependencies are installed
check_dependencies() {
    echo "Checking for required dependencies..."
    for cmd in "${DEPENDENCIES[@]}"; do
        if ! command -v "$cmd" &>/dev/null; then
            echo "Error: '$cmd' is not installed. Please install it and retry."
            exit 1
        fi
    done
    echo "All dependencies are satisfied."
}

# Function to extract the title from Markdown using YAML frontmatter or fallback to filename
extract_title() {
    local md_file="$1"
    # Attempt to extract title from YAML frontmatter
    local title
    title=$(grep -m1 '^title:' "$md_file" | sed 's/title: //') || true
    if [[ -z "$title" ]]; then
        # If no title found, use the filename without extension
        title=$(basename "$md_file" .md.old)
    fi
    echo "$title"
}

# Function to convert a single Markdown file to HTML
convert_md_to_html() {
    local md_old_file="$1"  # Path to the .md.old file
    # Determine the relative path from TEMP_DIR
    local relative_path="${md_old_file#$TEMP_DIR/}"
    # Determine the output HTML file path
    local html_file="$TEMP_DIR/${relative_path%.md.old}.html"
    # Create the necessary directories for the HTML file
    mkdir -p "$(dirname "$html_file")"
    
    # Extract the title for the HTML document
    local title
    title=$(extract_title "$md_old_file")
    
    echo "Converting '$md_old_file' to '$html_file'..."
    
    # Use pandoc to convert Markdown to HTML with CSS and metadata
    if [[ -f "$CSS_FILE" ]]; then
        pandoc -f markdown -s --css="$CSS_FILE" --metadata title="$title" "$md_old_file" -o "$html_file" || {
            echo "Error: Failed to convert '$md_old_file' to HTML." | tee -a "$ERROR_LOG_FILE"
            return 1
        }
    else
        echo "Warning: CSS file '$CSS_FILE' not found. Skipping CSS for '$md_old_file'."
        pandoc -f markdown -s --metadata title="$title" "$md_old_file" -o "$html_file" || {
            echo "Error: Failed to convert '$md_old_file' to HTML." | tee -a "$ERROR_LOG_FILE"
            return 1
        }
    fi
    
    # Debug: Print a snippet of the HTML file before running sed
    echo "Snippet before sed in '$html_file':"
    head -n 5 "$html_file"
    echo "..."
    
    # Adjust internal href links:
    # 1. Replace href="path/to/file.md.old" with href="path/to/file.html"
    # 2. Replace href="path/to/file" with href="path/to/file.html" only if 'file' has no extension
    echo "Adjusting links in '$html_file'..."
    
    # First, replace links ending with .md.old
    if ! sed -i -E 's|(href=")([^"#:/]+(/[^"#:/]+)*)\.md\.old(")|\1\2.html\4|g' "$html_file"; then
        echo "Error: Failed to adjust '.md.old' links in '$html_file'." | tee -a "$ERROR_LOG_FILE"
        echo "Snippet around problematic area in '$html_file':" | tee -a "$ERROR_LOG_FILE"
        grep -E 'href="[^"#:/]+\.md\.old"' "$html_file" | head -n 5 | tee -a "$ERROR_LOG_FILE" || true
        return 1
    fi
    
    # Then, replace links without any extension
    if ! sed -i -E 's|(href=")([^"#:/.]+(/[^"#:/.]+)*)(")|\1\2.html\4|g' "$html_file"; then
        echo "Error: Failed to adjust extensionless links in '$html_file'." | tee -a "$ERROR_LOG_FILE"
        echo "Snippet around problematic area in '$html_file':" | tee -a "$ERROR_LOG_FILE"
        grep -E 'href="[^"#:/.]+"' "$html_file" | head -n 5 | tee -a "$ERROR_LOG_FILE" || true
        return 1
    fi
    
    # Debug: Print a snippet of the HTML file after running sed
    echo "Snippet after sed in '$html_file':"
    head -n 5 "$html_file"
    echo "..."
    
    # Log the successful conversion
    echo "$(date +"%Y-%m-%d %H:%M:%S") - Converted '$md_old_file' to '$html_file'." >> "$LOG_FILE"
    
    echo "Successfully converted '$md_old_file' to '$html_file'."
}

# Function to synchronize Markdown files and relevant assets to TEMP_DIR
synchronize_markdown() {
    echo "Synchronizing Markdown files and assets to '$TEMP_DIR'..."
    
    # Use rsync to copy only .md, .pdf, and image files, excluding unwanted directories and files
    rsync -av --delete \
        --exclude='*.html' \
        --exclude='*.sh' \
        --exclude='.git/' \
        --exclude='.gitignore' \
        --exclude='*.bak' \
        --include='*/' \
        --include='*.md' \
        --include='*.pdf' \
        --include='*.png' \
        --include='*.jpg' \
        --include='*.jpeg' \
        --include='*.gif' \
        --exclude='*' \
        "$SOURCE_DIR/" "$TEMP_DIR/" | grep '\.md$' || true
    
    echo "Synchronization completed."
}

# Function to rename .md files to .md.old in TEMP_DIR
rename_md_files() {
    echo "Renaming new or modified .md files to .md.old in '$TEMP_DIR'..."
    
    # Find all .md files in TEMP_DIR
    find "$TEMP_DIR" -type f -name '*.md' | while IFS= read -r md_file; do
        # Determine the corresponding .md.old file
        md_old_file="${md_file}.old"
        
        # Determine the source .md file
        source_md="$SOURCE_DIR/${md_file#$TEMP_DIR/}"
        
        # Check if the .md.old file exists
        if [[ ! -f "$md_old_file" ]]; then
            # New file detected, copy to .md.old
            cp "$source_md" "$md_old_file"
            echo "New file detected. Copied '$source_md' to '$md_old_file'."
            # Convert to HTML
            convert_md_to_html "$md_old_file" &
        else
            # Compare the source .md with the existing .md.old
            if ! diff -q "$source_md" "$md_old_file" &>/dev/null; then
                # Files differ, update .md.old and reconvert
                cp "$source_md" "$md_old_file"
                echo "Modified file detected. Updated '$md_old_file' with changes from '$source_md'."
                # Convert to HTML
                convert_md_to_html "$md_old_file" &
            else
                echo "No changes detected for '$source_md'. Skipping conversion."
            fi
        fi
    done
    
    # Wait for all background conversions to finish
    wait
    
    echo "Renaming and conversion of new or modified .md files completed."
}

# Function to handle deletions: Remove .html files corresponding to deleted .md files
handle_deletions() {
    echo "Handling deletions of Markdown files..."
    
    # Find all .md.old files in TEMP_DIR
    find "$TEMP_DIR" -type f -name '*.md.old' | while IFS= read -r md_old_file; do
        # Determine the corresponding .md file in SOURCE_DIR
        source_md="$SOURCE_DIR/${md_old_file#$TEMP_DIR/}"
        source_md="${source_md%.md.old}.md"
        
        # Check if the source .md file exists
        if [[ ! -f "$source_md" ]]; then
            # Corresponding .md file has been deleted, remove the .html file
            html_file="${md_old_file%.md.old}.html"
            if [[ -f "$html_file" ]]; then
                rm "$html_file"
                echo "Deleted '$html_file' as the source Markdown file no longer exists."
                # Log the deletion
                echo "$(date +"%Y-%m-%d %H:%M:%S") - Deleted '$html_file' due to source removal." >> "$LOG_FILE"
            fi
            # Optionally, remove the .md.old file itself
            rm "$md_old_file"
            echo "Removed obsolete '$md_old_file'."
        fi
    done
    
    echo "Deletion handling completed."
}

# Function to generate index.html specifically
generate_index() {
    local index_md_old="$TEMP_DIR/index.md.old"
    local index_html="$TEMP_DIR/index.html"
    
    if [[ ! -f "$index_md_old" ]]; then
        echo "Error: 'index.md.old' not found in '$TEMP_DIR'."
        exit 1
    fi
    
    echo "Generating 'index.html' from 'index.md.old'..."
    
    # Convert the index.md.old file to HTML
    convert_md_to_html "$index_md_old"
    
    # Ensure index.html exists
    if [[ ! -f "$index_html" ]]; then
        echo "Error: Failed to generate 'index.html'."
        exit 1
    fi
    
    echo "'index.html' generation completed."
}

# Function to open index.html in qutebrowser
open_browser() {
    local index_file="$TEMP_DIR/index.html"
    if [[ -f "$index_file" ]]; then
        echo "Opening '$index_file' in qutebrowser..."
        qutebrowser "$index_file" &
        echo "Opened '$index_file' in qutebrowser."
    else
        echo "Error: '$index_file' does not exist. Please ensure it is generated correctly."
        exit 1
    fi
}

# Function to update synchronization and conversion based on differences
update_if_needed() {
    # Synchronize new and updated files
    synchronize_markdown
    # Rename and convert new or modified .md files
    rename_md_files
    # Handle deletions
    handle_deletions
    # Generate index.html
    generate_index
}

# ============================
# Main Script Execution
# ============================

main() {
    check_dependencies
    
    # Create TEMP_DIR if it doesn't exist
    if [[ ! -d "$TEMP_DIR" ]]; then
        echo "Temporary directory '$TEMP_DIR' does not exist. Creating and performing full synchronization."
        mkdir -p "$TEMP_DIR"
        synchronize_markdown
        rename_md_files
        handle_deletions
        generate_index
    else
        echo "Temporary directory '$TEMP_DIR' already exists. Checking for updates."
        update_if_needed
    fi
    
    open_browser
    
    echo "All tasks completed successfully."
}

main
