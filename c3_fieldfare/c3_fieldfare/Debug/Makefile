################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Additional_Drivers/esp8266 \
../Additional_Drivers/ssd1306 \
../HAL_Drivers \
../Additional_Drivers


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Additional_Drivers/ssd1306/font.c \
../main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Additional_Drivers/ssd1306/font.o \
main.o

OBJS_AS_ARGS +=  \
Additional_Drivers/ssd1306/font.o \
main.o

C_DEPS +=  \
Additional_Drivers/ssd1306/font.d \
main.d

C_DEPS_AS_ARGS +=  \
Additional_Drivers/ssd1306/font.d \
main.d

OUTPUT_FILE_PATH +=c3_fieldfare.elf

OUTPUT_FILE_PATH_AS_ARGS +=c3_fieldfare.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





Additional_Drivers/ssd1306/%.o: ../Additional_Drivers/ssd1306/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAC_DFP\1.0.27\include" -I"../HAL_Drivers" -I".." -I"../Additional_Drivers" -I"../Additional_Drivers/ssd1306" -I"../Additional_Drivers/esp8266" -I"../Additional_Drivers/esp8266_nodemcu"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega384c3 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAC_DFP\1.0.27\gcc\dev\atxmega384c3" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAC_DFP\1.0.27\include" -I"../HAL_Drivers" -I".." -I"../Additional_Drivers" -I"../Additional_Drivers/ssd1306" -I"../Additional_Drivers/esp8266" -I"../Additional_Drivers/esp8266_nodemcu"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega384c3 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAC_DFP\1.0.27\gcc\dev\atxmega384c3" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="c3_fieldfare.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atxmega384c3 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAC_DFP\1.0.27\gcc\dev\atxmega384c3"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "c3_fieldfare.elf" "c3_fieldfare.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "c3_fieldfare.elf" "c3_fieldfare.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "c3_fieldfare.elf" > "c3_fieldfare.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "c3_fieldfare.elf" "c3_fieldfare.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "c3_fieldfare.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "c3_fieldfare.elf" "c3_fieldfare.a" "c3_fieldfare.hex" "c3_fieldfare.lss" "c3_fieldfare.eep" "c3_fieldfare.map" "c3_fieldfare.srec" "c3_fieldfare.usersignatures"
	