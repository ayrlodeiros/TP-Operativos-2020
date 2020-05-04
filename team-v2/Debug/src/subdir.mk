################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/acciones.c \
../src/config-reader.c \
../src/constructor.c \
../src/planificacion.c \
../src/team-v2.c \
../src/utils.c 

OBJS += \
./src/acciones.o \
./src/config-reader.o \
./src/constructor.o \
./src/planificacion.o \
./src/team-v2.o \
./src/utils.o 

C_DEPS += \
./src/acciones.d \
./src/config-reader.d \
./src/constructor.d \
./src/planificacion.d \
./src/team-v2.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


