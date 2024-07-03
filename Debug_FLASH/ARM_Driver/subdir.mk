################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_Driver/ARM_ADC.c \
../ARM_Driver/ARM_GPIO.c \
../ARM_Driver/ARM_LPIT.c \
../ARM_Driver/ARM_LPUART.c 

OBJS += \
./ARM_Driver/ARM_ADC.o \
./ARM_Driver/ARM_GPIO.o \
./ARM_Driver/ARM_LPIT.o \
./ARM_Driver/ARM_LPUART.o 

C_DEPS += \
./ARM_Driver/ARM_ADC.d \
./ARM_Driver/ARM_GPIO.d \
./ARM_Driver/ARM_LPIT.d \
./ARM_Driver/ARM_LPUART.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_Driver/%.o: ../ARM_Driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@ARM_Driver/ARM_ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


