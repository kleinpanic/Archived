# Demo Guide & Examples

## Quick Demos

### 1. Minimal Setup (PPM single frame)

```bash
./earth --out earth.ppm --record 1
# View with: feh earth.ppm, display earth.ppm, or convert to PNG
```

### 2. Interactive Window (SDL2)

```bash
make sdl2
./earth --backend sdl2
# Press ESC to quit
```

### 3. Blueprint Style Animation

```bash
mkdir -p frames
./earth --style blueprint \
        --width 800 --height 800 \
        --out frames/frame_%05d.ppm \
        --record 300 \
        --speed 0.6
```

## Full Example Recipes

### Neon Cyberpunk Earth

```bash
./earth --backend sdl2 \
        --style neon \
        --bloom 1.0 \
        --speed 1.2 \
        --thickness 1.5 \
        --grid 24 \
        --subdiv 5
```

Creates a vibrant, glowing wireframe with lots of detail.

### Slow Moonlit Rotation

```bash
./earth --backend sdl2 \
        --style moonlight \
        --speed 0.3 \
        --bloom 0.3 \
        --grid 18
```

Calm, elegant grayscale animation.

### Terminal Green Retro

```bash
./earth --backend sdl2 \
        --style terminal-green \
        --scanlines \
        --speed 0.8 \
        --thickness 1.2
```

Classic computer terminal aesthetic with scanlines.

### High-Contrast Noir

```bash
./earth --backend sdl2 \
        --style noir \
        --no-bloom \
        --speed 0.5 \
        --thickness 1.0
```

Clean black & white, no glow.

### Earth at Night (City Lights)

```bash
./earth --backend sdl2 \
        --style night \
        --bloom 0.8 \
        --speed 0.4 \
        --grid 12 \
        --seed 42
```

Warm orange/yellow lines suggesting city lights.

## Generating Video

### Method 1: FFmpeg (H.264)

```bash
# Render frames
mkdir frames
./earth --width 1920 --height 1080 \
        --out frames/frame_%05d.ppm \
        --record 600 \
        --style blueprint

# Convert to video
ffmpeg -framerate 60 -i frames/frame_%05d.ppm \
       -c:v libx264 -crf 18 -pix_fmt yuv420p \
       earth.mp4
```

### Method 2: Animated GIF

```bash
# Render frames
./earth --width 600 --height 600 \
        --out frames/frame_%05d.ppm \
        --record 120 \
        --style neon

# Convert to GIF
ffmpeg -i frames/frame_%05d.ppm \
       -vf "fps=30,scale=600:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" \
       -loop 0 earth.gif
```

### Method 3: WebM (web-friendly)

```bash
ffmpeg -framerate 60 -i frames/frame_%05d.ppm \
       -c:v libvpx-vp9 -crf 30 -b:v 0 \
       earth.webm
```

## High-Resolution Rendering

### 4K (3840×2160)

```bash
./earth --width 3840 --height 2160 \
        --out 4k_earth.ppm \
        --record 1 \
        --style blueprint \
        --grid 36 \
        --subdiv 6 \
        --thickness 1.5

# Convert to PNG for easier viewing
ffmpeg -i 4k_earth.ppm 4k_earth.png
```

**Warning**: 4K rendering is slow (~5-10 FPS). Expect 1-2 minutes per frame with high detail.

### Poster Print (8K)

```bash
./earth --width 7680 --height 4320 \
        --out 8k_earth.ppm \
        --record 1 \
        --style moonlight \
        --grid 48 \
        --subdiv 7 \
        --thickness 2.0

ffmpeg -i 8k_earth.ppm -q:v 1 8k_earth.jpg
```

**Warning**: Extremely slow. Consider rendering overnight.

## Seamless Loops

For perfectly looping animations, use multiples of rotation period:

```bash
# One full rotation (assuming default speed 0.8)
# Math: 2π / 0.8 ≈ 7.85 seconds ≈ 471 frames @ 60fps

./earth --out frames/loop_%05d.ppm \
        --record 471 \
        --style neon

# Video will loop seamlessly
ffmpeg -framerate 60 -i frames/loop_%05d.ppm \
       -c:v libx264 -crf 18 -pix_fmt yuv420p \
       -vf "loop=3:32767:0" \
       looping_earth.mp4
```

## Batch Rendering Multiple Styles

```bash
#!/bin/bash
styles=("default" "blueprint" "neon" "moonlight" "terminal-green" "noir" "night")

for style in "${styles[@]}"; do
    echo "Rendering $style..."
    ./earth --width 800 --height 800 \
            --out "renders/${style}.ppm" \
            --record 1 \
            --style "$style"
done
```

## Linux Framebuffer (Raspberry Pi, etc.)

```bash
# Build with fbdev support
make fbdev

# Run on framebuffer (requires root or proper permissions)
sudo ./earth --backend fbdev --device /dev/fb0

# Or set permissions
sudo chown $USER /dev/fb0
./earth --backend fbdev
```

**Note**: May interfere with X11/Wayland. Best used in console mode (Ctrl+Alt+F1).

## Benchmark & Optimize

### Find Optimal Settings for Your Hardware

```bash
# Test different resolutions
for res in 400 600 800 1000 1200; do
    echo "Testing ${res}x${res}..."
    ./earth --width $res --height $res \
            --benchmark --record 100 2>&1 | grep "Avg FPS"
done

# Test geometry complexity
for grid in 12 18 24 30 36; do
    echo "Testing grid=$grid..."
    ./earth --grid $grid --benchmark --record 100 2>&1 | grep "Avg FPS"
done
```

### Stress Test

```bash
# Maximum detail, see how far you can push
./earth --width 1920 --height 1080 \
        --grid 48 --subdiv 7 \
        --bloom 1.0 \
        --benchmark --record 60
```

## Tips & Tricks

### 1. Deterministic Output

Use `--seed` for reproducible procedural coastlines:

```bash
./earth --seed 12345 --out earth_a.ppm --record 1
./earth --seed 12345 --out earth_b.ppm --record 1
# earth_a.ppm and earth_b.ppm are identical
```

### 2. Time-lapse Speed

Fast rotation for time-lapse effect:

```bash
./earth --speed 5.0 --backend sdl2
```

### 3. Minimalist Look

```bash
./earth --grid 12 --subdiv 0 \
        --no-bloom --no-atmosphere \
        --style noir
```

Just the essential latitude/longitude grid.

### 4. Maximum Detail

```bash
./earth --grid 36 --subdiv 6 \
        --seed 999 \
        --style default
```

Dense wireframe with complex procedural features.

### 5. Fast Preview

```bash
./earth --width 320 --height 240 --backend sdl2 --fps 30
```

Low resolution for quick preview/testing.

## Troubleshooting

### "SDL2 backend not available"

Build with SDL2:
```bash
make clean && make sdl2
```

Or install SDL2:
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev

# macOS
brew install sdl2

# Fedora
sudo dnf install SDL2-devel
```

### Slow Performance

1. Lower resolution: `--width 600 --height 600`
2. Reduce geometry: `--grid 12 --subdiv 2`
3. Disable effects: `--no-bloom --no-atmosphere`
4. Rebuild optimized: `make clean && make`

### Output Files Too Large

PPM files are uncompressed. Convert to PNG:

```bash
ffmpeg -i earth.ppm earth.png
# Or batch convert
for f in frames/*.ppm; do
    ffmpeg -i "$f" "${f%.ppm}.png"
done
```

## Advanced: Custom Render Pipeline

Modify source code for custom effects:

### Add Custom Style

Edit `src/render.c`, function `get_style_color()`:

```c
case STYLE_CUSTOM:
    return pack_color(i/2, i, i/2, 255); // cyan-ish
```

Add to `include/earth/config.h`:

```c
typedef enum {
    ...
    STYLE_CUSTOM
} render_style_t;
```

### Change Rotation Axis

Edit `src/render.c`, function `render_frame()`:

```c
// Current: Y-axis primary, X-axis secondary
mat4_t rot_y = mat4_rotate_y(ctx->rotation_angle);
mat4_t rot_x = mat4_rotate_x(ctx->rotation_angle * 0.3f);

// Custom: Z-axis wobble
mat4_t rot_z = mat4_rotate_z(sinf(ctx->rotation_angle) * 0.5f);
```

### Add Custom Geometry

Edit `src/app.c`, function `app_create()`:

```c
// Add great circles
geom_add_great_circles(app->geometry, 1.0f, 6);
```

## Further Inspiration

- Animate camera distance: modify `ctx->camera_pos` over time
- Pulsating lines: modulate `line_thickness` with `sin(time)`
- Color cycling: interpolate between styles based on time
- Particle trails: accumulate frames with decay (temporal blur)

## Community Examples

Share your renders! Post to:
- `/r/proceduralgeneration`
- `/r/generative`
- Twitter: `#WireframeEarth`
