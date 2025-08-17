# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -DSFML_STATIC -I. -std=c++20 -fPIC
#LDFLAGS = -lwinmm
LDFLAGS = 

# Directories
OBJ_DIR = ../obj
SRC_DIR = .

# Source files
SOURCES = tbin/Map.cpp
HEADERS = tbin/Layer.hpp tbin/Map.hpp tbin/PropertyValue.hpp tbin/Tile.hpp tbin/TileSheet.hpp
TEST_SOURCES = main.cpp

# Object files
DEBUG_OBJS = $(SOURCES:%.cpp=$(OBJ_DIR)/Debug/%.o)
RELEASE_OBJS = $(SOURCES:%.cpp=$(OBJ_DIR)/Release/%.o)
TEST_OBJS = $(TEST_SOURCES:%.cpp=$(OBJ_DIR)/DebugTest/%.o)

# Targets
DEBUG_TARGET = ../sc0-tbin-d
RELEASE_TARGET = ../sc0-tbin
TEST_TARGET = ../libsc0-tbin-test

# Default target
all: debug release test

# Debug target (shared library)
debug: $(DEBUG_TARGET).dll

$(DEBUG_TARGET).dll: $(DEBUG_OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)  

$(OBJ_DIR)/Debug/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -g -c $< -o $@

# Release target (shared library)
release: $(RELEASE_TARGET).dll

$(RELEASE_TARGET).dll: $(RELEASE_OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) -s 

$(OBJ_DIR)/Release/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -O2 -c $< -o $@

# Test target (executable)
test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(DEBUG_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)  

$(OBJ_DIR)/DebugTest/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -g -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR)/Debug $(OBJ_DIR)/Release $(OBJ_DIR)/DebugTest
	rm -f $(DEBUG_TARGET).dll $(RELEASE_TARGET).dll $(TEST_TARGET)

# Phony targets
.PHONY: all debug release test clean

