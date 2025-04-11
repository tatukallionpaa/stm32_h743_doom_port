################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/opl/ioperm_sys.c \
../Core/opl/opl.c \
../Core/opl/opl3.c \
../Core/opl/opl_linux.c \
../Core/opl/opl_obsd.c \
../Core/opl/opl_queue.c \
../Core/opl/opl_sdl.c \
../Core/opl/opl_stm32.c \
../Core/opl/opl_timer.c \
../Core/opl/opl_win32.c 

OBJS += \
./Core/opl/ioperm_sys.o \
./Core/opl/opl.o \
./Core/opl/opl3.o \
./Core/opl/opl_linux.o \
./Core/opl/opl_obsd.o \
./Core/opl/opl_queue.o \
./Core/opl/opl_sdl.o \
./Core/opl/opl_stm32.o \
./Core/opl/opl_timer.o \
./Core/opl/opl_win32.o 

C_DEPS += \
./Core/opl/ioperm_sys.d \
./Core/opl/opl.d \
./Core/opl/opl3.d \
./Core/opl/opl_linux.d \
./Core/opl/opl_obsd.d \
./Core/opl/opl_queue.d \
./Core/opl/opl_sdl.d \
./Core/opl/opl_stm32.d \
./Core/opl/opl_timer.d \
./Core/opl/opl_win32.d 


# Each subdirectory must supply rules for building sources it contributes
Core/opl/%.o Core/opl/%.su Core/opl/%.cyclo: ../Core/opl/%.c Core/opl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Core/doom -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"/home/tatu/STM32CubeIDE/workspace_1.17.0/stm32_h743_doom_port/Core" -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -I/home/tatu/STM32CubeIDE/workspace_1.17.0/stm32_h743_doom_port/Core/emu-8950 -I/home/tatu/STM32CubeIDE/workspace_1.17.0/stm32_h743_doom_port/Core/opl -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-opl

clean-Core-2f-opl:
	-$(RM) ./Core/opl/ioperm_sys.cyclo ./Core/opl/ioperm_sys.d ./Core/opl/ioperm_sys.o ./Core/opl/ioperm_sys.su ./Core/opl/opl.cyclo ./Core/opl/opl.d ./Core/opl/opl.o ./Core/opl/opl.su ./Core/opl/opl3.cyclo ./Core/opl/opl3.d ./Core/opl/opl3.o ./Core/opl/opl3.su ./Core/opl/opl_linux.cyclo ./Core/opl/opl_linux.d ./Core/opl/opl_linux.o ./Core/opl/opl_linux.su ./Core/opl/opl_obsd.cyclo ./Core/opl/opl_obsd.d ./Core/opl/opl_obsd.o ./Core/opl/opl_obsd.su ./Core/opl/opl_queue.cyclo ./Core/opl/opl_queue.d ./Core/opl/opl_queue.o ./Core/opl/opl_queue.su ./Core/opl/opl_sdl.cyclo ./Core/opl/opl_sdl.d ./Core/opl/opl_sdl.o ./Core/opl/opl_sdl.su ./Core/opl/opl_stm32.cyclo ./Core/opl/opl_stm32.d ./Core/opl/opl_stm32.o ./Core/opl/opl_stm32.su ./Core/opl/opl_timer.cyclo ./Core/opl/opl_timer.d ./Core/opl/opl_timer.o ./Core/opl/opl_timer.su ./Core/opl/opl_win32.cyclo ./Core/opl/opl_win32.d ./Core/opl/opl_win32.o ./Core/opl/opl_win32.su

.PHONY: clean-Core-2f-opl

