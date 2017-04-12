################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FatFS/silabs_diskio.c 

OBJS += \
./FatFS/silabs_diskio.o 

C_DEPS += \
./FatFS/silabs_diskio.d 


# Each subdirectory must supply rules for building sources it contributes
FatFS/silabs_diskio.o: ../FatFS/silabs_diskio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFM32WG842F256=1' '-DDEBUG=1' -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/SDcard_EPSI/emlib" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/SDcard_EPSI/Calendar" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/SDcard_EPSI/inc" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/SDcard_EPSI/FatFS/src" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/SDcard_EPSI/FatFS/src/option" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"FatFS/silabs_diskio.d" -MT"FatFS/silabs_diskio.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


