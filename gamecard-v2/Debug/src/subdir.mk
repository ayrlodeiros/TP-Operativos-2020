################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bitmap.c \
../src/bloques.c \
<<<<<<< HEAD
=======
../src/conexiones.c \
>>>>>>> develop
../src/config-reader.c \
../src/gamecard-v2.c \
../src/utils.c 

OBJS += \
./src/bitmap.o \
./src/bloques.o \
<<<<<<< HEAD
=======
./src/conexiones.o \
>>>>>>> develop
./src/config-reader.o \
./src/gamecard-v2.o \
./src/utils.o 

C_DEPS += \
./src/bitmap.d \
./src/bloques.d \
<<<<<<< HEAD
=======
./src/conexiones.d \
>>>>>>> develop
./src/config-reader.d \
./src/gamecard-v2.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


