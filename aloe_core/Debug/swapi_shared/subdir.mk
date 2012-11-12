################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../swapi_shared/swapi_context.c \
../swapi_shared/swapi_counter.c \
../swapi_shared/swapi_error.c \
../swapi_shared/swapi_itf.c \
../swapi_shared/swapi_log.c \
../swapi_shared/swapi_var.c 

OBJS += \
./swapi_shared/swapi_context.o \
./swapi_shared/swapi_counter.o \
./swapi_shared/swapi_error.o \
./swapi_shared/swapi_itf.o \
./swapi_shared/swapi_log.o \
./swapi_shared/swapi_var.o 

C_DEPS += \
./swapi_shared/swapi_context.d \
./swapi_shared/swapi_counter.d \
./swapi_shared/swapi_error.d \
./swapi_shared/swapi_itf.d \
./swapi_shared/swapi_log.d \
./swapi_shared/swapi_var.d 


# Each subdirectory must supply rules for building sources it contributes
swapi_shared/%.o: ../swapi_shared/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I"../common" -I"../base" -I"../base/include" -I"../hwapi/include" -I"../swapi_shared" -I"../swapi_shared/include" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


