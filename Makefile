# Makefile for PIC16F887_test

# General variables
MCU = 16F887
CC = /opt/microchip/xc8/v2.10/bin/xc8-cc
CFLAGS = -mcpu=$(MCU) -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=default -msummary=-psect,-class,+mem,-hex,-file -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -std=c99 -gdwarf-3 -mstack=compiled:auto:auto
BUILD_DIR = build
BIN_DIR = bin
OUTPUT_HEX = $(BIN_DIR)/$(PROJECT_NAME).hex
OUTPUT_ELF = $(BIN_DIR)/$(PROJECT_NAME).elf

# Source file and header file
SRC = PIC16F887.c
HEADER_FILES =

# Project name
PROJECT_NAME = PIC16F887_test

all: $(OUTPUT_HEX)

$(OUTPUT_HEX): $(OUTPUT_ELF)
	@echo "Generating HEX file..."
	/opt/microchip/xc8/v2.10/bin/xc8-bin2hex $(OUTPUT_ELF) -a -omf=hex -w

$(OUTPUT_ELF): $(BUILD_DIR)/$(SRC:.c=.p1)
	@echo "Generating ELF file..."
	/opt/microchip/xc8/v2.10/bin/xc8-cc -mcpu=$(MCU) -Wl,-Map=$(BIN_DIR)/$(PROJECT_NAME).map -DXPRJ_default=default -Wl,--defsym=__MPLAB_BUILD=1 $(CFLAGS) -Wl,--memorysummary,build/memoryfile.xml -o $(OUTPUT_ELF) $^

$(BUILD_DIR)/$(SRC:.c=.p1): $(SRC) $(HEADER_FILES)
	@echo "Compiling source files..."
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*

.PHONY: all clean
