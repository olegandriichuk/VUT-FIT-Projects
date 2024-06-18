CPROCOMPILER = gcc
SETS = -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread -lrt
TS = 
OBJECTFILES = mainFunc.o atomsData.o processes.o memory.o queue.o errorPrintingFunc.o semaphores.o
TARGET = proj2

all: $(TARGET)

$(TARGET): $(OBJECTFILES)
	$(CPROCOMPILER) $(SETS) -o $(TARGET) $(OBJECTFILES) $(TS)
	rm -f $(OBJECTFILES)
