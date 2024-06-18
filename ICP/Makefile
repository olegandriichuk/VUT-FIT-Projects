# Makefile

SRC_DIR = src
BUILD_DIR = $(SRC_DIR)/build
PRO_FILE = $(SRC_DIR)/ICP_project.pro
EXECUTABLE = ICP_proj
DOC_DIR = doc
EXAMPLES_DIR = examples
PACK_NAME = xsynek04-xandri07.zip

# Default target to compile and build the project
all: compile
	cd $(BUILD_DIR) && make

# Generate the Makefile for the build directory
compile: $(PRO_FILE)
	mkdir -p $(BUILD_DIR)
	qmake -o $(BUILD_DIR)/Makefile $(PRO_FILE)

# Run the program
run: all
	cd $(BUILD_DIR) && ./$(EXECUTABLE)

# Generate documentation using Doxygen
doxygen:
	cd $(DOC_DIR) && doxygen Doxyfile

# Clean all build files and generated documentation
clean:
	rm -rf $(BUILD_DIR)/* $(DOC_DIR)/html $(BUILD_DIR)/.qmake.stash $(DOC_DIR)/latex
	rmdir $(BUILD_DIR)

# Pack the necessary files for submission into a ZIP archive
pack: clean
	zip -r $(PACK_NAME) $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h $(SRC_DIR)/*.pro $(SRC_DIR)/*.ui $(DOC_DIR)/* $(EXAMPLES_DIR)/* Makefile README.txt Classdiagram.pdf
