#
# 2019/03/15 created from previous versions

# The name of your project (used to name the compiled .hex file)
# TARGET = $(nodir $CURDIR)
TARGET = main

# PATH CONFIGURATION =========================================
# Path to arduino installation, we will use the toolchain from this install
ARDUINOPATH = /home/john/opt/arduino-1.8.8

# Path to the project directory, pick one of these approaches
# PROJECTPATH = /home/john/src/ohio
# PROJECTPATH = $CURDIR
PROJECTPATH = $(abspath $(CURDIR))

# this is where we have stored copy of the core teensy3 code
COREPATH = $(PROJECTPATH)/teensy3
# INC = $(COREPATH)
INC = $(PROJECTPATH)/include
LIB = $(PROJECTPATH)/lib

# path location for toolchain, Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(abspath $(ARDUINOPATH)/hardware/tools)
COMPILERPATH = $(abspath $(ARDUINOPATH)/hardware/tools/arm/bin)
# ===================================================================

# TOOLS =============================================================
# toolchain programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
OBJDUMP= $(abspath $(COMPILERPATH))/arm-none-eabi-objdump
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size
# ===================================================================


# BUILD OPTIONS & FLAGS ===================================================
#
# configurable USB serial or keyboard option
# .... serial port is the default
# .... "make keyboard" will compile core software for USB keyboard
USBFLAG = USB_SERIAL
ifeq ($(USB),keyboard) 
   USBFLAG=USB_KEYBOARDONLY
endif

# board specific settings -- Teensy 3.5 
MCU=MK64FX512
MCU_LD = mk64fx512.ld
CPUARCH = cortex-m4

# options configured thru Arduino menus
OPTIONS = -DF_CPU=120000000 -D$(USBFLAG) -DLAYOUT_US_ENGLISH 
# options needed by many Arduino libraries to configure for Teensy 3.x
OPTIONS += -D__$(MCU)__  -DTEENSYDUINO=145 -DARDUINO=10808
# This can be used in cpp/c programs to conditionally compile for ino
# or regular cpp/c programs
OPTIONs += -DUSING_MAKEFILE 

# common flags & options
CPPFLAGS = -c -O2 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -mthumb -mcpu=$(CPUARCH) -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant $(OPTIONS)
# C++ flags
CXXFLAGS = -fno-exceptions -felide-constructors -std=gnu++14 -fno-rtti
# C flags
CFLAGS =
# assembler flags
ASFLAGS = -x assembler-with-cpp

# linker options
TS = $(shell date +%s)
LDFLAGS = -O2 -Wl,--gc-sections,--relax,--defsym=__rtc_localtime=$(TS) -T$(INC)/$(MCU_LD) -lstdc++  -mthumb -mcpu=$(CPUARCH) -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant 

# libraries to link... my version of the library
# since the teensy is compiled to indivdual object we do not have a arduino lib
LIBS = $(LIB)/core.a -larm_cortexM4l_math -lm
# ===================================================================


# automatically create lists of the sources and objects
# TODO: this does not handle Arduino libraries yet...
# C_FILES := $(wildcard *.c)
# CPP_FILES := $(wildcard *.cpp)
# OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

C_FILES := $(wildcard $(COREPATH)/*.c)
CPP_FILES := $(wildcard $(COREPATH)/*.cpp)
OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

# .SECONDARY do not delete intermediate files
# This allow us to look at a dump of the elf file
.SECONDARY:
# this invocation of make will be run serially
# .NOTPARALLEL:

all: $(TARGET).hex

# coreserial:
# 	cd $(COREPATH) ; make
# 
# corekeyboard:
# 	cd $(COREPATH) ; make USB=keyboard
# 
keyboard:
	make USB=keyboard

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INC) -o "$@" "$<"

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(INC) -o "$@" "$<"

%.elf: %.o
	$(CC) $(LDFLAGS) -o "$@" "$<" -L$(LIB) $(LIBS)

upload: $(TARGET).hex
	$(abspath $(TOOLSPATH))/teensy_post_compile -file=$(TARGET) -path=$(shell pwd) -tools=$(abspath $(TOOLSPATH))
	-$(abspath $(TOOLSPATH))/teensy_reboot

dump: 
	$(OBJDUMP) -d $(TARGET).elf

# compiler generated dependency info
# -include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d $(TARGET).elf $(TARGET).hex


# MAKEFILE NOTES
# $@  -- the file named on the left side of the :
# $<  -- the first item in the dependencies list
