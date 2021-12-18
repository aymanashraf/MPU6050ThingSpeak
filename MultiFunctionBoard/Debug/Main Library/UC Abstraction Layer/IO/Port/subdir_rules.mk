################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Main\ Library/UC\ Abstraction\ Layer/IO/Port/PORT_CORE.obj: ../Main\ Library/UC\ Abstraction\ Layer/IO/Port/PORT_CORE.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"E:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ATI/CCS9/RTOS/MultiFunctionBoard/Main Library/StdType" --include_path="D:/ATI/CCS9/RTOS/MultiFunctionBoard/Main Library/Verbose" --include_path="D:/ATI/CCS9/RTOS/MultiFunctionBoard/Main Library/UC Abstraction Layer/IO/Port" --include_path="D:/ATI/CCS9/RTOS/MultiFunctionBoard/Main Library/Compiler" --include_path="D:/ATI/CCS9/RTOS/MultiFunctionBoard" --include_path="E:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/include" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Main Library/UC Abstraction Layer/IO/Port/PORT_CORE.d_raw" --obj_directory="Main Library/UC Abstraction Layer/IO/Port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


