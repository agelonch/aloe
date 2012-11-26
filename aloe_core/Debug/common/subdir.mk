################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/mempool.c \
../common/packet.c \
../common/serializable.c \
../common/waveform.c 

OBJS += \
./common/mempool.o \
./common/packet.o \
./common/serializable.o \
./common/waveform.o 

C_DEPS += \
./common/mempool.d \
./common/packet.d \
./common/serializable.d \
./common/waveform.d 


# Each subdirectory must supply rules for building sources it contributes
common/%.o: ../common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I../manager/include -I../test_suite/ -I../manapi/include -I../node/include -I"../common/include" -I"../hwapi/include" -I"../swapi_shared/include" -I"../swapi_shared/" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -Wno-format -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


