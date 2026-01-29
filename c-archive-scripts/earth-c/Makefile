# Earth Renderer - Makefile
# Pure C wireframe Earth renderer with multiple backends

CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wshadow -pedantic -O3 -march=native
CFLAGS += -Iinclude
LDFLAGS := -lm

# Optional backends (enable with make USE_SDL2=1 or USE_FBDEV=1)
ifdef USE_SDL2
CFLAGS += -DUSE_SDL2 $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)
endif

ifdef USE_FBDEV
CFLAGS += -DUSE_FBDEV
endif

# Debug build
ifdef DEBUG
CFLAGS := -std=c11 -Wall -Wextra -Wshadow -pedantic -g -O0 -Iinclude
CFLAGS += -fsanitize=address -fsanitize=undefined
LDFLAGS += -fsanitize=address -fsanitize=undefined
endif

# Source files
SRCS := src/math.c \
        src/geom.c \
        src/raster.c \
        src/render.c \
        src/stars.c \
        src/config.c \
        src/app.c \
        src/backend_ppm.c \
        src/main.c

# Conditionally add backend sources
ifdef USE_SDL2
SRCS += src/backend_sdl2.c
endif

ifdef USE_FBDEV
SRCS += src/backend_fbdev.c
endif

OBJS := $(SRCS:.c=.o)
TARGET := earth

# Test files
TEST_SRCS := tests/test_math.c tests/test_geom.c
TEST_OBJS := $(TEST_SRCS:.c=.o)
TEST_TARGET := test_runner

.PHONY: all clean test help install

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo ""
	@echo "Build complete: ./$(TARGET)"
	@echo "Run with: ./$(TARGET) --help"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build with SDL2 backend
sdl2:
	$(MAKE) clean
	$(MAKE) USE_SDL2=1

# Build with fbdev backend
fbdev:
	$(MAKE) clean
	$(MAKE) USE_FBDEV=1

# Build with all backends
full:
	$(MAKE) clean
	$(MAKE) USE_SDL2=1 USE_FBDEV=1

# Debug build
debug:
	$(MAKE) clean
	$(MAKE) DEBUG=1

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(filter-out src/main.o,$(OBJS))
	$(CC) -o $@ $^ $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(TEST_OBJS) $(TEST_TARGET)
	rm -f frames/*.ppm output/*.ppm *.ppm

# Install (optional)
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

# Help
help:
	@echo "Earth Renderer - Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make             Build with default (PPM) backend"
	@echo "  make sdl2        Build with SDL2 backend"
	@echo "  make fbdev       Build with Linux framebuffer backend"
	@echo "  make full        Build with all backends"
	@echo "  make debug       Build debug version with sanitizers"
	@echo "  make test        Build and run tests"
	@echo "  make clean       Remove build artifacts"
	@echo "  make install     Install to /usr/local/bin"
	@echo ""
	@echo "Examples:"
	@echo "  make USE_SDL2=1                    # Enable SDL2"
	@echo "  make USE_SDL2=1 USE_FBDEV=1        # Enable multiple backends"
	@echo "  make CC=clang CFLAGS=-O2           # Custom compiler/flags"

# Dependencies (header changes trigger rebuild)
src/math.o: include/earth/math.h
src/geom.o: include/earth/geom.h include/earth/math.h
src/raster.o: include/earth/raster.h include/earth/math.h
src/render.o: include/earth/render.h include/earth/raster.h include/earth/geom.h include/earth/math.h include/earth/config.h
src/config.o: include/earth/config.h
src/app.o: include/earth/app.h include/earth/config.h include/earth/backend.h include/earth/render.h include/earth/geom.h include/earth/raster.h
src/backend_ppm.o: include/earth/backend.h include/earth/raster.h include/earth/config.h
src/backend_sdl2.o: include/earth/backend.h include/earth/raster.h include/earth/config.h
src/backend_fbdev.o: include/earth/backend.h include/earth/raster.h include/earth/config.h
src/main.o: include/earth/app.h include/earth/config.h
