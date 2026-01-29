#!/bin/bash
# Convert PPM frame sequence to animated GIF

set -e

if [ $# -lt 2 ]; then
    echo "Usage: $0 <frame_pattern> <output.gif> [fps] [width]"
    echo ""
    echo "Examples:"
    echo "  $0 'frames/frame_%05d.ppm' output.gif"
    echo "  $0 'frames/frame_%05d.ppm' output.gif 30 600"
    exit 1
fi

PATTERN="$1"
OUTPUT="$2"
FPS="${3:-30}"
WIDTH="${4:-800}"

if ! command -v ffmpeg &> /dev/null; then
    echo "Error: ffmpeg not found. Please install ffmpeg."
    exit 1
fi

echo "Converting frames to GIF..."
echo "  Pattern: $PATTERN"
echo "  Output: $OUTPUT"
echo "  FPS: $FPS"
echo "  Width: $WIDTH"
echo ""

ffmpeg -framerate "$FPS" -i "$PATTERN" \
    -vf "fps=$FPS,scale=$WIDTH:-1:flags=lanczos,split[s0][s1];[s0]palettegen=max_colors=256[p];[s1][p]paletteuse=dither=bayer:bayer_scale=5" \
    -loop 0 \
    -y \
    "$OUTPUT"

echo ""
echo "Done! Created: $OUTPUT"
ls -lh "$OUTPUT"
