################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/broker-v2.c \
../src/conexion.c \
../src/config-reader.c \
../src/message-queue.c \
../src/protocolo.c \
../src/utils.c 

OBJS += \
./src/broker-v2.o \
./src/conexion.o \
./src/config-reader.o \
./src/message-queue.o \
./src/protocolo.o \
./src/utils.o 

C_DEPS += \
./src/broker-v2.d \
./src/conexion.d \
./src/config-reader.d \
./src/message-queue.d \
./src/protocolo.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


