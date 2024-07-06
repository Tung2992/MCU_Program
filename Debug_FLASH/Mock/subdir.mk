################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mock/Middleware.c \
../Mock/Mock.c \
../Mock/Queue.c \
../Mock/main.c 

OBJS += \
./Mock/Middleware.o \
./Mock/Mock.o \
./Mock/Queue.o \
./Mock/main.o 

C_DEPS += \
./Mock/Middleware.d \
./Mock/Mock.d \
./Mock/Queue.d \
./Mock/main.d 


# Each subdirectory must supply rules for building sources it contributes
Mock/%.o: ../Mock/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mock/Middleware.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


