
MAKEFILE_DIR:=$(shell dirname "$(shell realpath "$(lastword $(MAKEFILE_LIST))")")
vpath %.c $(MAKEFILE_DIR)

include $(MAKEFILE_DIR)/Makefile
