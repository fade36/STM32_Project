TARGET = stm32_app

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

MCU = -mcpu=cortex-m3 -mthumb

SRC_DIR = src
LIB_DIR = lib

SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/*.c)
ASMS = $(wildcard $(LIB_DIR)/*.s)
HDRS = $(wildcard $(SRC_DIR)/*.h) $(wildcard $(LIB_DIR)/*.h)

CFLAGS = $(MCU) -O0 -g3 -Wall -Isrc -Ilib
LDFLAGS = $(MCU) -Tstm32l152.ld -nostartfiles -Wl,--gc-sections

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(SRCS) $(ASMS) $(HDRS) stm32l152.ld
	$(CC) $(CFLAGS) $(SRCS) $(ASMS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.elf *.bin
	
flash: $(TARGET).bin
	st-flash --connect-under-reset write $(TARGET).bin 0x08000000