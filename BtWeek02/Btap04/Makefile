PROJECT = firmware

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb

SRC_DIR    = Src
BUILD_DIR  = Build
BIN_DIR    = Bin
LINKER_DIR = Linker

SOURCES = \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/startup.c \
    $(SRC_DIR)/clock.c \
    $(SRC_DIR)/gpio.c \
    $(SRC_DIR)/timer.c

OBJECTS = \
    $(BUILD_DIR)/main.o \
    $(BUILD_DIR)/startup.o \
    $(BUILD_DIR)/clock.o \
    $(BUILD_DIR)/gpio.o \
    $(BUILD_DIR)/timer.o

CFLAGS = \
    $(MCU) \
    -O0 \
    -g3 \
    -Wall \
    -Wextra \
    -ffreestanding \
    -ffunction-sections \
    -fdata-sections \
    -IInc


LDFLAGS = \
    $(MCU) \
    -T$(LINKER_DIR)/STM32F103C8T6.ld \
    -nostdlib \
    -Wl,--gc-sections \
    -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map



all: directories $(BIN_DIR)/$(PROJECT).bin


directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)



$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/startup.o: $(SRC_DIR)/startup.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/clock.o: $(SRC_DIR)/clock.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/gpio.o: $(SRC_DIR)/gpio.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/timer.o: $(SRC_DIR)/timer.c
	$(CC) $(CFLAGS) -c $< -o $@



$(BIN_DIR)/$(PROJECT).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@


$(BIN_DIR)/$(PROJECT).bin: $(BIN_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@



flash: all
	st-flash write $(BIN_DIR)/$(PROJECT).bin 0x08000000
	st-flash reset


clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*


.PHONY: all clean flash directories