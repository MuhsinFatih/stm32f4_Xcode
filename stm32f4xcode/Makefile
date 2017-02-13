# STM32F4-Discovery Makefile for Mac OSX
SHELL:=/bin/bash


BAUDRATE = 115200

OUTPATH = $(abspath $(BUILD_DIR))
BASEDIR = $(abspath ./)

CPPFILES = $(wildcard *.cpp)
CFILES = $(wildcard *.c)

CPPOBJECTS = $(addprefix $(OUTPATH)/, $(notdir $(addsuffix .o, $(CPPFILES))))
COBJECTS = $(addprefix $(OUTPATH)/, $(notdir $(addsuffix .o, $(CFILES))))


OBJECTS += $(CPPOBJECTS)
OBJECTS += $(COBJECTS)

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
#override FLOAT_TYPE = hard
override FLOAT_TYPE = soft
endif
endif

ifeq ($(BUILD_DIR), )
BUILD_DIR=../build
PROJECT_NAME=program
endif

###################################################

AS=$(BINPATH)arm-none-eabi-as
CC=$(BINPATH)arm-none-eabi-gcc
CP=$(BINPATH)arm-none-eabi-g++
LD=$(BINPATH)arm-none-eabi-g++
OBJCOPY=$(BINPATH)arm-none-eabi-objcopy
OBJDUMP=$(BINPATH)arm-none-eabi-objdump
SIZE=$(BINPATH)arm-none-eabi-size
MKDIR_P = mkdir -p
LINKER_SCRIPT = stm32_flash.ld

CPU = -mcpu=cortex-m4 -mthumb

CFLAGS  = $(CPU) -c -std=gnu99 -g -O2 -Wall
LDFLAGS  = $(CPU) -mlittle-endian -mthumb-interwork -nostartfiles -Wl,--gc-sections,-Map=$(OUTPATH)/$(PROJECT_NAME).map,--cref -specs=nosys.specs 

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

# Default to STM32F40_41xxx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F40_41xxx
endif

CFLAGS += -D$(DEVICE_DEF) -DBAUDRATE=$(BAUDRATE) -g

vpath %.a lib


# Includes
INCLUDE_PATHS = -I$(BASEDIR)/inc -I$(BASEDIR)/lib/cmsis/stm32f4xx -I$(BASEDIR)/lib/cmsis/include -I$(BASEDIR)
INCLUDE_PATHS += -I$(BASEDIR)/lib/Conf
INCLUDE_PATHS += -I$(BASEDIR)/lib/STM32F4xx_StdPeriph_Driver/inc

# Library paths
LIBPATHS = -L$(BASEDIR)/lib/STM32F4xx_StdPeriph_Driver

# Libraries to link
LIBS = -lstdperiph -lc -lgcc -lnosys -lm

###################################################

.PHONY: all clean flash

all: $(BUILD_DIR) $(OUTPATH)/$(PROJECT_NAME).elf
	@TAGS="TODO:|FIXME:"
	@./todo.sh
	@$(SIZE) $(OUTPATH)/$(PROJECT_NAME).elf

$(CPPOBJECTS): $(OUTPATH)/%.cpp.o: %.cpp
	@echo "(CP) $(notdir $<)"
	$(CP) $(CFLAGS) -std=gnu99 $(INCLUDE_PATHS) -o $@ $<

$(COBJECTS): $(OUTPATH)/%.c.o: %.c
	@echo "(CC) $@ $(notdir $<)"
	$(CC) $(CFLAGS) -std=gnu99 $(INCLUDE_PATHS) -o $@ $<

$(OUTPATH)/$(PROJECT_NAME).elf: $(OBJECTS)
	$(AS) $(CPU) -o $(OUTPATH)/CortexM4asmOps.o CortexM4asmOps.s
	$(LD) $(LDFLAGS) -T$(LINKER_SCRIPT) $(LIBPATHS) -o $@  $^ $(LIBS) $(LD_SYS_LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJECT_NAME).elf $(OUTPATH)/$(PROJECT_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJECT_NAME).elf $(OUTPATH)/$(PROJECT_NAME).bin
	$(OBJDUMP) -S --disassemble $(OUTPATH)/$(PROJECT_NAME).elf > $(OUTPATH)/$(PROJECT_NAME).dis

$(BUILD_DIR):
	$(MKDIR_P) $(OUTPATH)

clean:
	rm -f $(OUTPATH)/*.o
	rm -f $(OUTPATH)/$(PROJECT_NAME).elf
	rm -f $(OUTPATH)/$(PROJECT_NAME).hex
	rm -f $(OUTPATH)/$(PROJECT_NAME).bin
	rm -f $(OUTPATH)/$(PROJECT_NAME).dis
	rm -f $(OUTPATH)/$(PROJECT_NAME).map
	# Remove the following line if you don't want to clean the Libraries as well
	$(MAKE) clean -C lib

flash:
	@/usr/local/bin/st-flash write $(OUTPATH)/$(PROJECT_NAME).bin 0x08000000 -s
	sleep 1
	@/usr/local/bin/st-util
	@/usr/local/bin/arm-none-eabi-gdb -ex tar "extended-remote :4242" -ex "load $(BUILD_DIR)/$(PROJECT_NAME).elf" -ex "continue"

