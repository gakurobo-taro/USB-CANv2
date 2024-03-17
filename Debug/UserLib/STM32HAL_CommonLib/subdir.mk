################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../UserLib/STM32HAL_CommonLib/data_convert.cpp \
../UserLib/STM32HAL_CommonLib/pwm.cpp 

OBJS += \
./UserLib/STM32HAL_CommonLib/data_convert.o \
./UserLib/STM32HAL_CommonLib/pwm.o 

CPP_DEPS += \
./UserLib/STM32HAL_CommonLib/data_convert.d \
./UserLib/STM32HAL_CommonLib/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
UserLib/STM32HAL_CommonLib/%.o UserLib/STM32HAL_CommonLib/%.su UserLib/STM32HAL_CommonLib/%.cyclo: ../UserLib/STM32HAL_CommonLib/%.cpp UserLib/STM32HAL_CommonLib/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UserLib-2f-STM32HAL_CommonLib

clean-UserLib-2f-STM32HAL_CommonLib:
	-$(RM) ./UserLib/STM32HAL_CommonLib/data_convert.cyclo ./UserLib/STM32HAL_CommonLib/data_convert.d ./UserLib/STM32HAL_CommonLib/data_convert.o ./UserLib/STM32HAL_CommonLib/data_convert.su ./UserLib/STM32HAL_CommonLib/pwm.cyclo ./UserLib/STM32HAL_CommonLib/pwm.d ./UserLib/STM32HAL_CommonLib/pwm.o ./UserLib/STM32HAL_CommonLib/pwm.su

.PHONY: clean-UserLib-2f-STM32HAL_CommonLib

