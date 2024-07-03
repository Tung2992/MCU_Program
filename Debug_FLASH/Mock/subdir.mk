################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mock/Mock.c 

OBJS += \
./Mock/Mock.o 

C_DEPS += \
./Mock/Mock.d 


# Each subdirectory must supply rules for building sources it contributes
Mock/%.o: ../Mock/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mock/Mock.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


