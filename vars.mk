#############
# Variables #
#############

## Make ##
SHELL := /bin/sh

INSTALL := install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

## Build ##
PROJECT := Utils

BTN := btn
HD := hd
HHSS := hhss
NSY := nsy
NSY2 := nsy2
YANDERE := yandere

TARGETS = $(BTN) $(HD) $(HHSS) $(NSY) $(NSY2) $(YANDERE)

TEST := test

## Install ##
DIR_LIB := lib
DIR_INSTALL := /usr/local
DIR_BIN = $(DIR_INSTALL)/bin
DIR_DATA = $(DIR_INSTALL)/share
DIR_MAN1 = $(DIR_INSTALL)/share/man/man1
DIR_MAN7 = $(DIR_INSTALL)/share/man/man7

HHSS_DATA := hsr.dat usr.dat dat.db rt.db

CMD_INSTALL_PROG = $(INSTALL_PROGRAM) $$i/c/$$i $(DIR_BIN)
CMD_INSTALL_DATA = $(INSTALL_DATA) $(HHSS)/$$i $(DIR_DATA)/$(HHSS)
CMD_INSTALL_MAN1 = $(INSTALL_DATA) $$i/$$i.1 $(DIR_MAN1)
CMD_INSTALL_MAN7 = $(INSTALL_DATA) $$i.7 $(DIR_MAN7)

## Miscellaneous ##
STR_NAME = $(call CSTR,38;5;49;1,$1)
STR_BUILD_DONE = $(call CSTR,48;5;222;30, [ BUILD COMPLETED ] )
STR_INST_DONE = $(call CSTR,48;5;150;30, [ INSTALL DONE ] )
STR_WARN = $(call CSTR,48;5;88;37, [ WARNING ] )

STR_HELP_GROUP = $(call CSTR,48;5;148;30, $1 )
STR_HELP_CMDHL = $(call CSTR,38;5;220,$1)
STR_HELP_ARGHL = $(call CSTR,36,$1)
