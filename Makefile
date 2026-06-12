TARGET = stm32_app

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

MCU = -mcpu=cortex-m3 -mthumb

SRC_DIR = src
LIB_DIR = lib

SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/*.c)
ASMS = $(wildcard $(LIB_DIR)/*.s)

CFLAGS = $(MCU) -O0 -g3 -Wall -Isrc -Ilib
LDFLAGS = $(MCU) -Tstm32l152.ld -nostartfiles -Wl,--gc-sections

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(SRCS) $(ASMS)
	$(CC) $(CFLAGS) $(SRCS) $(ASMS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.elf *.bin