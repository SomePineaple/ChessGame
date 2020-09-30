################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Engine.cpp \
../src/Main.cpp \
../src/Move.cpp \
../src/Renderer.cpp 

OBJS += \
./src/Engine.o \
./src/Main.o \
./src/Move.o \
./src/Renderer.o 

CPP_DEPS += \
./src/Engine.d \
./src/Main.d \
./src/Move.d \
./src/Renderer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


