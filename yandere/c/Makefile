## Variables ##
TARGET := yandere

CC := gcc
CFLAGS := -O -Wall -W -pedantic -g
CPPFLAGS = \
	-I $(DIR_LIB) \
	-DPROGNAME='"$(TARGET)"'

SOURCES = $(wildcard *.c)
SOURCES += $(wildcard $(DIR_LIB)/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

DIR_LIB := ../../lib

## Targets ##
.PHONY: all clean

## Default Goal ##
all: $(TARGET)

## Build ##
$(TARGET): $(OBJECTS)
	$(CC) $(notdir $^) $(CFLAGS) -o $@

%.o: %.c
	$(CC) $< $(CPPFLAGS) $(CFLAGS) -c -o $(notdir $@)

## Dependency ##
%.d: %.c
	@set -e; rm -f $@; \
	 $(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

-include $(SOURCES:.c=.d)

# Note: for more information, consult the "4.14 Generating Prerequisites Automatically" section in the GNU Make manual.

## Tasks ##
clean:
	rm -f $(TARGET) *.o *.d
