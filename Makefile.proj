#
# 2019/03/15 created from previous versions

# The name of your project (used to name the compiled .hex file)
# TARGET = $(nodir $CURDIR)
TARGET = main

COREMAKE = Makefile.core

# PATH CONFIGURATION =========================================
# Path to arduino installation, we will use the toolchain from this install
ARDUINOPATH = /home/john/opt/arduino-1.8.8

# Path to the project directory
PROJECTPATH = $(abspath $(CURDIR))

# this is where we have stored copy of the core teensy3 code
COREPATH = $(PROJECTPATH)/teensy3
COREINC = $(PROJECTPATH)/include
CORELIB = $(PROJECTPATH)/lib
BUILDPATH = $(abspath $(COREPATH)/build)
# LIBPATH = $(PROJECTPATH)/lib

# path location for toolchain, Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(abspath $(ARDUINOPATH)/hardware/tools)
COMPILERPATH = $(abspath $(ARDUINOPATH)/hardware/tools/arm/bin)

export ARDUINOPATH PROJECTPATH COREPATH BUILDPATH CORELIB COREINC 
# ===================================================================

# TOOLS =============================================================
# toolchain programs
CC =      $(COMPILERPATH)/arm-none-eabi-gcc
CXX =     $(COMPILERPATH)/arm-none-eabi-g++
AR =      $(COMPILERPATH)/arm-none-eabi-gcc-ar
OBJCOPY = $(COMPILERPATH)/arm-none-eabi-objcopy
OBJDUMP=  $(COMPILERPATH)/arm-none-eabi-objdump
SIZE =    $(COMPILERPATH)/arm-none-eabi-size
REBOOT =  $(TOOLSPATH)/teensy_reboot
POSTCOMPILE = $(TOOLSPATH)/teensy_post_compile

export CC CXX AR OBJCOPY OBJDUMP SIZE
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

# CPP CXX C & AS flags plus options
CPPFLAGS = -c -O2 -g -Wall -ffunction-sections -fdata-sections -nostdlib \
		 -MMD -mthumb -mcpu=$(CPUARCH) -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
		 -fsingle-precision-constant $(OPTIONS)
CXXFLAGS = -fno-exceptions -felide-constructors -std=gnu++14 -fno-rtti
CFLAGS =
ASFLAGS = -x assembler-with-cpp

# linker options
TS = $(shell date +%s)
LDFLAGS = -O2 -Wl,--gc-sections,--relax,--defsym=__rtc_localtime=$(TS) \
          -T$(COREINC)/$(MCU_LD) -lstdc++  -mthumb -mcpu=$(CPUARCH) \
		  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant 

LIBS = -larm_cortexM4l_math -lm

export CPPFLAGS CXXFLAGS CFLAGS ASFLAGS LDFLAGS LIBS MCU_LD
# ===================================================================

# .SECONDARY do not delete intermediate files
# This allow us to look at a disassembly (disasm) of the elf file
.SECONDARY:

# this invocation of make will be run serially, not sure why it is here
# .NOTPARALLEL:

# let us not get target confused with file names
.PHONY: all core cleancore keyboard upload disasm

all: $(TARGET).hex

core:
	cd $(COREPATH) &&  make -f Makefile.core clean
	cd $(COREPATH) &&  make -f Makefile.core

cleancore:
	cd $(COREPATH) &&  make -f Makefile.core clean

keyboard:
	make USB=keyboard

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	$(SIZE) $<

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(COREINC) -o "$@" "$<"

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(COREINC) -o "$@" "$<"

%.elf: %.o
	$(CC) $(LDFLAGS) -o "$@" "$<" $(CORELIB)/core.a -L$(CORELIB) $(LIBS)

upload: $(TARGET).hex
	$(POSTCOMPILE) -file=$(TARGET) -path=$(shell pwd) -tools=$(TOOLSPATH)
	-$(REBOOT)

disasm: 
	$(OBJDUMP) -d -S -C $(TARGET).elf

clean:
	rm -f *.o *.d $(TARGET).elf $(TARGET).hex


# MAKEFILE NOTES
# $@  -- the file named on the left side of the :
# $<  -- the first item in the dependencies list
#
# -$(REBOOT)
# Hyphen prior to commands in makefiles is used to suppress errors and continue
#
# compiler generated dependency info
# -include $(OBJS:.o=.d)
