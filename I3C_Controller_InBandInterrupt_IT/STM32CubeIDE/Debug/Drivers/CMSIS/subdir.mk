################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/system_stm32h5xx.c 

OBJS += \
./Drivers/CMSIS/system_stm32h5xx.o 

C_DEPS += \
./Drivers/CMSIS/system_stm32h5xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32h5xx.o: C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/system_stm32h5xx.c Drivers/CMSIS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H503xx -c -I../../Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/BSP/STM32H5xx_Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS

clean-Drivers-2f-CMSIS:
	-$(RM) ./Drivers/CMSIS/system_stm32h5xx.cyclo ./Drivers/CMSIS/system_stm32h5xx.d ./Drivers/CMSIS/system_stm32h5xx.o ./Drivers/CMSIS/system_stm32h5xx.su

.PHONY: clean-Drivers-2f-CMSIS

