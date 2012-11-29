################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../node/nod_anode.c \
../node/nod_dispatcher.c \
../node/nod_module.c \
../node/nod_sync.c \
../node/nod_waveform.c 

OBJS += \
./node/nod_anode.o \
./node/nod_dispatcher.o \
./node/nod_module.o \
./node/nod_sync.o \
./node/nod_waveform.o 

C_DEPS += \
./node/nod_anode.d \
./node/nod_dispatcher.d \
./node/nod_module.d \
./node/nod_sync.d \
./node/nod_waveform.d 


# Each subdirectory must supply rules for building sources it contributes
node/%.o: ../node/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I../manager/include -I../test_suite/ -I../manapi/include -I../node/include -I"../common/include" -I"../hwapi/include" -I"../swapi_shared/include" -I"../swapi_shared/" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -Wno-format -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


