#!/bin/bash

# Define paths and requirements
CONVERTIONS_PATH="./convertions.py"
UTILS_PATH="./utils"
REQUIREMENTS_FILE="requirements.txt"
VENV_DIR="./venv"
VERSION=$(grep "VERSION = " $CONVERTIONS_PATH | cut -d '"' -f 2)
TARGET_BINARY="/usr/local/bin/convertions"
TEMP_BINARY="./dist/convertions"

# Step 1: Check for or set up the virtual environment and install dependencies
echo "Setting up virtual environment and checking dependencies..."
if [ ! -d "$VENV_DIR" ]; then
    python3 -m venv $VENV_DIR
fi
source $VENV_DIR/bin/activate

# Install requirements into the VENV if not already present
pip install -r $REQUIREMENTS_FILE || { echo "Error installing dependencies."; deactivate; exit 1; }

# Ensure PyInstaller is installed in the VENV
pip install pyinstaller || { echo "Error installing PyInstaller."; deactivate; exit 1; }
echo "Dependencies installed successfully in the virtual environment."

# Step 2: Build the binary using PyInstaller within the VENV
echo "Building binary with PyInstaller..."
pyinstaller --onefile --add-data "$UTILS_PATH:utils/" --name convertions $CONVERTIONS_PATH || { echo "Error creating binary."; deactivate; exit 1; }
deactivate
echo "Binary created successfully at $TEMP_BINARY"

# Step 3: Check for existing version in /usr/local/bin
if [ -f "$TARGET_BINARY" ]; then
    INSTALLED_VERSION=$($TARGET_BINARY --version | awk '{print $NF}')
    echo "Installed version: $INSTALLED_VERSION, New version: $VERSION"

    if [ "$VERSION" = "$INSTALLED_VERSION" ]; then
        echo "The installed version is up-to-date. No update required."
        exit 0
    else
        echo "Updating convertions to version $VERSION."
    fi
else
    echo "No existing version found in /usr/local/bin. Installing new version."
fi

# Step 4: Ask the user if they want to install the binary to /usr/local/bin
while true; do
    read -p "Do you want to install the binary to /usr/local/bin? (yes/no/cancel) " choice
    case $choice in
        yes )
            sudo mv "$TEMP_BINARY" "$TARGET_BINARY" && echo "Convertions installed at $TARGET_BINARY, version $VERSION."
            rm -rf build dist __pycache__ *.spec
            break
            ;;
        no )
            echo "Installation to /usr/local/bin skipped. Binary remains at $TEMP_BINARY."
            break
            ;;
        cancel )
            echo "Installation canceled. Cleaning up..."
            rm -rf build dist __pycache__ *.spec
            [ -d "$VENV_DIR" ] && rm -rf "$VENV_DIR"
            echo "Cleanup complete. Exiting."
            exit 0
            ;;
        * )
            echo "Please enter 'yes', 'no', or 'cancel'."
            ;;
    esac
done

echo "Installation complete. Binary is available at $TARGET_BINARY or in the current directory as $TEMP_BINARY if skipped."

