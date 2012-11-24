################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Administrador.cpp \
../src/Auto.cpp \
../src/Entrada.cpp \
../src/Estacionamiento.cpp \
../src/LockFile.cpp \
../src/Proceso.cpp \
../src/Semaforo.cpp \
../src/logger.cpp \
../src/tp1.cpp 

OBJS += \
./src/Administrador.o \
./src/Auto.o \
./src/Entrada.o \
./src/Estacionamiento.o \
./src/LockFile.o \
./src/Proceso.o \
./src/Semaforo.o \
./src/logger.o \
./src/tp1.o 

CPP_DEPS += \
./src/Administrador.d \
./src/Auto.d \
./src/Entrada.d \
./src/Estacionamiento.d \
./src/LockFile.d \
./src/Proceso.d \
./src/Semaforo.d \
./src/logger.d \
./src/tp1.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


