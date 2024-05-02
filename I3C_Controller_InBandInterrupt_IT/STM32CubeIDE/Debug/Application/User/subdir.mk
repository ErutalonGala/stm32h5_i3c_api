################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/bma530.c \
../Application/User/bst_api.c \
C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/main.c \
C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/stm32h5xx_hal_msp.c \
C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/stm32h5xx_it.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c 

OBJS += \
./Application/User/bma530.o \
./Application/User/bst_api.o \
./Application/User/main.o \
./Application/User/stm32h5xx_hal_msp.o \
./Application/User/stm32h5xx_it.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o 

C_DEPS += \
./Application/User/bma530.d \
./Application/User/bst_api.d \
./Application/User/main.d \
./Application/User/stm32h5xx_hal_msp.d \
./Application/User/stm32h5xx_it.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/%.o Application/User/%.su Application/User/%.cyclo: ../Application/User/%.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H503xx -c -I../../Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/BSP/STM32H5xx_Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/main.o: C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H503xx -c -I../../Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/BSP/STM32H5xx_Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32h5xx_hal_msp.o: C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/stm32h5xx_hal_msp.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H503xx -c -I../../Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/BSP/STM32H5xx_Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32h5xx_it.o: C:/Users/yuh1rt/Documents/pro/stm32h5_i3c_api/I3C_Controller_general/I3C_Controller_InBandInterrupt_IT/Src/stm32h5xx_it.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H503xx -c -I../../Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/BSP/STM32H5xx_Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/bma530.cyclo ./Application/User/bma530.d ./Application/User/bma530.o ./Application/User/bma530.su ./Application/User/bst_api.cyclo ./Application/User/bst_api.d ./Application/User/bst_api.o ./Application/User/bst_api.su ./Application/User/main.cyclo ./Application/User/main.d ./Application/User/main.o ./Application/User/main.su ./Application/User/stm32h5xx_hal_msp.cyclo ./Application/User/stm32h5xx_hal_msp.d ./Application/User/stm32h5xx_hal_msp.o ./Application/User/stm32h5xx_hal_msp.su ./Application/User/stm32h5xx_it.cyclo ./Application/User/stm32h5xx_it.d ./Application/User/stm32h5xx_it.o ./Application/User/stm32h5xx_it.su ./Application/User/syscalls.cyclo ./Application/User/syscalls.d ./Application/User/syscalls.o ./Application/User/syscalls.su ./Application/User/sysmem.cyclo ./Application/User/sysmem.d ./Application/User/sysmem.o ./Application/User/sysmem.su

.PHONY: clean-Application-2f-User

