## Remote console over bluetooth
DBG_BT_ADDR=00:12:09:28:04:27
DBG_PIN=1234
DBG_BAUDRATE=9600
DBG_BT_DEV=rfcomm0

## OpenOCD scripts
SCRIPTS_DIR=openocd
S_INIT=$(SCRIPTS_DIR)/init.cfg
S_SYSTEM=$(SCRIPTS_DIR)/skippycopter.cfg
S_SHUTDOWN=$(SCRIPTS_DIR)/shutdown.cfg


## Boards
BOARDS=powerboard motherboard

firmware: $(BOARDS)

# Powerboard
powerboard:
	make -C powerboard
	
# Motherboard
motherboard:
	make -C motherboard
	

## JTAG flash rules	
flash-%: %
	openocd -f $(S_SYSTEM) -f $(SCRIPTS_DIR)/$<.cfg -f $(S_INIT) -f $(SCRIPTS_DIR)/flash-$<.cfg -f $(S_SHUTDOWN) 
	
debug-%: %
	openocd -f $(S_SYSTEM) -f $(SCRIPTS_DIR)/$<.cfg -f $(S_INIT)

# Need to be ran as root ATM
console:
	rfcomm bind $(DBG_BT_DEV) $(DBG_BT_ADDR) 1
	minicom -D /dev/$(DBG_BT_DEV) -b $(DBG_BAUDRATE)
	rfcomm release $(DBG_BT_DEV)
	
clean:
	for board in $(BOARDS) ; do \
		make -C $$board clean ; \
	done
	
distclean:
	for board in $(BOARDS) ; do \
		make -C $$board distclean ; \
	done
