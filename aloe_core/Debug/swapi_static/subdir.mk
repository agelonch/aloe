################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../swapi_static/swapi_static.c 

OBJS += \
./swapi_static/swapi_static.o 

C_DEPS += \
./swapi_static/swapi_static.d 


# Each subdirectory must supply rules for building sources it contributes
swapi_static/%.o: ../swapi_static/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I"../common" -I"../base" -I"../base/include" -I"../hwapi/include" -I"../swapi_shared" -I"../swapi_shared/include" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


