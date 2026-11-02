# STM32L152 Low-Power Uptime Timer

Bare-metal firmware for the **STM32L152** (Cortex-M3) that works as a button-controlled "time since last reset" timer with a built-in segment LCD. The CPU sleeps most of the time and wakes up only on a button press or on a 1 Hz RTC tick.

**No HAL, no CMSIS, no vendor libraries.** Register definitions, drivers, LCD driver, linker script, Makefile and startup code were written from the reference manual, the datasheet and ST's public examples.

## Behavior

| 1st press | any time | Display turns on and shows time since last reset |
| 2nd press | within 30 s | Display asks to reset the timer (`RST`) |
| 3rd press | within 30 s | Timer is reset, display turns off |
| none | 30 s of inactivity | Display turns off, timer is **not** reset |

The time is shown as days and hours, for example `D01H05`.

## How it works

- **Timekeeping:** the RTC runs from LSI. The prescalers are set so that the clock is 1 Hz, and the RTC wake-up timer fires once per second. Each wake-up advances the elapsed-time counter.
- **Button:** PA0 is connected to EXTI line 0 (rising edge). The interrupt handler only sets a flag, and all logic runs in the main loop. A short software delay filters contact bounce.
- **Power:** the CPU enters Sleep mode with `WFI` whenever the display is off. Wake-up sources are the RTC wake-up timer and the button.
- **Display:** custom driver for the on-board segment LCD (1/4 duty, 1/3 bias, pins on AF11), with its own character-to-segment mapping.

## Hardware

- Board: STM32L152C-Discovery
- Button: user button on PA0
- Display: on-board segment LCD
- RTC clock: LSI

## Build and flash

Requires `arm-none-eabi-gcc`, `make` and a flasher such as `st-flash`.

```bash
make
st-flash write stm32_app.bin 0x08000000
```

## Project structure

```
.
├── src/           # application and startup code
├── lib/           # minimal register definitions
├── Makefile
└── stm32l152.ld   # linker script
```

## Notes and limitations

- Currently one "hour" equals 20 seconds (demo setting to check the roll-over quickly). For real time, set the threshold in `ElapsedTime_Tick()` to 3600.
- LSI is not precise. For accurate time, switch the RTC to the external 32.768 kHz LSE crystal.
- Regular Sleep mode is used. Stop mode would reduce consumption further.
- Current consumption has not been measured yet.

## What I learned

- Writing peripheral drivers from the reference manual (RTC, EXTI, NVIC, LCD)
- RTC wake-up timer configuration and low-power design with `WFI`
- Driving a multiplexed segment LCD
- Linker script, startup code and Makefile-based cross-compilation

## Credits

- STM32L152 reference manual (RM0038) and datasheet
- LCD character maps taken from ST's public STM32L152-Discovery LCD driver
- Startup assembly code written with the help of an AI assistant
- Everything else written by me