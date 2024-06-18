# Makefile for ipk24chat-client project

# Compiler settings - Can be customized.
CPP = g++
CPPFLAGS = -Wall -std=c++20
ZIPNAME  = xandri07.zip
# Files
SOURCES = Connection.cpp dataValidation.cpp constructMessages.cpp Client.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ipk24chat-client

# Default target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(CPPFLAGS) -o $@ $^

# To obtain object files
%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
zip:
	zip -v -r ${ZIPNAME} .* *
# Phony targets
.PHONY: all clean