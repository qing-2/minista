# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src

# Source files
SRCS = src/Path.cc src/AdjacencyListGraph.cc src/main.cc

# Header files
HDRS = src/Graph.h src/Path.h src/AdjacencyListGraph.h

# Object files
OBJS = $(SRCS:.cc=.o)

# Executable name
EXEC = graph_app

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cc $(HDRS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
