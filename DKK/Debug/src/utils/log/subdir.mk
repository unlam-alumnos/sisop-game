################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/log/LogUtils.c 

OBJS += \
./src/utils/log/LogUtils.o 

C_DEPS += \
./src/utils/log/LogUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/log/%.o: ../src/utils/log/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


