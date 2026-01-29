import pygame
import tkinter as tk
from tkinter import ttk
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np
import noise
import threading

# Constants
G = 6.67430e-11  # Gravitational constant in m^3 kg^-1 s^-2
SUN_MASS = 1.989e30  # Mass of the sun in kilograms (kg)
AU = 1.496e11  # Astronomical Unit in meters (average distance from Earth to Sun)

# Scaling factors for visualization
DISTANCE_SCALE = 1e-9  # Scale down distances (from meters to a more manageable size)
RADIUS_SCALE = 1e-6  # Scale down radii (from meters to a more manageable size)

# Class to represent each celestial object (planet or sun)
class CelestialObject:
    def __init__(self, name, radius, distance, rotation_speed, mass, color, draw_func=None, vis_radius=None):
        self.name = name
        self.radius = radius * RADIUS_SCALE  # Apply radius scaling
        self.distance = distance * AU * DISTANCE_SCALE  # Apply distance scaling if it's a planet
        self.vis_radius = vis_radius if vis_radius else self.radius  # Visualization radius
        self.orbital_speed = self.calculate_orbital_speed() if distance > 0 else 0
        self.rotation_speed = rotation_speed
        self.mass = mass
        self.density = self.calculate_density()  # Calculate density based on radius and mass
        self.color = color
        self.angle = 0  # Initial orbital angle
        self.is_wireframe = False  # To display the planet as a wireframe
        self.draw_func = draw_func if draw_func else self.default_draw

        # Store default values
        self.default_values = {
            "radius": radius,
            "distance": distance,
            "orbital_speed": self.orbital_speed,
            "rotation_speed": rotation_speed,
            "mass": mass,
            "density": self.density
        }

    def draw(self):
        glPushMatrix()
        if self.distance > 0:
            glRotatef(self.angle * 180.0 / np.pi, 0, 1, 0)  # Orbit around the Y-axis
            glTranslatef(self.distance, 0, 0)  # Move to the planet's orbital position
        glRotatef(self.angle * 180.0 / np.pi, 0, 1, 0)  # Rotate the object on its own axis

        if self.is_wireframe:
            glColor3f(1.0, 1.0, 0.0)  # Yellow for wireframe
            glLineWidth(2)
            draw_wireframe_sphere(self.vis_radius + 0.1)
        else:
            self.draw_func()

        glPopMatrix()

    def update_position(self):
        if self.distance > 0:
            self.angle += self.orbital_speed

    def default_draw(self):
        glColor3f(*self.color)
        draw_sphere(self.vis_radius)

    def restore_defaults(self):
        """Restore the object's properties to their default values."""
        self.radius = self.default_values["radius"] * RADIUS_SCALE
        self.distance = self.default_values["distance"] * AU * DISTANCE_SCALE
        self.orbital_speed = self.calculate_orbital_speed() if self.distance > 0 else 0
        self.rotation_speed = self.default_values["rotation_speed"]
        self.mass = self.default_values["mass"]
        self.density = self.default_values["density"]

    def calculate_density(self):
        """Calculate the density of the object based on its mass and radius."""
        volume = (4/3) * np.pi * self.radius**3
        return self.mass / volume

    def update_mass_based_on_density(self):
        """Update the mass based on the current density and radius."""
        volume = (4/3) * np.pi * self.radius**3
        self.mass = self.density * volume

    def update_density_based_on_mass(self):
        """Update the density based on the current mass and radius."""
        self.density = self.calculate_density()

    def calculate_orbital_speed(self):
        """Calculate the orbital speed based on the distance from the sun."""
        return np.sqrt(G * SUN_MASS / (self.distance / DISTANCE_SCALE)) * 86400 / (2 * np.pi * AU)  # Convert to AU/day

# Function to draw the Earth with water, land, and poles
def draw_earth(radius, land_density=0.2):
    segments = 64
    scale = 5.0  # Scaling for noise generation

    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)

    for i in range(segments):
        lat0 = np.pi * (-0.5 + float(i) / segments)
        z0 = np.sin(lat0) * radius
        zr0 = np.cos(lat0) * radius

        lat1 = np.pi * (-0.5 + float(i + 1) / segments)
        z1 = np.sin(lat1) * radius
        zr1 = np.cos(lat1) * radius

        glBegin(GL_QUAD_STRIP)
        for j in range(segments + 1):
            lng = 2 * np.pi * float(j) / segments
            x = np.cos(lng)
            y = np.sin(lng)

            if i < 8 or i > segments - 9:  # Perfect circular poles
                glColor3f(1.0, 1.0, 1.0)  # White for poles
                glVertex3f(x * zr0, y * zr0, z0)
                glVertex3f(x * zr1, y * zr1, z1)
            else:
                # Apply Perlin noise to create land patches
                noise_value = noise.pnoise3(x * scale, y * scale, z0 * scale)
                if noise_value > (0.5 - land_density):  # Land (brown/green)
                    glColor3f(0.4, 0.8, 0.4)
                    glVertex3f(x * zr0, y * zr0, z0)
                    glVertex3f(x * zr1, y * zr1, z1)
                else:  # Water (blue)
                    glColor3f(0.0, 0.0, 1.0)
                    glVertex3f(x * zr0, y * zr0, z0)
                    glVertex3f(x * zr1, y * zr1, z1)
        glEnd()

    glDisable(GL_CULL_FACE)

# Function to draw a wireframe sphere
def draw_wireframe_sphere(radius):
    """ Draws a wireframe sphere to highlight selected planets with fewer wires """
    segments = 16  # Reduced number of segments for more spacing
    for i in range(segments):
        lat0 = np.pi * (-0.5 + float(i) / segments)
        z0 = np.sin(lat0) * radius
        zr0 = np.cos(lat0) * radius

        lat1 = np.pi * (-0.5 + float(i + 1) / segments)
        z1 = np.sin(lat1) * radius
        zr1 = np.cos(lat1) * radius

        glBegin(GL_LINE_LOOP)
        for j in range(segments + 1):
            lng = 2 * np.pi * float(j) / segments
            x = np.cos(lng)
            y = np.sin(lng)

            glVertex3f(x * zr0, y * zr0, z0)
        glEnd()

        glBegin(GL_LINE_LOOP)
        for j in range(segments + 1):
            lng = 2 * np.pi * float(j) / segments
            x = np.cos(lng)
            y = np.sin(lng)

            glVertex3f(x * zr1, y * zr1, z1)
        glEnd()

# Function to draw a simple sphere
def draw_sphere(radius):
    segments = 64
    for i in range(segments):
        lat0 = np.pi * (-0.5 + float(i) / segments)
        z0 = np.sin(lat0) * radius
        zr0 = np.cos(lat0) * radius

        lat1 = np.pi * (-0.5 + float(i + 1) / segments)
        z1 = np.sin(lat1) * radius
        zr1 = np.cos(lat1) * radius

        glBegin(GL_QUAD_STRIP)
        for j in range(segments + 1):
            lng = 2 * np.pi * float(j) / segments
            x = np.cos(lng)
            y = np.sin(lng)

            glVertex3f(x * zr0, y * zr0, z0)
            glVertex3f(x * zr1, y * zr1, z1)
        glEnd()

def draw_infinite_grid(camera_x, camera_y, zoom):
    glColor3f(0.5, 0.5, 0.5)
    glBegin(GL_LINES)

    # Grid settings
    grid_spacing = int(1e11 * DISTANCE_SCALE)  # 1 AU grid spacing, converted to an integer
    grid_range = 100  # Number of grid lines in each direction

    # Determine the range of grid lines to draw based on camera position and zoom
    start_x = int((camera_x - grid_range * grid_spacing) // grid_spacing) * grid_spacing
    end_x = int((camera_x + grid_range * grid_spacing) // grid_spacing) * grid_spacing
    start_y = int((camera_y - grid_range * grid_spacing) // grid_spacing) * grid_spacing
    end_y = int((camera_y + grid_range * grid_spacing) // grid_spacing) * grid_spacing

    # Draw vertical grid lines
    for x in range(start_x, end_x + grid_spacing, grid_spacing):
        glVertex3f(x, 0, start_y)
        glVertex3f(x, 0, end_y)

    # Draw horizontal grid lines
    for y in range(start_y, end_y + grid_spacing, grid_spacing):
        glVertex3f(start_x, 0, y)
        glVertex3f(end_x, 0, y)

    glEnd()

    # Draw the X and Y axes with different colors
    glColor3f(1.0, 0.0, 0.0)  # Red for X-axis
    glBegin(GL_LINES)
    glVertex3f(start_x, 0, 0)
    glVertex3f(end_x, 0, 0)
    glEnd()

    glColor3f(0.0, 1.0, 0.0)  # Green for Y-axis
    glBegin(GL_LINES)
    glVertex3f(0, 0, start_y)
    glVertex3f(0, 0, end_y)
    glEnd()

def open_tkinter_window(celestial_objects):
    global editor_open

    def on_close():
        global editor_open
        editor_open = False
        for obj in celestial_objects.values():
            obj.is_wireframe = False  # Remove wireframe when window is closed
        root.destroy()

    root = tk.Tk()
    root.title("Celestial Object Simulation Controls")
    root.protocol("WM_DELETE_WINDOW", on_close)

    object_var = tk.StringVar()
    object_var.set("")  # Default option is nothing

    rotation_speed_var = tk.StringVar()
    orbital_speed_var = tk.StringVar()
    object_mass_var = tk.StringVar()
    object_radius_var = tk.StringVar()
    object_density_var = tk.StringVar()

    def update_fields(*args):
        object_name = object_var.get()
        for obj in celestial_objects.values():
            obj.is_wireframe = False
        if object_name:
            obj = celestial_objects[object_name]
            obj.is_wireframe = True
            rotation_speed_var.set(str(obj.rotation_speed))
            orbital_speed_var.set(str(obj.orbital_speed) if obj.distance > 0 else "N/A")
            object_mass_var.set(str(obj.mass))
            object_radius_var.set(str(obj.radius))
            object_density_var.set(str(obj.density))

            # Disable orbital speed entry if it's the Sun
            orbital_speed_entry.config(state=tk.NORMAL if obj.distance > 0 else tk.DISABLED)

    object_var.trace("w", update_fields)

    # Dropdown menu to select object (planets and Sun)
    ttk.Label(root, text="Select Object").grid(column=0, row=0, padx=10, pady=5)
    object_menu = ttk.OptionMenu(root, object_var, "", *celestial_objects.keys())
    object_menu.grid(column=1, row=0, padx=10, pady=5)

    # Rotation Speed Control
    ttk.Label(root, text="Rotation Speed").grid(column=0, row=1, padx=10, pady=5)
    ttk.Entry(root, textvariable=rotation_speed_var).grid(column=1, row=1, padx=10, pady=5)

    # Orbital Speed Control
    ttk.Label(root, text="Orbital Speed").grid(column=0, row=2, padx=10, pady=5)
    orbital_speed_entry = ttk.Entry(root, textvariable=orbital_speed_var)
    orbital_speed_entry.grid(column=1, row=2, padx=10, pady=5)

    # Object Mass Control
    ttk.Label(root, text="Mass").grid(column=0, row=3, padx=10, pady=5)
    mass_entry = ttk.Entry(root, textvariable=object_mass_var)
    mass_entry.grid(column=1, row=3, padx=10, pady=5)

    # Object Radius Control
    ttk.Label(root, text="Radius").grid(column=0, row=4, padx=10, pady=5)
    radius_entry = ttk.Entry(root, textvariable=object_radius_var)
    radius_entry.grid(column=1, row=4, padx=10, pady=5)

    # Object Density Control
    ttk.Label(root, text="Density").grid(column=0, row=5, padx=10, pady=5)
    density_entry = ttk.Entry(root, textvariable=object_density_var)
    density_entry.grid(column=1, row=5, padx=10, pady=5)

    # Apply Changes
    def apply_changes():
        object_name = object_var.get()
        if object_name:
            obj = celestial_objects[object_name]

            # Apply radius and mass changes
            obj.radius = float(object_radius_var.get())
            obj.mass = float(object_mass_var.get())
            obj.update_density_based_on_mass()

            # Apply density changes (will update mass)
            if object_density_var.get() != str(obj.density):
                obj.density = float(object_density_var.get())
                obj.update_mass_based_on_density()

            # Apply other changes
            obj.rotation_speed = float(rotation_speed_var.get())
            if obj.distance > 0:  # Only recalculate orbital speed for planets
                obj.orbital_speed = obj.calculate_orbital_speed()

            update_fields()  # Refresh fields with updated values

    ttk.Button(root, text="Apply", command=apply_changes).grid(column=0, row=6, columnspan=2, padx=10, pady=10)

    # Restore Defaults Button
    def restore_defaults():
        object_name = object_var.get()
        if object_name:
            obj = celestial_objects[object_name]
            obj.restore_defaults()
            update_fields()  # Refresh fields with default values

    ttk.Button(root, text="Restore to Default", command=restore_defaults).grid(column=0, row=7, columnspan=2, padx=10, pady=10)

    # Close Button
    ttk.Button(root, text="Close", command=on_close).grid(column=0, row=8, columnspan=2, padx=10, pady=10)

    root.mainloop()

def pygame_loop(celestial_objects):
    pygame.init()
    display = (1200, 900)
    screen = pygame.display.set_mode(display, DOUBLEBUF | OPENGL)
    gluPerspective(45, display[0] / display[1], 0.1, 2500.0)  # Increase the far clipping plane
    glEnable(GL_DEPTH_TEST)  # Enable depth testing

    # Initial camera position and rotation
    camera_z = -500  # Increase the distance for a better overview
    camera_x = 0
    camera_y = 0
    camera_rot_x = 0
    camera_rot_y = 0
    zoom = 1.0
    glTranslatef(camera_x, camera_y, camera_z)

    last_click_time = 0
    double_click_interval = 500  # milliseconds
    left_mouse_down = False
    right_mouse_down = False
    last_mouse_pos = None

    global editor_open
    editor_open = False

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1 and not editor_open:  # Left mouse button for rotation
                    left_mouse_down = True
                    last_mouse_pos = pygame.mouse.get_pos()
                    current_time = pygame.time.get_ticks()
                    if current_time - last_click_time < double_click_interval:
                        editor_open = True
                        threading.Thread(target=open_tkinter_window, args=(celestial_objects,)).start()
                    last_click_time = current_time
                elif event.button == 3:  # Right mouse button for panning
                    right_mouse_down = True
                    last_mouse_pos = pygame.mouse.get_pos()
                elif event.button == 4:  # Scroll up for zoom
                    zoom *= 1.1
                    camera_z += 10
                elif event.button == 5:  # Scroll down for zoom
                    zoom /= 1.1
                    camera_z -= 10
            elif event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:  # Left mouse button
                    left_mouse_down = False
                elif event.button == 3:  # Right mouse button
                    right_mouse_down = False
            elif event.type == pygame.MOUSEMOTION:
                if left_mouse_down:  # Rotate view
                    x, y = pygame.mouse.get_pos()
                    dx = x - last_mouse_pos[0]
                    dy = y - last_mouse_pos[1]
                    camera_rot_x += dy * 0.1  # Adjust sensitivity as needed
                    camera_rot_y += dx * 0.1  # Adjust sensitivity as needed
                    last_mouse_pos = (x, y)
                elif right_mouse_down:  # Pan view
                    x, y = pygame.mouse.get_pos()
                    dx = x - last_mouse_pos[0]
                    dy = y - last_mouse_pos[1]
                    camera_x += dx * 0.1  # Adjust sensitivity as needed
                    camera_y -= dy * 0.1  # Adjust sensitivity as needed
                    last_mouse_pos = (x, y)

        glLoadIdentity()
        gluPerspective(45, display[0] / display[1], 0.1, 2500.0)  # Apply the same far clipping plane here
        glTranslatef(camera_x, camera_y, camera_z)
        glRotatef(camera_rot_x, 1, 0, 0)
        glRotatef(camera_rot_y, 0, 1, 0)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        # Draw the infinite grid with proper origin
        draw_infinite_grid(camera_x, camera_y, zoom)

        # Draw all celestial objects (including the Sun)
        for obj in celestial_objects.values():
            obj.draw()
            obj.update_position()

        pygame.display.flip()
        pygame.time.wait(10)

if __name__ == "__main__":
    # Initialize celestial objects (planets and Sun)
    celestial_objects = {
        "Sun": CelestialObject("Sun", radius=6.96e8, distance=0, rotation_speed=0.0, mass=SUN_MASS, color=(1.0, 1.0, 0.0), vis_radius=10),  # Scaled-down visualization radius
        "Mercury": CelestialObject("Mercury", radius=2.44e6, distance=0.39, rotation_speed=0.004, mass=3.30e23, color=(0.6, 0.6, 0.6)),  # Mercury, gray
        "Venus": CelestialObject("Venus", radius=6.05e6, distance=0.72, rotation_speed=0.002, mass=4.87e24, color=(1.0, 0.8, 0.2)),  # Venus, yellowish
        "Earth": CelestialObject("Earth", radius=6.37e6, distance=1.0, rotation_speed=0.01, mass=5.97e24, color=(0.0, 0.0, 1.0), draw_func=lambda: draw_earth(6.37e6 * RADIUS_SCALE)),  # Earth with details
        "Mars": CelestialObject("Mars", radius=3.39e6, distance=1.52, rotation_speed=0.015, mass=6.42e23, color=(1.0, 0.5, 0.0)),  # Mars, reddish
        "Jupiter": CelestialObject("Jupiter", radius=7.14e7, distance=5.20, rotation_speed=0.03, mass=1.90e27, color=(0.8, 0.7, 0.5)),  # Jupiter, largest
        "Saturn": CelestialObject("Saturn", radius=6.03e7, distance=9.58, rotation_speed=0.027, mass=5.68e26, color=(0.9, 0.8, 0.5)),  # Saturn, rings not drawn
        "Uranus": CelestialObject("Uranus", radius=2.56e7, distance=19.22, rotation_speed=0.022, mass=8.68e25, color=(0.4, 0.7, 1.0)),  # Uranus, blue
        "Neptune": CelestialObject("Neptune", radius=2.47e7, distance=30.05, rotation_speed=0.018, mass=1.02e26, color=(0.3, 0.5, 1.0)),  # Neptune, deep blue
    }

    pygame_loop(celestial_objects)
