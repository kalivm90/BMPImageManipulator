CC= gcc
CFLAGS= -Wall -Wextra -std=c11
TARGET= main.c
OUTPUT= $(basename $(TARGET))

run: $(TARGET)
	$(CC) $(TARGET) helpers.c -o $(OUTPUT) -lm

clean:
	rm -f run
