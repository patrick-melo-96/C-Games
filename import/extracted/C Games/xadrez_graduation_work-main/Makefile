CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = xadrez
SRCS = xadrez.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c xadrez.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run rebuild
