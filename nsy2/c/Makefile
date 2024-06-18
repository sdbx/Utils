CC = gcc
CCFLAGS = -O -Wall -W -pedantic

TARGET = nsy2

SOURCES = $(wildcard ./*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CCFLAGS) -c -o $@ $<
