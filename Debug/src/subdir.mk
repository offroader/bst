################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Node.cpp \
../src/OST.cpp \
../src/OSTNode.cpp \
../src/RBT.cpp \
../src/RBTNode.cpp \
../src/Tree.cpp \
../src/main.cpp 

OBJS += \
./src/Node.o \
./src/OST.o \
./src/OSTNode.o \
./src/RBT.o \
./src/RBTNode.o \
./src/Tree.o \
./src/main.o 

CPP_DEPS += \
./src/Node.d \
./src/OST.d \
./src/OSTNode.d \
./src/RBT.d \
./src/RBTNode.d \
./src/Tree.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


