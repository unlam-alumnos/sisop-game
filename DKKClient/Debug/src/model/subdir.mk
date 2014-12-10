################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/Actor.cpp \
../src/model/Barril.cpp \
../src/model/Fuego.cpp \
../src/model/Kong.cpp \
../src/model/Map.cpp \
../src/model/Princesa.cpp \
../src/model/Tarro.cpp \
../src/model/Timer.cpp 

OBJS += \
./src/model/Actor.o \
./src/model/Barril.o \
./src/model/Fuego.o \
./src/model/Kong.o \
./src/model/Map.o \
./src/model/Princesa.o \
./src/model/Tarro.o \
./src/model/Timer.o 

CPP_DEPS += \
./src/model/Actor.d \
./src/model/Barril.d \
./src/model/Fuego.d \
./src/model/Kong.d \
./src/model/Map.d \
./src/model/Princesa.d \
./src/model/Tarro.d \
./src/model/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/%.o: ../src/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


