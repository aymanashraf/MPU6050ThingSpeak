################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-2097625135:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-2097625135-inproc

build-2097625135-inproc: ../MultiFunctionBoard.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_32_01_22_core/xs" --xdcpath="C:/ti/tirtos_tivac_2_16_01_14/packages;C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;C:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;C:/ti/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;C:/ti/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;C:/ti/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C123GH6PM -r release -c "C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-2097625135 ../MultiFunctionBoard.cfg
configPkg/compiler.opt: build-2097625135
configPkg/: build-2097625135

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/StdType" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/Verbose" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/MCAL/IO/Port" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/Compiler" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/BoardAL/IO/IOHWAB" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/BoardAL/IO/TOUCH" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/ServiceLayer/DEM" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/BoardAL/Communication/MPU" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard/Main Library/OS" --include_path="D:/ATI/CCS9/RTOS/RTOS/MultiFunctionBoard" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/include" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --define=TIVAWARE --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


