## Variables ##
TEST := test
TEST_MAKER := test_maker

DIR_BIN := /usr/local/bin
DIR_TESTCASE := .test

CC := gcc
CFLAGS := -O -Wall -W -pedantic -g

## Build ##
.PHONY: $(TEST)

# Note. TESTCASE and PROG must be passed
$(TEST): $(TEST_MAKER)
	./$(TEST_MAKER) $(DIR_TESTCASE)/$(TESTCASE)
	./$(TEST) $(DIR_BIN)/$(PROG)

$(TEST_MAKER): $(TEST_MAKER).o
