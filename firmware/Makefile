## OpenOCD scripts
SCRIPTS_DIR=openocd
S_SYSTEM=$(SCRIPTS_DIR)/skippycopter.cfg
S_ERASE_ALL=$(SCRIPTS_DIR)/erase-all.cfg
S_FLASH_ALL=$(SCRIPTS_DIR)/flash-all.cfg
S_SHUTDOWN=$(SCRIPTS_DIR)/shutdown.cfg


## Boards
BOARDS=powerboard

firmware: $(BOARDS)

# Powerboard
powerboard: powerboard/powerboard.hex
powerboard/powerboard.hex:
	make -C powerboard
	

## JTAG flash rules
erase-all: firmware
	openocd -f $(S_SYSTEM) -f $(S_ERASE_ALL) -f $(S_SHUTDOWN) 
	
flash-all: firmware
	openocd -f $(S_SYSTEM) -f $(S_ERASE_ALL) -f $(S_FLASH_ALL) -f $(S_SHUTDOWN) 
	
clean:
	for board in $(BOARDS) ; do \
		make -C $$board clean ; \
	done
	
distclean:
	for board in $(BOARDS) ; do \
		make -C $$board distclean ; \
	done
