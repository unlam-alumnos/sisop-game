################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/character/Character.cpp \
../src/model/character/Luigi.cpp \
../src/model/character/Mario.cpp 

OBJS += \
./src/model/character/Character.o \
./src/model/character/Luigi.o \
./src/model/character/Mario.o 

CPP_DEPS += \
./src/model/character/Character.d \
./src/model/character/Luigi.d \
./src/model/character/Mario.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/character/%.o: ../src/model/character/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


