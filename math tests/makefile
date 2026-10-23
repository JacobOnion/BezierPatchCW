CXX      ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
SRC_DIR  := ../BezierPatchWindowRelease
TARGET   := testLibrary

# Picks up whichever of these .cpp files exist (skips any that are header-only)
LIB_SRCS := $(wildcard $(addprefix $(SRC_DIR)/,Vector3.cpp Point3.cpp Matrix4.cpp Homogeneous4.cpp Quaternion.cpp))
HEADERS  := $(wildcard $(SRC_DIR)/*.h)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): testLibrary.cpp $(LIB_SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) $(filter %.cpp,$^) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)