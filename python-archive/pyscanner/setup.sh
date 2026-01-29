#!/usr/bin/env bash

PROJECT_DIR="$HOME/.local/share/pyscanner"
BIN="$HOME/.local/bin"

mkdir -p "$PROJECT_DIR"
cp pyscanner.py "$PROJECT_DIR"

echo "Setting up virtual environment"
cd "$PROJECT_DIR"
python3 -m venv venv
source venv/bin/activate
pip install imutils numba opencv-python
deactivate

echo "Creating executable"
cat > "$BIN/pyscanner" << EOL
#!/usr/bin/env bash

source "$PROJECT_DIR/venv/bin/activate"
python3 "$PROJECT_DIR/pyscanner.py" "\$@"
EOL

chmod +x "$BIN/pyscanner"
