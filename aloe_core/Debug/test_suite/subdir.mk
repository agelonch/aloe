################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test_suite/run_test_suite_waveform.c 

OBJS += \
./test_suite/run_test_suite_waveform.o 

C_DEPS += \
./test_suite/run_test_suite_waveform.d 


# Each subdirectory must supply rules for building sources it contributes
test_suite/%.o: ../test_suite/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I../manager/include -I../test_suite/ -I../manapi/include -I../node/include -I"../common/include" -I"../hwapi/include" -I"../swapi_shared/include" -I"../swapi_shared/" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -Wno-format -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


