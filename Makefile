## Includes ##
include vars.mk
include defs.mk

## Targets ##
.SUFFIXES:
.SUFFIXES: .c .o .h

.PHONY: all test install uninstall clean cleanall help

## Default Goal ##
all: $(TARGETS)

$(TARGETS):
	$(MAKE) -C $@/c TARGET=$@ DIR_DATA=$(DIR_DATA)
	@echo $(STR_BUILD_DONE) $(call STR_NAME,$@)

## Test Program ##
$(TEST): $(TEST_MAKER)
	cd $(DIR_TEST) && ./$(TEST_MAKER) $(DIR_TESTCASE)/$(testcase)
	$(DIR_TEST)/$(TEST) $(DIR_BIN)/$(prog)

$(TEST_MAKER): $(DIR_TEST)/$(TEST_MAKER).o
	$(CC) $^ $(CFLAGS) -o $(DIR_TEST)/$@

## Auxiliary Tasks ##
install:
	test -d $(DIR_BIN) || mkdir -p $(DIR_BIN)
	test -d $(DIR_DATA)/$(HHSS) || mkdir -p $(DIR_DATA)/$(HHSS)
	test -d $(DIR_MAN1) || mkdir -p $(DIR_MAN1)
	test -d $(DIR_MAN7) || mkdir -p $(DIR_MAN7)
	$(call TPL_INST_PROG,$(TARGETS),$(CMD_INSTALL_PROG))
	$(call TPL_INST_PROG,$(HHSS_DATA),$(CMD_INSTALL_DATA))
	$(call TPL_INST_MAN,$(TARGETS),$(CMD_INSTALL_MAN1),1)
	$(call TPL_INST_MAN,$(PROJECT),$(CMD_INSTALL_MAN7),7)

uninstall:
	rm -f $(foreach target,$(TARGETS),$(DIR_BIN)/$(target))
	rm -rf $(DIR_DATA)/$(HHSS)
	rm -f $(foreach target,$(TARGETS),$(DIR_MAN1)/$(target).1)
	rm -f $(DIR_MAN7)/$(PROJECT).7

clean:
	find . -name "*.o" -delete
	find . -name "*.d" -delete
	rm -f $(foreach target,$(TARGETS),$(target)/c/$(target))

cleanall: uninstall clean
	rm -f $(foreach file,$(TEST) $(TEST_MAKER),$(DIR_TEST)/$(file))

help:
	@echo The below are the list of available commands from this Makefile.
	@echo
	@echo $(call STR_HELP_GROUP,1. MAKING ACTIONS)
	@echo "	make		builds every program."
	@echo "	make "$(call STR_HELP_CMDHL,program-names...)
	@echo " 			builds mentioned program(s) only, e.g. make btn nsy"
	@echo "	make "$(call STR_HELP_CMDHL,install)"	copies the executables of each program into $(DIR_BIN) directory."
	@echo
	@echo $(call STR_HELP_GROUP,2. CLEANING ACTIONS)
	@echo "	make "$(call STR_HELP_CMDHL,clean)"	deletes all object files and all executables."
	@echo "	make "$(call STR_HELP_CMDHL,uninstall)"	deletes everything under $(DIR_BIN) directory."
	@echo "	make "$(call STR_HELP_CMDHL,cleanall)"	clean + uninstall + some files in ./test"
	@echo
	@echo $(call STR_HELP_GROUP,3. MISCELLANEOUS)
	@echo "	make "$(call STR_HELP_CMDHL,test) \
	$(call STR_HELP_ARGHL,prog)=\"the name of a program to be tested\" $(call STR_HELP_ARGHL,testcase)=\"the name of a testcase file\"
	@echo "			performs a test on the executable with the test file."
	@echo "	make "$(call STR_HELP_CMDHL,help)"	prints this long manual on the screen that you are reading now."
