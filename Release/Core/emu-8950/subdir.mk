################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/emu-8950/emu8950.c \
../Core/emu-8950/emuadpcm.c 

OBJS += \
./Core/emu-8950/emu8950.o \
./Core/emu-8950/emuadpcm.o 

C_DEPS += \
./Core/emu-8950/emu8950.d \
./Core/emu-8950/emuadpcm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/emu-8950/%.o Core/emu-8950/%.su Core/emu-8950/%.cyclo: ../Core/emu-8950/%.c Core/emu-8950/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Core/doom -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"/home/tatu/STM32CubeIDE/workspace_1.17.0/stm32_h743_doom_port/Core" -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -I/home/tatu/STM32CubeIDE/workspace_1.17.0/stm32_h743_doom_port/Core/emu-8950 -I/home/tatu/STM32CubeIDE/workspace_1.17.0/stm32_h743_doom_port/Core/opl -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-emu-2d-8950

clean-Core-2f-emu-2d-8950:
	-$(RM) ./Core/emu-8950/emu8950.cyclo ./Core/emu-8950/emu8950.d ./Core/emu-8950/emu8950.o ./Core/emu-8950/emu8950.su ./Core/emu-8950/emuadpcm.cyclo ./Core/emu-8950/emuadpcm.d ./Core/emu-8950/emuadpcm.o ./Core/emu-8950/emuadpcm.su

.PHONY: clean-Core-2f-emu-2d-8950

