## Variables ##
TARGET := hhss

CC := gcc
CFLAGS := -O -Wall -W -pedantic -g
CPPFLAGS = \
	-I $(DIR_COMMON) \
	-DINSTPATH='"$(DIR_DATA)/$(TARGET)"' \
	-DPROGNAME='"$(TARGET)"'

SOURCES = $(wildcard *.c)
SOURCES += $(wildcard $(DIR_COMMON)/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

DIR_DATA := ../..
DIR_COMMON := ../../common

## Default Goal ##
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	$(CC) $< $(CPPFLAGS) $(CFLAGS) -c -o $@

clean:
	rm -f $(TARGET) *.o *.d

# Refer to the "4.14 Generating Prerequisites Automatically"
# section in the GNU Make manual.
%.d: %.c
	@set -e; rm -f $@; \
	 $(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

## Dependency ##
include $(SOURCES:.c=.d)
