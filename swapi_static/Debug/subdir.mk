################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../swapi_static.c 

OBJS += \
./swapi_static.o 

C_DEPS += \
./swapi_static.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -I"../../aloe_core/base" -I"../../aloe_core/hwapi" -I"../../aloe_core/base/include" -I"../../aloe_core/hwapi/include" -I"../../aloe_core/swapi_shared/include" -I"../../aloe_core/common" -I"../../aloe_core/swapi_shared" -O0 -g3 -Wall -c -fmessage-length=0 -Wl,--whole-archive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


