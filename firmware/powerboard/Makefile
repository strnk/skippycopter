FIRMWARE=powerboard
MCU=atmega164p
F_CPU=8000000

SRC=main.c
OBJS=$(SRC:.c=.o)


all: $(FIRMWARE) hex

#
# HEX creation
#

hex: $(FIRMWARE).hex
$(FIRMWARE).hex: $(FIRMWARE).elf
	avr-objcopy -O ihex $(FIRMWARE).elf $(FIRMWARE).hex

#
# ELF compilation
#

$(FIRMWARE): $(FIRMWARE).elf
$(FIRMWARE).elf: $(OBJS)
	avr-gcc -DF_CPU=$(F_CPU) -mmcu=$(MCU) -o $@ $<
	
%.o: %.c
	avr-gcc -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c -o $@ $<
	
	
clean:
	rm -f *.o
	
distclean: clean
	rm -f $(FIRMWARE).elf $(FIRMWARE).hex
