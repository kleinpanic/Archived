# Performance Guide

## Benchmarking

### Running Benchmarks

```bash
# Default resolution benchmark
./earth --benchmark --record 300

# High resolution
./earth --width 1920 --height 1080 --benchmark --record 300

# Stress test
./earth --width 3840 --height 2160 --grid 36 --subdiv 6 --benchmark --record 100
```

Output example:

```
Frame 60: 15.23 ms avg, 65.6 FPS
Frame 120: 15.31 ms avg, 65.3 FPS
Frame 180: 15.28 ms avg, 65.4 FPS
...
=== Benchmark Results ===
Frames rendered: 300
Avg frame time: 15.27 ms
Avg FPS: 65.5
Max frame time: 18.92 ms
Min FPS: 52.9
```

## Performance Characteristics

### Resolution Scaling

Performance is primarily bound by:

1. **Pixel fillrate** (line rasterization)
2. **Line count** (geometry complexity)
3. **Post-processing** (bloom blur)

| Resolution | Lines | Typical FPS (i5-10400) |
|------------|-------|------------------------|
| 400×400    | 2240  | ~250 FPS               |
| 800×800    | 2240  | ~80 FPS                |
| 1920×1080  | 2240  | ~35 FPS                |
| 3840×2160  | 2240  | ~9 FPS                 |

### Geometry Complexity

Line count impact:

| Config | Line Count | FPS @ 800×800 |
|--------|------------|---------------|
| `--grid 12 --subdiv 3` | ~1500 | ~95 |
| `--grid 18 --subdiv 4` (default) | ~2240 | ~80 |
| `--grid 24 --subdiv 5` | ~4800 | ~45 |
| `--grid 36 --subdiv 6` | ~12000 | ~20 |

## Optimization Notes

### Compiler Flags

The Makefile uses aggressive optimization:

```makefile
CFLAGS := -std=c11 -O3 -march=native -Wall -Wextra
```

- `-O3`: Maximum optimization (inlining, loop unrolling, vectorization)
- `-march=native`: Use CPU-specific instructions (SSE4.2, AVX, etc.)

To verify vectorization:

```bash
gcc -O3 -march=native -Iinclude -fopt-info-vec-optimized -c src/raster.c
```

### Hotspots

Profiling with `perf` on Linux:

```bash
make clean && make CFLAGS="-O3 -march=native -g -Iinclude"
perf record -g ./earth --benchmark --record 300
perf report
```

Typical profile:

```
  45%  raster_line_aa       (line rasterization)
  18%  render_project_geometry (projection & lighting)
  12%  render_apply_bloom   (post-processing blur)
   8%  fb_clear_depth       (depth buffer clear)
   7%  geom_transform       (matrix-vector multiply)
  10%  other
```

### Memory Access Patterns

- **Framebuffer**: Linear access in rasterizer (good cache behavior)
- **Depth buffer**: Same access pattern as framebuffer (cache-friendly)
- **Geometry**: Iteration over line array (sequential, prefetcher-friendly)

Avoid:
- Random access patterns
- Excessive branching in inner loops
- Heap allocation per-frame

### Algorithmic Complexity

Per-frame cost:

```
O(lines) for projection & lighting
O(lines × avg_line_length) for rasterization
O(width × height × blur_radius²) for bloom
```

Dominant term at high resolution: `O(lines × avg_line_length)`

At high geometry density: Projection can become bottleneck.

## Optimization Opportunities

### Already Implemented

✓ Aggressive compiler optimization (`-O3 -march=native`)
✓ Fixed timestep (decoupled from render)
✓ Backface culling (reduces lines by ~50%)
✓ Minimal heap allocation in inner loop
✓ Tight inner loops in rasterizer
✓ Depth buffer occlusion

### Future Work (Not Yet Implemented)

1. **SIMD Math Library**
   - Vectorize vec3/mat4 operations
   - Process 4 vertices at once with SSE/AVX
   - Estimated gain: 2-4× on projection

2. **Multi-threading**
   - Tile-based rasterization (divide screen into NxN tiles)
   - One thread per tile
   - Requires per-tile depth buffer or atomic operations
   - Estimated gain: 2-8× on multi-core CPUs

3. **Separable Blur**
   - Current bloom: O(radius²) per pixel
   - Separable: O(2 × radius) per pixel (horizontal + vertical pass)
   - Estimated gain: 3-5× on bloom pass

4. **Line Clipping**
   - Clip lines to screen bounds before rasterization
   - Avoids wasted work on off-screen pixels
   - Estimated gain: 10-20% when zoomed in

5. **Adaptive LOD**
   - Reduce geometry density near limb (small projected lines)
   - More detail near center
   - Estimated gain: 20-30% in complex scenes

6. **Fixed-Point Math** (for embedded targets)
   - Replace float with int32_t fixed-point
   - Useful for platforms without FPU
   - Trade: precision vs. performance

## Power Users: Manual Tuning

### Disable Expensive Features

```bash
# No bloom (saves ~12% frame time)
./earth --no-bloom

# No occlusion (saves ~8% but looks worse)
./earth --no-occlusion

# Minimal geometry
./earth --grid 12 --subdiv 2
```

### Match FPS to Display

```bash
# 30 FPS (better for video encoding)
./earth --fps 30

# 144 FPS (for high-refresh displays)
./earth --fps 144 --backend sdl2
```

### Record Mode Performance

Recording (PPM output) adds I/O overhead:

```bash
# Write to fast SSD or tmpfs
./earth --out /tmp/frames/frame_%05d.ppm --record 600

# Use ramdisk
mkdir /mnt/ramdisk
mount -t tmpfs -o size=2G tmpfs /mnt/ramdisk
./earth --out /mnt/ramdisk/frame_%05d.ppm --record 600
```

## Profiling Tools

### Linux

**perf**:
```bash
perf stat ./earth --benchmark --record 300
perf record -g ./earth --benchmark --record 300
perf report
```

**valgrind (cachegrind)**:
```bash
valgrind --tool=cachegrind ./earth --record 10
cg_annotate cachegrind.out.<pid>
```

### macOS

**Instruments**:
```bash
instruments -t "Time Profiler" ./earth --benchmark --record 300
```

### Cross-platform

**gprof**:
```bash
make clean
make CFLAGS="-O3 -march=native -pg -Iinclude" LDFLAGS="-pg -lm"
./earth --benchmark --record 300
gprof earth gmon.out > profile.txt
```

## Target Metrics

### Acceptable Performance

- **Interactive use**: ≥30 FPS
- **Smooth animation**: ≥60 FPS
- **Recording**: ≥15 FPS (depends on I/O)

### Bottleneck Identification

If FPS is low:

1. **Check line count**: `--grid` and `--subdiv` have multiplicative impact
2. **Resolution**: Halve width/height, see if FPS quadruples → fillrate bound
3. **Disable bloom**: If FPS jumps >15%, blur is bottleneck
4. **Profile**: Use `perf` to see where time is spent

## Hardware Recommendations

### Minimum

- CPU: Dual-core 2.0+ GHz (e.g., Raspberry Pi 4)
- RAM: 128 MB
- Resolution: 400×400, ~30 FPS achievable

### Recommended

- CPU: Quad-core 3.0+ GHz (e.g., Intel i5-8xxx, Ryzen 5 3600)
- RAM: 256 MB
- Resolution: 800×800 @ 60 FPS, 1920×1080 @ 30 FPS

### High-End

- CPU: 8+ cores 4.0+ GHz (e.g., Intel i9, Ryzen 9, Threadripper)
- RAM: 512 MB
- Resolution: 3840×2160 @ 30 FPS (with optimizations)

*Note: Current implementation is single-threaded, so single-core performance matters most.*

## Energy Efficiency

For embedded/battery-powered:

```bash
# Lower resolution, lower FPS
./earth --width 320 --height 240 --fps 30

# Minimal geometry
./earth --grid 12 --subdiv 2

# Disable post-processing
./earth --no-bloom --no-atmosphere
```

Use `powertop` (Linux) or `powermetrics` (macOS) to measure power consumption.

## Comparison to GPU Rendering

This is a **CPU software rasterizer**. A comparable GPU implementation would be:

- 50-100× faster at high resolutions
- 10× faster at low resolutions

Software rendering advantages:

- No GPU required (works everywhere)
- Deterministic output
- No driver issues
- Educational value

Use cases:

- Headless servers
- Embedded systems without GPU
- Retro/aesthetic software rendering
- Learning computer graphics fundamentals
