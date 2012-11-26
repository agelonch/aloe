################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../manager/man_mapping.c \
../manager/man_platform.c \
../manager/man_probelistener.c \
../manager/man_waveform.c \
../manager/man_waveform_parser.c 

OBJS += \
./manager/man_mapping.o \
./manager/man_platform.o \
./manager/man_probelistener.o \
./manager/man_waveform.o \
./manager/man_waveform_parser.o 

C_DEPS += \
./manager/man_mapping.d \
./manager/man_platform.d \
./manager/man_probelistener.d \
./manager/man_waveform.d \
./manager/man_waveform_parser.d 


# Each subdirectory must supply rules for building sources it contributes
manager/%.o: ../manager/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I../manager/include -I../test_suite/ -I../manapi/include -I../node/include -I"../common/include" -I"../hwapi/include" -I"../swapi_shared/include" -I"../swapi_shared/" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -Wno-format -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


