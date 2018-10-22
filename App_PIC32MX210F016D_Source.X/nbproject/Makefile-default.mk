#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../main.c ../ID_Decode.c ../Initial.c ../Timers.c ../adf7021.c ../Uart.c ../EEPROM.c ../pcf8563.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/ID_Decode.o ${OBJECTDIR}/_ext/1472/Initial.o ${OBJECTDIR}/_ext/1472/Timers.o ${OBJECTDIR}/_ext/1472/adf7021.o ${OBJECTDIR}/_ext/1472/Uart.o ${OBJECTDIR}/_ext/1472/EEPROM.o ${OBJECTDIR}/_ext/1472/pcf8563.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/ID_Decode.o.d ${OBJECTDIR}/_ext/1472/Initial.o.d ${OBJECTDIR}/_ext/1472/Timers.o.d ${OBJECTDIR}/_ext/1472/adf7021.o.d ${OBJECTDIR}/_ext/1472/Uart.o.d ${OBJECTDIR}/_ext/1472/EEPROM.o.d ${OBJECTDIR}/_ext/1472/pcf8563.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/ID_Decode.o ${OBJECTDIR}/_ext/1472/Initial.o ${OBJECTDIR}/_ext/1472/Timers.o ${OBJECTDIR}/_ext/1472/adf7021.o ${OBJECTDIR}/_ext/1472/Uart.o ${OBJECTDIR}/_ext/1472/EEPROM.o ${OBJECTDIR}/_ext/1472/pcf8563.o

# Source Files
SOURCEFILES=../main.c ../ID_Decode.c ../Initial.c ../Timers.c ../adf7021.c ../Uart.c ../EEPROM.c ../pcf8563.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX220F032D
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c   
	
${OBJECTDIR}/_ext/1472/ID_Decode.o: ../ID_Decode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ID_Decode.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ID_Decode.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ID_Decode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/ID_Decode.o.d" -o ${OBJECTDIR}/_ext/1472/ID_Decode.o ../ID_Decode.c   
	
${OBJECTDIR}/_ext/1472/Initial.o: ../Initial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Initial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Initial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Initial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Initial.o.d" -o ${OBJECTDIR}/_ext/1472/Initial.o ../Initial.c   
	
${OBJECTDIR}/_ext/1472/Timers.o: ../Timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Timers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Timers.o.d" -o ${OBJECTDIR}/_ext/1472/Timers.o ../Timers.c   
	
${OBJECTDIR}/_ext/1472/adf7021.o: ../adf7021.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adf7021.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adf7021.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/adf7021.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/adf7021.o.d" -o ${OBJECTDIR}/_ext/1472/adf7021.o ../adf7021.c   
	
${OBJECTDIR}/_ext/1472/Uart.o: ../Uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Uart.o.d" -o ${OBJECTDIR}/_ext/1472/Uart.o ../Uart.c   
	
${OBJECTDIR}/_ext/1472/EEPROM.o: ../EEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/EEPROM.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/EEPROM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/EEPROM.o.d" -o ${OBJECTDIR}/_ext/1472/EEPROM.o ../EEPROM.c   
	
${OBJECTDIR}/_ext/1472/pcf8563.o: ../pcf8563.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pcf8563.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pcf8563.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pcf8563.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/pcf8563.o.d" -o ${OBJECTDIR}/_ext/1472/pcf8563.o ../pcf8563.c   
	
else
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c   
	
${OBJECTDIR}/_ext/1472/ID_Decode.o: ../ID_Decode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ID_Decode.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ID_Decode.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ID_Decode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/ID_Decode.o.d" -o ${OBJECTDIR}/_ext/1472/ID_Decode.o ../ID_Decode.c   
	
${OBJECTDIR}/_ext/1472/Initial.o: ../Initial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Initial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Initial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Initial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Initial.o.d" -o ${OBJECTDIR}/_ext/1472/Initial.o ../Initial.c   
	
${OBJECTDIR}/_ext/1472/Timers.o: ../Timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Timers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Timers.o.d" -o ${OBJECTDIR}/_ext/1472/Timers.o ../Timers.c   
	
${OBJECTDIR}/_ext/1472/adf7021.o: ../adf7021.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adf7021.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adf7021.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/adf7021.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/adf7021.o.d" -o ${OBJECTDIR}/_ext/1472/adf7021.o ../adf7021.c   
	
${OBJECTDIR}/_ext/1472/Uart.o: ../Uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Uart.o.d" -o ${OBJECTDIR}/_ext/1472/Uart.o ../Uart.c   
	
${OBJECTDIR}/_ext/1472/EEPROM.o: ../EEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/EEPROM.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/EEPROM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/EEPROM.o.d" -o ${OBJECTDIR}/_ext/1472/EEPROM.o ../EEPROM.c   
	
${OBJECTDIR}/_ext/1472/pcf8563.o: ../pcf8563.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pcf8563.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pcf8563.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pcf8563.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DPIC32_STARTER_KITS -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/pcf8563.o.d" -o ${OBJECTDIR}/_ext/1472/pcf8563.o ../pcf8563.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-L"..",-Map="${DISTDIR}/Demo_App_PIC32_Starter_Kits.X.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L"..",-Map="${DISTDIR}/Demo_App_PIC32_Starter_Kits.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/App_PIC32MX210F016D_Source.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
