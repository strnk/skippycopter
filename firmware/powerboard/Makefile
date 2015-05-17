PREFIX		?= avr
CC			:= $(PREFIX)-gcc
CXX			:= $(PREFIX)-g++
LD			:= $(PREFIX)-gcc
AR			:= $(PREFIX)-ar
AS			:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB			:= $(PREFIX)-gdb

FIRMWARE=powerboard
MCU=atmega164p
F_CPU=1000000

SRC=main.c
OBJS=$(SRC:.c=.o)


all: $(FIRMWARE) hex

#
# HEX creation
#

hex: $(FIRMWARE).hex
$(FIRMWARE).hex: $(FIRMWARE).elf
	$(OBJCOPY) -O ihex $(FIRMWARE).elf $(FIRMWARE).hex

#
# ELF compilation
#

$(FIRMWARE): $(FIRMWARE).elf
$(FIRMWARE).elf: $(OBJS)
	$(CC) -DF_CPU=$(F_CPU) -mmcu=$(MCU) -o $@ $<
	
%.o: %.c
	$(CC) -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c -o $@ $<
	
	
clean:
	rm -f *.o
	
distclean: clean
	rm -f $(FIRMWARE).elf $(FIRMWARE).hex
