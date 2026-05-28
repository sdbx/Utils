define installation_template
for i in $1; do \
	if $2; then \
		echo $(inst_done_str) $(call name_str,$$i) has been installed successfully.; \
	else \
		echo $(warn_str) There was a failure on installing $(call name_str,$$i).; \
	fi \
done
endef

# inst_man_t - manual install template
# $1 = list of program names
# $2 = install command
# $3 = manual section
define inst_man_t
for i in $1; do \
	if $2; then \
		echo $(inst_done_str) $(call name_str,$$i.$3) has been installed.; \
	else \
		echo $(warn_str) Failed to install $(call name_str,$$i.$3).; \
	fi \
done
endef