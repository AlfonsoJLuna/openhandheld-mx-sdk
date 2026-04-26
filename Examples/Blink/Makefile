# Target processor selection (STM32L431CCU6)
CPU := cortex-m4
MCU := STM32L431xx
STARTUP := stm32l431xx
LDSCRIPT := STM32L432KCUx

# Path to the SDK (relative to this Makefile)
SDK := ../../SDK

# Third party sources
S_FILES += $(SDK)/cmsis-device-l4/Source/Templates/gcc/startup_$(STARTUP).s
C_FILES += $(wildcard $(SDK)/cmsis-device-l4/Source/Templates/*.c)
C_FILES += $(wildcard $(SDK)/stm32l4xx-hal-driver/Src/*.c)
C_FILES += $(wildcard $(SDK)/MX/*.c)

# First party sources
C_FILES += $(wildcard src/*.c)

# Needed objects
OBJECTS += $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
OBJECTS += $(addprefix obj/,$(notdir $(S_FILES:.s=.o)))

# Include search paths
INCS += -I$(SDK)/cmsis-core/CMSIS/Core/Include
INCS += -I$(SDK)/cmsis-device-l4/Include
INCS += -I$(SDK)/stm32l4xx-hal-driver/Inc
INCS += -I$(SDK)/MX
INCS += -Isrc

# GCC Flags
CFLAGS += -mcpu=$(CPU) -mthumb -Wall -Wextra -std=c99 -ffunction-sections -fdata-sections -Os -g
CFLAGS += -Wno-unused-parameter -Wstack-usage=256 # Generate a warning if any function stack is > 256 bytes
LFLAGS += -Wl,--gc-sections -Wl,-Map=bin/bin.map -specs=nano.specs -specs=nosys.specs
LFLAGS += -Wl,-T$(SDK)/MX/$(LDSCRIPT)_FLASH.ld
DEFINE += -D$(MCU) -DUSE_FULL_LL_DRIVER -DPRINTF_DISABLE_SUPPORT_FLOAT -DPRINTF_DISABLE_SUPPORT_EXPONENTIAL

# Rules
all: bin/bin.hex

bin/bin.hex: bin/bin.elf
	arm-none-eabi-objcopy -O ihex bin/bin.elf "bin/bin.hex"
	arm-none-eabi-size bin/bin.hex

bin/bin.elf: $(OBJECTS)
	arm-none-eabi-gcc $^ $(CFLAGS) $(LFLAGS) -o $@

obj/%.o: $(SDK)/cmsis-device-l4/Source/Templates/gcc/%.s
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

obj/%.o: $(SDK)/cmsis-device-l4/Source/Templates/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

obj/%.o: $(SDK)/stm32l4xx-hal-driver/Src/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

obj/%.o: $(SDK)/MX/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

obj/%.o: src/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

-include $(OBJECTS:.o=.d)

flash: all
	openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "program bin/bin.hex verify reset exit"

debug: all
	openocd -f interface/stlink.cfg -f target/stm32l4x.cfg
	arm-none-eabi-gdb -ex "target remote localhost:3333" ./bin/bin.elf
