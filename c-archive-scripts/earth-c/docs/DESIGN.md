# Design & Architecture

## Overview

Earth Renderer is a software rasterizer that produces convincing 3D wireframe visualizations using a classic graphics pipeline implemented entirely in C.

## Rendering Pipeline

```
┌──────────────────────────────────────────────────────────────┐
│                     GEOMETRY GENERATION                       │
│  ┌────────────┐  ┌─────────────┐  ┌──────────────────────┐  │
│  │ Lat/Long   │  │ Icosphere   │  │ Procedural           │  │
│  │ Grid       │  │ Wireframe   │  │ Coastlines           │  │
│  └────────────┘  └─────────────┘  └──────────────────────┘  │
│         │                │                    │                │
│         └────────────────┴────────────────────┘                │
│                          │                                     │
│                          ▼                                     │
│                    [ Line Segments ]                           │
│                     (world space)                              │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                    MODEL TRANSFORMATION                        │
│                (rotation, animation state)                     │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                    VIEW TRANSFORMATION                         │
│              (camera position, look-at matrix)                 │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                  PROJECTION TRANSFORMATION                     │
│           (perspective matrix, FOV, aspect ratio)              │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                    BACKFACE CULLING                            │
│         (reject lines facing away from camera)                 │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                    LIGHTING COMPUTATION                        │
│     - Directional light (sun-like source)                      │
│     - Ambient term (constant base illumination)                │
│     - Rim lighting (atmospheric glow at limb)                  │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│              VIEWPORT TRANSFORM & CLIPPING                     │
│        (NDC [-1,1] → screen pixels [0, width/height])          │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                    LINE RASTERIZATION                          │
│  - Xiaolin Wu anti-aliased line drawing                        │
│  - Depth buffer test per-pixel                                 │
│  - Alpha blending for sub-pixel coverage                       │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                    POST-PROCESSING                             │
│  - Bloom (box blur + additive blend)                           │
│  - Vignette (radial darkening)                                 │
│  - Optional: scanlines, motion blur                            │
└──────────────────────────────────────────────────────────────┘
                             │
                             ▼
                      [ Final Image ]
```

## Mathematics

### Coordinate Systems

1. **World Space**: 3D coordinates where the sphere is centered at origin
2. **View Space**: Coordinates relative to the camera
3. **Clip Space**: Homogeneous coordinates after perspective projection
4. **NDC (Normalized Device Coordinates)**: [-1, 1] cube after perspective divide
5. **Screen Space**: Integer pixel coordinates [0, width) × [0, height)

### Projection Matrix (Perspective)

```
Given:
  fov   = field of view (degrees)
  aspect = width / height
  near, far = near and far clipping planes

Let:
  f = 1 / tan(fov * π / 360)

Matrix:
  ┌                                          ┐
  │  f/aspect    0         0            0    │
  │     0        f         0            0    │
  │     0        0   -(f+n)/(f-n)  -2fn/(f-n)│
  │     0        0        -1            0    │
  └                                          ┘
```

After applying this matrix to a point `(x, y, z, 1)`, we get `(x', y', z', w')`.

Perspective divide: `(x'/w', y'/w', z'/w')` gives NDC coordinates.

### Look-At Matrix (View)

```
Given:
  eye    = camera position
  center = look-at target
  up     = up vector

Compute:
  f = normalize(center - eye)  // forward
  s = normalize(cross(f, up))  // right
  u = cross(s, f)              // up

Matrix:
  ┌                              ┐
  │  s.x   s.y   s.z   -dot(s,e) │
  │  u.x   u.y   u.z   -dot(u,e) │
  │ -f.x  -f.y  -f.z    dot(f,e) │
  │   0     0     0        1     │
  └                              ┘
```

### Lighting Model

For a point on the sphere at position `P`:

```
normal = normalize(P)  // sphere centered at origin
intensity = ambient + diffuse * max(0, dot(normal, light_dir))
            + rim * pow(1 - |dot(normal, view_dir)|, 3)

where:
  ambient = 0.3        // base illumination
  diffuse = 0.7        // directional contribution
  rim     = 0.4        // atmospheric glow factor
```

The rim lighting term creates the characteristic glow near the limb (edge) of the sphere.

## Line Rasterization: Xiaolin Wu Algorithm

Traditional line drawing (Bresenham) produces aliased, jagged lines. Xiaolin Wu's algorithm provides anti-aliasing by computing fractional coverage.

### Algorithm Outline

1. Ensure line is not steep (swap x/y if needed)
2. Ensure left-to-right (swap endpoints if needed)
3. Compute gradient: `dy/dx`
4. For each x coordinate along the line:
   - Compute y position: `y = y0 + gradient * (x - x0)`
   - Split into integer and fractional parts
   - Draw two pixels at `floor(y)` and `ceil(y)` with alpha based on fractional part

### Depth Buffer Integration

Each pixel has an associated depth value. When rasterizing:

```
for each pixel (x, y) with coverage alpha:
    z_line = interpolate depth along line
    if z_line < depth_buffer[x, y]:
        depth_buffer[x, y] = z_line
        color_buffer[x, y] = blend(color_buffer[x, y], line_color, alpha)
```

This prevents back-facing lines from drawing over front-facing ones.

## Geometry Generation

### Latitude/Longitude Grid

Classic spherical grid. For a sphere of radius `R`:

```
Point(lat, lon) = (
    R * cos(lat) * cos(lon),
    R * sin(lat),
    R * cos(lat) * sin(lon)
)
```

Connect adjacent grid points to form lines.

### Icosphere Wireframe

Start with an icosahedron (20 faces, 12 vertices), then subdivide edges:

1. Take each edge
2. Split into N segments
3. Project each intermediate point onto sphere surface (normalize and scale)

This provides uniform triangle distribution, unlike UV spheres which cluster at poles.

### Procedural Coastlines

Random walk on sphere surface:

1. Start at random point (lat, lon)
2. For N steps:
   - Add small random offset to lat/lon
   - Clamp latitude to avoid poles
   - Wrap longitude
3. Convert (lat, lon) path to 3D line segments

Produces organic "coastline-like" curves without requiring geographic data.

## Occlusion / Hidden Line Removal

### Backface Culling

For each line segment, compute the midpoint and surface normal:

```
midpoint = (start + end) / 2
normal = normalize(midpoint)  // sphere centered at origin
view_dir = normalize(camera_pos - midpoint)

if dot(normal, view_dir) < 0:
    reject line (back-facing)
```

### Depth Buffer

Complement backface culling with per-pixel depth testing:

- Each pixel stores nearest depth seen so far
- When drawing a line pixel, compare `z_line` to `z_buffer[x, y]`
- Only update pixel if `z_line` is closer

This handles cases where lines from the back hemisphere wrap around the limb.

## Post-Processing Effects

### Bloom

Simulates light bleeding/glow:

1. Blur the image (box blur or gaussian)
2. Blend blurred result additively with original
3. Intensity controlled by `bloom_intensity` parameter

Simple box blur implementation:

```
for each pixel (x, y):
    sum = 0
    for dx in [-radius, radius]:
        for dy in [-radius, radius]:
            sum += image[x+dx, y+dy]
    blurred[x, y] = sum / ((2*radius+1)^2)
```

### Vignette

Darken edges:

```
for each pixel (x, y):
    dist = distance_to_center(x, y) / max_distance
    factor = 1 - dist * strength
    pixel *= factor
```

### Scanlines (Optional)

Darken every other line to simulate CRT scanlines:

```
for y in [0, height) step 2:
    for x in [0, width):
        pixel[x, y] *= 0.9
```

## Performance Considerations

### Hotspots

1. **Line rasterization**: Most CPU time spent here
   - Optimize inner loop
   - Minimize conditionals
   - Compiler auto-vectorization hints (`-O3 -march=native`)

2. **Depth buffer access**: Cache-friendly access patterns matter

3. **Post-processing**: Blur is O(width × height × radius²)
   - Use separable filter (future optimization)
   - Limit blur radius

### Memory Layout

- **Structure-of-Arrays (SoA)** for geometry can improve cache locality
- **Framebuffer**: Contiguous RGBA8888 pixels
- **Depth buffer**: Separate float array (parallel access)

### Fixed Timestep

Application uses fixed timestep update:

```
accumulator += delta_time
while accumulator >= frame_time:
    update(frame_time)
    accumulator -= frame_time
```

This decouples rendering from wall-clock time, ensuring deterministic animation.

## Future Enhancements

- **SIMD**: Vectorize math operations (SSE/AVX for x86, NEON for ARM)
- **Multi-threading**: Tile-based rasterization across threads
- **Adaptive LOD**: Reduce geometry density at limb edges
- **Temporal anti-aliasing**: Accumulate multiple frames with jitter
- **Better blur**: Separable gaussian or compute-efficient approximations

## References

- Xiaolin Wu's line algorithm: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
- Perspective projection: https://en.wikipedia.org/wiki/3D_projection
- Icosahedron: https://en.wikipedia.org/wiki/Icosahedron
- Depth buffer: https://en.wikipedia.org/wiki/Z-buffering
