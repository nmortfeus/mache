CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wvla
LDLIBS=
LDFLAGS=
SRCS= main.c maching.c
OBJS=$(SRCS:.c=.o)

TARGET=mache

mache: $(OBJS)
	$(CC) -o $(TARGET) $^


.PHONY: clean

clean:
	$(RM) $(OBJS) $(TARGET)
