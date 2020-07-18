################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/broker-v2.c \
../src/conexion.c \
../src/config-reader.c \
../src/constructor.c \
../src/protocolo.c 

OBJS += \
./src/broker-v2.o \
./src/conexion.o \
./src/config-reader.o \
./src/constructor.o \
./src/protocolo.o 

C_DEPS += \
./src/broker-v2.d \
./src/conexion.d \
./src/config-reader.d \
./src/constructor.d \
./src/protocolo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


