CC = gcc
CFLAGS = -O3 -Wall
SRC = blokus.c main.c
OBJ = $(SRC:.c=.o)
TARGET = blokus

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

