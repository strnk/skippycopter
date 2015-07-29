PREFIX		?= arm-none-eabi
CC			:= $(PREFIX)-gcc
CXX			:= $(PREFIX)-g++
LD			:= $(PREFIX)-gcc
AR			:= $(PREFIX)-ar
AS			:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump

FIRMWARE	 = motherboard

LDSCRIPT	 = stm32f4.ld
SRC			 = sys/cppheap.cpp sys/flags.c periph/sys_scheduler.cpp board_setup.c led.c periph/i2c_device.cpp \
	periph/pwm.cpp periph/ppm.cpp radio/radio.cpp \
	imu/fusion/madgwick.cpp imu/hmc5883l.cpp imu/adxl345.cpp imu/bmp085.cpp imu/l3g4200d.cpp imu/imu.cpp \
	cli/functions/system.cpp cli/functions/pwm.cpp cli/cli_functions.c cli/cli.cpp \
	system/powerboard.cpp system/system.cpp gps/gps.cpp \
	main.cpp

## Debug mode
SRC 		+= debug/performance.cpp
CPPFLAGS 	+= -DDEBUG

OBJS		 = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o, $(SRC)))

OPENCM3_DIR	?= /usr/local/arm-none-eabi/
INCLUDE_DIR	= $(OPENCM3_DIR)/include
LIB_DIR		= $(OPENCM3_DIR)/lib


FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS	 = -mthumb -mcpu=cortex-m4 $(FP_FLAGS)

###############################################################################
# C flags

CFLAGS		+= -Os -g
CFLAGS		+= -Wextra -Wimplicit-function-declaration
CFLAGS		+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS		+= -fno-common -ffunction-sections -fdata-sections

###############################################################################
# C++ flags

CXXFLAGS	+= -Os -g
CXXFLAGS	+= -Wextra -Wredundant-decls  -Weffc++
CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections
CXXFLAGS	+= -ffreestanding -nostdlib
CXXFLAGS	+= -fno-rtti -nostartfiles -fno-exceptions -fno-unwind-tables -fno-stack-protector 
CXXFLAGS	+= -std=c++0x -fno-builtin

###############################################################################
# C & C++ preprocessor common flags

CPPFLAGS	+= -MD
CPPFLAGS	+= -Wall -Wundef 
CPPFLAGS	+= -I$(INCLUDE_DIR) -I. -DSTM32F4

###############################################################################
# Linker flags

LDFLAGS		+= --static -nostartfiles -fno-rtti -nostartfiles -fno-exceptions -fno-unwind-tables -fno-stack-protector 
LDFLAGS		+= -L$(LIB_DIR)
LDFLAGS		+= -T$(LDSCRIPT)
LDFLAGS		+= -Wl,-Map=$(FIRMWARE).map
LDFLAGS		+= -Wl,--gc-sections
ifeq ($(V),99)
LDFLAGS		+= -Wl,--print-gc-sections
endif

###############################################################################
# Used libraries

LDLIBS		+= -lopencm3_stm32f4
LDLIBS		+= -Wl,--start-group -lm -lc -lgcc -lnosys -Wl,--end-group

all: $(FIRMWARE) hex

test:
	echo $(OBJS)
#
# HEX creation
#

hex: $(FIRMWARE).hex
$(FIRMWARE).hex: $(FIRMWARE).elf
	$(OBJCOPY) -Oihex $< $@

%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(OBJCOPY) -Obinary $(*).elf $(*).bin


%.srec: %.elf
	$(OBJCOPY) -Osrec $(*).elf $(*).srec

%.list: %.elf
	$(OBJDUMP) -S $(*).elf > $(*).list


$(FIRMWARE): $(FIRMWARE).elf
$(FIRMWARE).elf $(FIRMWARE).map: $(OBJS) $(LDSCRIPT) $(LIB_DIR)/libopencm3_stm32f4.a
	$(LD) $(LDFLAGS) $(ARCH_FLAGS) $(OBJS) $(LDLIBS) -o $(FIRMWARE).elf

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $(*).o -c $(*).c

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $(*).o -c $(*).cpp
	
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	
distclean: clean
	rm -f $(FIRMWARE).elf $(FIRMWARE).hex $(FIRMWARE).map
	
	
