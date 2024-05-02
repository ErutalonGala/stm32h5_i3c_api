################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32H5xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/STM32H5xx_Nucleo/subdir.mk
-include Application/User/subdir.mk
-include Application/Startup/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := I3C_Controller_InBandInterrupt_IT
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
I3C_Controller_InBandInterrupt_IT.elf \

MAP_FILES += \
I3C_Controller_InBandInterrupt_IT.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
I3C_Controller_InBandInterrupt_IT.list \


# All Target
all: main-build

# Main-build Target
main-build: I3C_Controller_InBandInterrupt_IT.elf secondary-outputs

# Tool invocations
I3C_Controller_InBandInterrupt_IT.elf I3C_Controller_InBandInterrupt_IT.map: $(OBJS) $(USER_OBJS) C:\Users\yuh1rt\Documents\pro\stm32h5_i3c_api\I3C_Controller_general\I3C_Controller_InBandInterrupt_IT\STM32CubeIDE\STM32H503RBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "I3C_Controller_InBandInterrupt_IT.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"C:\Users\yuh1rt\Documents\pro\stm32h5_i3c_api\I3C_Controller_general\I3C_Controller_InBandInterrupt_IT\STM32CubeIDE\STM32H503RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="I3C_Controller_InBandInterrupt_IT.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

I3C_Controller_InBandInterrupt_IT.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "I3C_Controller_InBandInterrupt_IT.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) I3C_Controller_InBandInterrupt_IT.elf I3C_Controller_InBandInterrupt_IT.list I3C_Controller_InBandInterrupt_IT.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
