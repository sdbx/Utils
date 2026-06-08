###############
# Definitions #
###############

# TPL_INST_PROG - program install template
# $1 = list of program names
# $2 = install command
define TPL_INST_PROG
for i in $1; do \
	if $2; then \
		echo $(STR_INST_DONE) $(call STR_NAME,$$i) has been installed successfully.; \
	else \
		echo $(STR_WARN) There was a failure on installing $(call STR_NAME,$$i).; \
	fi \
done
endef

# TPL_INST_MAN - manual install template
# $1 = list of program names
# $2 = install command
# $3 = manual section
define TPL_INST_MAN
for i in $1; do \
	if $2; then \
		echo $(STR_INST_DONE) $(call STR_NAME,$$i.$3) has been installed.; \
	else \
		echo $(STR_WARN) Failed to install $(call STR_NAME,$$i.$3).; \
	fi \
done
endef

# CSTR - color string
# $1 = color value
# $2 = message
define CSTR
"\033[$1m$2\033[0m"
endef
