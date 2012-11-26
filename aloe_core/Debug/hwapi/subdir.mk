################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hwapi/hwapi_base.c \
../hwapi/hwapi_error.c \
../hwapi/hwapi_file.c \
../hwapi/hwapi_itf.c \
../hwapi/hwapi_itfphysic.c \
../hwapi/hwapi_itfqueue.c \
../hwapi/hwapi_kernel.c \
../hwapi/hwapi_process.c \
../hwapi/hwapi_task.c \
../hwapi/hwapi_time.c \
../hwapi/hwapi_timer.c \
../hwapi/pipeline.c \
../hwapi/queue.c 

OBJS += \
./hwapi/hwapi_base.o \
./hwapi/hwapi_error.o \
./hwapi/hwapi_file.o \
./hwapi/hwapi_itf.o \
./hwapi/hwapi_itfphysic.o \
./hwapi/hwapi_itfqueue.o \
./hwapi/hwapi_kernel.o \
./hwapi/hwapi_process.o \
./hwapi/hwapi_task.o \
./hwapi/hwapi_time.o \
./hwapi/hwapi_timer.o \
./hwapi/pipeline.o \
./hwapi/queue.o 

C_DEPS += \
./hwapi/hwapi_base.d \
./hwapi/hwapi_error.d \
./hwapi/hwapi_file.d \
./hwapi/hwapi_itf.d \
./hwapi/hwapi_itfphysic.d \
./hwapi/hwapi_itfqueue.d \
./hwapi/hwapi_kernel.d \
./hwapi/hwapi_process.d \
./hwapi/hwapi_task.d \
./hwapi/hwapi_time.d \
./hwapi/hwapi_timer.d \
./hwapi/pipeline.d \
./hwapi/queue.d 


# Each subdirectory must supply rules for building sources it contributes
hwapi/%.o: ../hwapi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D__USE_GNU -I../manager/include -I../test_suite/ -I../manapi/include -I../node/include -I"../common/include" -I"../hwapi/include" -I"../swapi_shared/include" -I"../swapi_shared/" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -std=c99 -Wno-unused-parameter -Winline -Wno-format -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


