################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ILI9488_driver/app.c \
../ILI9488_driver/file.c \
../ILI9488_driver/ili9488.c \
../ILI9488_driver/uart.c 

C_DEPS += \
./ILI9488_driver/app.d \
./ILI9488_driver/file.d \
./ILI9488_driver/ili9488.d \
./ILI9488_driver/uart.d 

OBJS += \
./ILI9488_driver/app.o \
./ILI9488_driver/file.o \
./ILI9488_driver/ili9488.o \
./ILI9488_driver/uart.o 


# Each subdirectory must supply rules for building sources it contributes
ILI9488_driver/%.o ILI9488_driver/%.su: ../ILI9488_driver/%.c ILI9488_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I/Users/abudujiasueryilihamu/Documents/Projects/stm32/TFTdispolay_Rev0/ILI9488_driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ILI9488_driver

clean-ILI9488_driver:
	-$(RM) ./ILI9488_driver/app.d ./ILI9488_driver/app.o ./ILI9488_driver/app.su ./ILI9488_driver/file.d ./ILI9488_driver/file.o ./ILI9488_driver/file.su ./ILI9488_driver/ili9488.d ./ILI9488_driver/ili9488.o ./ILI9488_driver/ili9488.su ./ILI9488_driver/uart.d ./ILI9488_driver/uart.o ./ILI9488_driver/uart.su

.PHONY: clean-ILI9488_driver

