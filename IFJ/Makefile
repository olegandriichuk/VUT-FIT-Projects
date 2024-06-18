CC := gcc
CFLAGS := -std=c99 -Wall -Wextra 
SRCS = scanner.c str.c parser.c main.c error.c symtable.c stack_for_trees.c ast.c expression.c codegen.c
OBJS = scanner.o str.o parser.o main.o error.o symtable.o stack_for_trees.o ast.o expression.o codegen.o
TARGET = ifj23

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
