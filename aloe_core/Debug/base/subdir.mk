################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../base/execinfo.c \
../base/interface.c \
../base/man_platform.c \
../base/man_probelistener.c \
../base/manapi_base.c \
../base/manapi_error.c \
../base/mapping.c \
../base/module.c \
../base/nod_anode.c \
../base/nod_dispatcher.c \
../base/nod_module.c \
../base/nod_module_execinfo.c \
../base/nod_sync.c \
../base/nod_variable.c \
../base/nod_waveform.c \
../base/nod_waveform_status.c \
../base/packet.c \
../base/variable.c \
../base/waveform.c \
../base/waveform_status.c 

OBJS += \
./base/execinfo.o \
./base/interface.o \
./base/man_platform.o \
./base/man_probelistener.o \
./base/manapi_base.o \
./base/manapi_error.o \
./base/mapping.o \
./base/module.o \
./base/nod_anode.o \
./base/nod_dispatcher.o \
./base/nod_module.o \
./base/nod_module_execinfo.o \
./base/nod_sync.o \
./base/nod_variable.o \
./base/nod_waveform.o \
./base/nod_waveform_status.o \
./base/packet.o \
./base/variable.o \
./base/waveform.o \
./base/waveform_status.o 

C_DEPS += \
./base/execinfo.d \
./base/interface.d \
./base/man_platform.d \
./base/man_probelistener.d \
./base/manapi_base.d \
./base/manapi_error.d \
./base/mapping.d \
./base/module.d \
./base/nod_anode.d \
./base/nod_dispatcher.d \
./base/nod_module.d \
./base/nod_module_execinfo.d \
./base/nod_sync.d \
./base/nod_variable.d \
./base/nod_waveform.d \
./base/nod_waveform_status.d \
./base/packet.d \
./base/variable.d \
./base/waveform.d \
./base/waveform_status.d 


# Each subdirectory must supply rules for building sources it contributes
base/%.o: ../base/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I"../common" -I"../base" -I"../base/include" -I"../hwapi/include" -I"../swapi_shared" -I"../swapi_shared/include" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


