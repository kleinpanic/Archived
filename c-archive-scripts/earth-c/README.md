# Earth Renderer

A high-performance, pure C software renderer that generates realistic 3D wireframe visualizations of Earth with proper perspective, lighting, and atmospheric effects.

![Earth](docs/preview.jpg)

## Features

- **Pure C Implementation**: C11/C17 standard, minimal dependencies (libc + libm only by default)
- **Multiple Backends**: PPM image output, SDL2 window, Linux framebuffer (`/dev/fb0`)
- **High Performance**: Optimized to hit 60 FPS at 800×800 on modern CPUs
- **Beautiful Rendering**:
  - Xiaolin Wu anti-aliased line drawing
  - Perspective-correct projection with configurable FOV
  - Hidden line removal via depth buffer
  - Directional + ambient + rim lighting
  - Atmospheric effects (bloom, vignette)
  - Multiple render styles (blueprint, neon, moonlight, noir, etc.)
- **Procedural Generation**: Latitude/longitude grids, icosphere wireframes, procedural "coastlines"
- **Flexible CLI**: Record frame sequences, benchmark mode, various customization options

## Quick Start

### Build (Default - PPM backend only)

```bash
make
```

### Build with SDL2 (interactive window)

```bash
make sdl2
# or: make USE_SDL2=1
```

### Run

```bash
# Interactive window (requires SDL2 build)
./earth --backend sdl2

# Generate a single frame
./earth --out earth.ppm --record 1

# Record 600 frames (10 seconds @ 60fps)
./earth --width 800 --height 800 --out frames/frame_%05d.ppm --record 600

# Benchmark
./earth --benchmark --record 300

# Different styles
./earth --style neon --bloom 0.8 --backend sdl2
./earth --style blueprint --thickness 1.5 --backend sdl2
./earth --style moonlight --speed 0.5 --backend sdl2
```

## Build Options

| Target | Description |
|--------|-------------|
| `make` | Default build (PPM backend only) |
| `make sdl2` | Build with SDL2 support |
| `make fbdev` | Build with Linux framebuffer support |
| `make full` | Build with all backends |
| `make debug` | Debug build with sanitizers |
| `make test` | Build and run tests |

## Command-Line Options

### Display

- `--width <N>` - Frame width (default: 800)
- `--height <N>` - Frame height (default: 800)
- `--fps <N>` - Target FPS (default: 60)
- `--fov <F>` - Field of view in degrees (default: 55)

### Backend

- `--backend <TYPE>` - Backend: `ppm`, `sdl2`, `fbdev` (default: ppm)
- `--out <PATH>` - Output path for PPM backend (supports `%05d` format)
- `--device <PATH>` - Framebuffer device path (default: `/dev/fb0`)

### Geometry

- `--grid <N>` - Latitude/longitude grid divisions (default: 18)
- `--subdiv <N>` - Icosphere subdivisions for detail (default: 4)
- `--seed <N>` - Random seed for procedural features

### Rendering

- `--style <STYLE>` - Render style: `default`, `blueprint`, `neon`, `moonlight`, `terminal-green`, `noir`, `night`
- `--speed <F>` - Rotation speed multiplier (default: 0.8)
- `--thickness <F>` - Line thickness (default: 1.0)
- `--bloom <F>` - Bloom intensity 0-1 (default: 0.4)
- `--no-bloom` - Disable bloom effect
- `--no-atmosphere` - Disable atmosphere/vignette
- `--no-occlusion` - Disable hidden line removal
- `--scanlines` - Enable CRT scanline effect

### Recording

- `--record <N>` - Record N frames and exit
- `--demo` - Demo mode (10 second seamless loop)
- `--benchmark` - Print frame timing statistics

## Render Styles

- **default** - Clean white wireframe
- **blueprint** - Blueprint/technical drawing aesthetic
- **neon** - Cyberpunk neon glow
- **moonlight** - Soft moonlit grayscale
- **terminal-green** - Classic terminal phosphor
- **noir** - High-contrast black & white
- **night** - Warm "city lights at night" theme

## Performance

Typical performance on modern hardware:

- **800×800**: ~60 FPS (Core i5/Ryzen 5)
- **1920×1080**: ~30-45 FPS
- **400×400**: ~200+ FPS

See `docs/PERFORMANCE.md` for optimization details and profiling info.

## Architecture

```
┌─────────────┐
│  CLI/Main   │
└──────┬──────┘
       │
       ▼
┌─────────────┐      ┌──────────────┐
│  App Loop   │◄─────┤   Config     │
└──────┬──────┘      └──────────────┘
       │
       ▼
┌─────────────┐      ┌──────────────┐
│   Render    │◄─────┤  Geometry    │
│   Context   │      │  Generator   │
└──────┬──────┘      └──────────────┘
       │
       ├──► Project (Camera + Perspective)
       ├──► Lighting (Directional + Rim)
       ├──► Rasterize (Wu AA Lines + Depth)
       └──► Post-FX (Bloom + Vignette)
              │
              ▼
       ┌─────────────┐
       │ Framebuffer │
       └──────┬──────┘
              │
       ┌──────┴───────┬─────────────┐
       ▼              ▼             ▼
   ┌───────┐     ┌────────┐   ┌────────┐
   │  PPM  │     │  SDL2  │   │ fbdev  │
   └───────┘     └────────┘   └────────┘
```

See `docs/DESIGN.md` for detailed pipeline explanation.

## Examples

### Generate HD animation frames

```bash
mkdir frames
./earth --width 1920 --height 1080 \
        --out frames/earth_%05d.ppm \
        --style blueprint \
        --record 300

# Convert to video (requires ffmpeg)
ffmpeg -framerate 60 -i frames/earth_%05d.ppm -c:v libx264 -pix_fmt yuv420p earth.mp4
```

### Interactive demos

```bash
# Neon cyberpunk Earth
./earth --backend sdl2 --style neon --bloom 1.0 --speed 1.2 --thickness 1.5

# Slow moonlit rotation
./earth --backend sdl2 --style moonlight --speed 0.3

# High-detail technical diagram
./earth --backend sdl2 --style blueprint --grid 36 --subdiv 5 --speed 0.5
```

## Dependencies

### Required (always)
- C11 compiler (gcc, clang)
- libc, libm

### Optional
- SDL2 (for `--backend sdl2`)
- Linux with `/dev/fb0` (for `--backend fbdev`)

## License

See `LICENSE` file.

## Documentation

- [Design & Pipeline](docs/DESIGN.md) - Math, rendering pipeline, algorithms
- [Performance](docs/PERFORMANCE.md) - Profiling, optimization notes, benchmarks
- [Demo Guide](docs/DEMO.md) - Example commands, tips & tricks

## Credits

Built from scratch as a complete ground-up rewrite.

Rendering techniques:
- Xiaolin Wu anti-aliased lines
- Perspective-correct depth buffering
- Icosphere geodesic subdivision
- Procedural noise-based coastlines
