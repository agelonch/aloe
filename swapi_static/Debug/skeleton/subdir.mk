################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../skeleton/params.c \
../skeleton/skeleton.c 

OBJS += \
./skeleton/params.o \
./skeleton/skeleton.o 

C_DEPS += \
./skeleton/params.d \
./skeleton/skeleton.d 


# Each subdirectory must supply rules for building sources it contributes
skeleton/%.o: ../skeleton/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -I../../aloe_core/common/include -I/usr/lib/gcc/i686-linux-gnu/4.7/include/ -I../include/ -I/usr/include/ -I../../aloe_core/swapi_shared -I../../aloe_core/node/include -I../../aloe_core/swapi_shared/include -I../../aloe_core/hwapi/include -O0 -g3 -Wall -c -fmessage-length=0 -Wl,--whole-archive -Wno-format -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


