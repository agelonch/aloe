################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../base/swapi_static.c 

OBJS += \
./base/swapi_static.o 

C_DEPS += \
./base/swapi_static.d 


# Each subdirectory must supply rules for building sources it contributes
base/%.o: ../base/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -I../../aloe_core/common/include -I/usr/lib/gcc/i686-linux-gnu/4.7/include/ -I../include/ -I/usr/include/ -I../../aloe_core/swapi_shared -I../../aloe_core/node/include -I../../aloe_core/swapi_shared/include -I../../aloe_core/hwapi/include -O0 -g3 -Wall -c -fmessage-length=0 -Wl,--whole-archive -Wno-format -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


