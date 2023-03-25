################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/func/bignum_func.c \
../Core/func/common_func.c \
../Core/func/prime_func.c \
../Core/func/rsa_func.c \
../Core/func/usart_func.c 

OBJS += \
./Core/func/bignum_func.o \
./Core/func/common_func.o \
./Core/func/prime_func.o \
./Core/func/rsa_func.o \
./Core/func/usart_func.o 

C_DEPS += \
./Core/func/bignum_func.d \
./Core/func/common_func.d \
./Core/func/prime_func.d \
./Core/func/rsa_func.d \
./Core/func/usart_func.d 


# Each subdirectory must supply rules for building sources it contributes
Core/func/%.o Core/func/%.su: ../Core/func/%.c Core/func/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Core/func -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-func

clean-Core-2f-func:
	-$(RM) ./Core/func/bignum_func.d ./Core/func/bignum_func.o ./Core/func/bignum_func.su ./Core/func/common_func.d ./Core/func/common_func.o ./Core/func/common_func.su ./Core/func/prime_func.d ./Core/func/prime_func.o ./Core/func/prime_func.su ./Core/func/rsa_func.d ./Core/func/rsa_func.o ./Core/func/rsa_func.su ./Core/func/usart_func.d ./Core/func/usart_func.o ./Core/func/usart_func.su

.PHONY: clean-Core-2f-func

