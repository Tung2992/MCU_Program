################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CORE_Driver/ARM_NVIC.c \
../CORE_Driver/ARM_SYSTICK.c 

OBJS += \
./CORE_Driver/ARM_NVIC.o \
./CORE_Driver/ARM_SYSTICK.o 

C_DEPS += \
./CORE_Driver/ARM_NVIC.d \
./CORE_Driver/ARM_SYSTICK.d 


# Each subdirectory must supply rules for building sources it contributes
CORE_Driver/%.o: ../CORE_Driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@CORE_Driver/ARM_NVIC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


