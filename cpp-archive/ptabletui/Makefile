CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude -lncurses

SRCS = src/main.cpp src/Element.cpp src/GridLayout.cpp src/InputHandler.cpp src/ScreenRenderer.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = periodic_table_tui

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

