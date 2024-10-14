# Compiler und Flags
CXX := g++
CXXFLAGS := -Wall -std=c++20

# Verzeichnisse
WAVE_DIR := Wave
BUILD_DIR := build

# Quell- und Objektdateien
SRC := $(WAVE_DIR)/audio.cpp
OBJ := $(BUILD_DIR)/audio.o

# Ziel
TARGET := $(BUILD_DIR)/wave_processor

# Standardregel
all: $(BUILD_DIR) $(TARGET)

# Regel zum Erstellen des Ziels
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regel zum Erstellen der Objektdateien
$(BUILD_DIR)/%.o: $(WAVE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Verzeichnis erstellen, falls es nicht existiert
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean-Up
clean:
	rm -rf $(BUILD_DIR)

# Regel zum Ausführen der Anwendung
run: $(TARGET)
	./$(TARGET) $(ARGS)

.PHONY: all clean run
