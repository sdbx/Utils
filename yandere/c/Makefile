## Variables ##
TARGET = yandere

CC := gcc
CFLAGS := -O -Wall -W -pedantic -g
CPPFLAGS = \
	-I $(DIR_COMMON) \
	-DPROGNAME='"$(TARGET)"'

SOURCES = $(wildcard *.c)
SOURCES += $(wildcard $(DIR_COMMON)/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

DIR_COMMON := ../../common

## Default Goal ##
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	$(CC) $< $(CPPFLAGS) $(CFLAGS) -c -o $@

# Refer to the "4.14 Generating Prerequisites Automatically"
# section in the GNU Make manual.
%.d: %.c
	@set -e; rm -f $@; \
	 $(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

## Dependency ##
include $(SOURCES:.c=.d)
