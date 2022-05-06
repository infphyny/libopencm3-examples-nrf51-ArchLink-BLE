PROJECT = blink
BUILD_DIR = bin

SHARED_DIR = ../my-common-code
CFILES = main.c
#CFILES += api.c
#AFILES += linker.ld.S

# TODO - you will need to edit these two lines!
DEVICE=nrf51822aa
#OOCD_FILE = board/stm32f4discovery.cfg
LDSCRIPT=generated.nrf51822.ld
# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../../../../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
