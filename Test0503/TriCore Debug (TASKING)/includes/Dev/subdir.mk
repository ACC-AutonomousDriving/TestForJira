################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../includes/Dev/Driver_Stm.c \
../includes/Dev/GTM_TOM_PWM.c \
../includes/Dev/Gtm.c \
../includes/Dev/adc.c 

COMPILED_SRCS += \
./includes/Dev/Driver_Stm.src \
./includes/Dev/GTM_TOM_PWM.src \
./includes/Dev/Gtm.src \
./includes/Dev/adc.src 

C_DEPS += \
./includes/Dev/Driver_Stm.d \
./includes/Dev/GTM_TOM_PWM.d \
./includes/Dev/Gtm.d \
./includes/Dev/adc.d 

OBJS += \
./includes/Dev/Driver_Stm.o \
./includes/Dev/GTM_TOM_PWM.o \
./includes/Dev/Gtm.o \
./includes/Dev/adc.o 


# Each subdirectory must supply rules for building sources it contributes
includes/Dev/%.src: ../includes/Dev/%.c includes/Dev/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/Administrator/AURIX-v1.9.20-workspace/Test0503/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/includes\/Dev\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

includes/Dev/Driver_Stm.o: ./includes/Dev/Driver_Stm.src includes/Dev/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

includes/Dev/GTM_TOM_PWM.o: ./includes/Dev/GTM_TOM_PWM.src includes/Dev/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

includes/Dev/Gtm.o: ./includes/Dev/Gtm.src includes/Dev/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

includes/Dev/adc.o: ./includes/Dev/adc.src includes/Dev/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-includes-2f-Dev

clean-includes-2f-Dev:
	-$(RM) ./includes/Dev/Driver_Stm.d ./includes/Dev/Driver_Stm.o ./includes/Dev/Driver_Stm.src ./includes/Dev/GTM_TOM_PWM.d ./includes/Dev/GTM_TOM_PWM.o ./includes/Dev/GTM_TOM_PWM.src ./includes/Dev/Gtm.d ./includes/Dev/Gtm.o ./includes/Dev/Gtm.src ./includes/Dev/adc.d ./includes/Dev/adc.o ./includes/Dev/adc.src

.PHONY: clean-includes-2f-Dev

