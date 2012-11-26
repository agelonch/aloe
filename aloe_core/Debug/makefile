################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include test_suite/subdir.mk
-include swapi_shared/subdir.mk
-include node/subdir.mk
-include manapi/subdir.mk
-include manager/subdir.mk
-include hwapi/subdir.mk
-include common/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: runcf

# Tool invocations
runcf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -Wl,-export-dynamic -o "runcf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) runcf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
