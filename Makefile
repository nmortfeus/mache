CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wvla
LDLIBS=
LDFLAGS=
SRCS= main.c maching.c
OBJS=$(SRCS:.c=.o)

TARGET=mache

mash: $(OBJS)
	$(CC) -o $(TARGET) $^

run: mash
	./mash

.PHONY: clean

clean:
	$(RM) $(OBJS) $(TARGET)
