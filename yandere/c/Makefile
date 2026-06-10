## Variables ##
TARGET := yandere

CC := gcc
CFLAGS := -O -Wall -W -pedantic
CFLAGS += -g
CFLAGS += -MMD -MP
CPPFLAGS = \
	-I $(DIR_LIB) \
	-DPROGNAME='"$(TARGET)"'

SOURCES = $(wildcard *.c)
LIB_SRCS = $(wildcard $(DIR_LIB)/*.c)
OBJECTS = $(SOURCES:.c=.o)
LIB_OBJS = $(notdir $(LIB_SRCS:.c=.o))

DIR_LIB := ../../lib

## Targets ##
.PHONY: all clean

## Default Goal ##
all: $(TARGET)

## Build ##
$(TARGET): $(OBJECTS) $(LIB_OBJS)
	$(CC) $^ $(CFLAGS) -o $@

$(OBJECTS): %.o: %.c
	$(CC) $< $(CPPFLAGS) $(CFLAGS) -c -o $@

$(LIB_OBJS): %.o: $(DIR_LIB)/%.c
	$(CC) $< $(CPPFLAGS) $(CFLAGS) -c -o $@

## Dependency ##
-include $(OBJECTS:.c=.d)
-include $(LIB_OBJS:.c=.d)

## Tasks ##
clean:
	rm -f $(TARGET) *.o *.d
