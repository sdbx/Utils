## Settings
SHELL := /bin/sh
CC := gcc
# -g for debug
CFLAGS := -O -Wall -W -pedantic -g

# deletes the default value and registers only necessary ones.
# (Refer to the chapter 16.1 in GNU make manual.)
.SUFFIXES:
.SUFFIXES: .c .o .h

# cstr = Color STRing
define cstr
"\033[$1m$2\033[0m"
endef
name_str = $(call cstr,38;5;49;1,$1)
build_completed_str = $(call cstr,48;5;222;30, [ BUILD COMPLETED ] )
installation_done_str = $(call cstr,48;5;150;30, [ INSTALLATION DONE ] )
warning_str = $(call cstr,48;5;88;37, [ WARNING ] )
notice_str = $(call cstr,48;5;152;30, [ NOTICE ] )

## Compilations
programs := btn hd hhss nsy nsy2 yandere
_programs := $(foreach program,$(programs),./$(program)/c/$(program))
default_goal := __record__
tempfile := __Utils_tempfile__

define is_recent
$(if $1,\
	@echo $(notice_str) "The following programs have made a successful build:"$\
	$(call name_str,$(foreach program,$1,\n\t$(program))),\
	@echo $(notice_str) All programs are already up-to-date!!)
endef

### This is the default goal. ###
$(default_goal): $(tempfile) $(_programs)
	$(call is_recent,$(file < $(tempfile)))
	rm $(tempfile)
	touch $(default_goal)

.PHONY: $(tempfile)
$(tempfile):
	touch $(tempfile)

define program_template
$1_path := ./$1/c
$1_name := $$($1_path)/$1
$1_sources := $$(wildcard $$($1_path)/*.c)
$1_objects := $$($1_sources:.c=.o)
all_objects += $$($1_objects)

.PHONY: $1
# This rule is for the command `make program-names...`.
$1: $$($1_name)
	if ! rm $(tempfile); then \
		echo $(notice_str) $(call name_str,$1) is already up-to-date!; \
	fi \

$$($1_name): $$($1_objects)
	$(CC) $$^ $(CFLAGS) -o $$@
	$$(file >> $$(tempfile),$1)
	@echo $(build_completed_str) $(call name_str,$$@)"\n"
endef
all_objects =

$(foreach program,$(programs),$(eval $(call program_template,$(program))))

## Auxiliary Tasks
.PHONY: install
INSTALL := install
INSTALL_PROGRAM := $(INSTALL)
INSTALL_DATA := $(INSTALL) -m 644
install_path := ./bin
bindir := $(install_path)
datadir := $(install_path)/data
hhss_data := hsr usr
program_installation_cmd := $(INSTALL_PROGRAM) ./$$i/c/$$i $(bindir)
data_installation_cmd := $(INSTALL_DATA) ./hhss/$$i $(datadir)

define installation_template
for i in $1; do \
	if $2; then \
		echo $(installation_done_str) $(call name_str,$$i) has been installed successfully.; \
	else \
		echo $(warning_str) There was a failure on installing $(call name_str,$$i).; \
	fi \
done
endef

install:
	test -d $(install_path) || (mkdir $(bindir) && mkdir $(datadir))
	$(call installation_template,$(programs),$(program_installation_cmd))
	$(call installation_template,$(hhss_data:%=%.dat),$(data_installation_cmd))

.PHONY: test
test_path := ./test
# It is necessary to assign values to these variables from the command line.
testcase =
prog =

test: $(test_path)/test_maker
	cd $(test_path) && ./test_maker ./.test/$(testcase)
	$(test_path)/test $(bindir)/$(prog)

$(test_path)/test_maker: $(test_path)/test_maker.o
	$(CC) $^ $(CFLAGS) -o $(test_path)/test_maker

.PHONY: uninstall
uninstall:
	rm -rf $(install_path)

.PHONY: clean
clean:
	rm -f $(all_objects)
	rm -f $(_programs)

.PHONY: cleanall
cleanall: uninstall clean
	rm -f $(foreach file,test_maker.o test test_maker,$(test_path)/$(file))

.PHONY: help
cmd_group = $(call cstr,48;5;148;30, $1 )
cmd_color = $(call cstr,38;5;220,$1)
cmd_arg_color = $(call cstr,36,$1)
help:
	@echo The below are the list of available commands from this Makefile.
	@echo
	@echo $(call cmd_group,1. MAKING ACTIONS)
	@echo "	make		builds every program."
	@echo "	make "$(call cmd_color,program-names...)
	@echo " 			builds mentioned program(s) only, e.g. make btn nsy"
	@echo "	make "$(call cmd_color,install)"	copies the executables of each program into ./bin directory."
	@echo
	@echo $(call cmd_group,2. CLEANING ACTIONS)
	@echo "	make "$(call cmd_color,clean)"	deletes all object files and all executables."
	@echo "	make "$(call cmd_color,uninstall)"	deletes everything under ./bin directory."
	@echo "	make "$(call cmd_color,cleanall)"	clean + uninstall + some files in ./test"
	@echo
	@echo $(call cmd_group,3. MISCELLANEOUS)
	@echo "	make "$(call cmd_color,test) \
	$(call cmd_arg_color,prog)=\"the name of a program to be tested\" $(call cmd_arg_color,testcase)=\"the name of a testcase file\"
	@echo "			performs a test on the executable with the test file."
	@echo "	make "$(call cmd_color,help)"	prints this long manual on the screen that you are reading now."

# Frequently Used Commands
.PHONY: bullshit
bullshit:
	export PATH=$(bindir) && hhss 5 | nsy e | btn 20
