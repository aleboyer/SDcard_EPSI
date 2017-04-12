################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include FatFS/src/option/subdir.mk
-include FatFS/src/subdir.mk
-include FatFS/subdir.mk
-include Calendar/subdir.mk
-include CMSIS/EFM32WG/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SDcard_EPSI.axf

# Tool invocations
SDcard_EPSI.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "SDcard_EPSI.ld" -Xlinker --gc-sections -Xlinker -Map="SDcard_EPSI.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o SDcard_EPSI.axf "./CMSIS/EFM32WG/startup_efm32wg.o" "./CMSIS/EFM32WG/system_efm32wg.o" "./Calendar/clock_tc.o" "./Calendar/crystal_parameters.o" "./Calendar/main_calendar_tc.o" "./FatFS/silabs_diskio.o" "./FatFS/src/ff.o" "./FatFS/src/option/syscall.o" "./FatFS/src/option/unicode.o" "./emlib/em_adc.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_int.o" "./emlib/em_rtc.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emlib/microsd.o" "./src/epsilometer_CMU_init.o" "./src/epsilometer_GPIO_init.o" "./src/epsilometer_SDcard.o" "./src/epsilometer_TIMER_init.o" "./src/epsilometer_analog.o" "./src/epsilometer_coms.o" "./src/epsilometer_main.o" -lm -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: SDcard_EPSI.hex'
	arm-none-eabi-objcopy -O ihex "SDcard_EPSI.axf" "SDcard_EPSI.hex"
	@echo ' '

	@echo 'Building bin file: SDcard_EPSI.bin'
	arm-none-eabi-objcopy -O binary "SDcard_EPSI.axf" "SDcard_EPSI.bin"
	@echo ' '

	@echo 'Building s37 file: SDcard_EPSI.s37'
	arm-none-eabi-objcopy -O srec "SDcard_EPSI.axf" "SDcard_EPSI.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "SDcard_EPSI.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SDcard_EPSI.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
