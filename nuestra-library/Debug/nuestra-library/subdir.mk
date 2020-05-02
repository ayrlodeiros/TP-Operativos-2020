################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nuestra-library/conexion.c \
../nuestra-library/mensajes.c \
../nuestra-library/serializar.c 

OBJS += \
./nuestra-library/conexion.o \
./nuestra-library/mensajes.o \
./nuestra-library/serializar.o 

C_DEPS += \
./nuestra-library/conexion.d \
./nuestra-library/mensajes.d \
./nuestra-library/serializar.d 


# Each subdirectory must supply rules for building sources it contributes
nuestra-library/%.o: ../nuestra-library/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


