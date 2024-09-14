CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wvla -pedantic -std=c99
TARGET = Preprocessor
SRCS = $(wildcard preprocessor/*.c)
OBJS = $(SRCS:.c=.o)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
