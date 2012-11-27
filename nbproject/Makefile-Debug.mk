#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=gfortran.exe
AS=as.exe

# Macros
CND_PLATFORM=MinGW_TDM_x86-Windows
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/events/message.o \
	${OBJECTDIR}/src/scripting/script_functions.o \
	${OBJECTDIR}/src/obj/resource.o \
	${OBJECTDIR}/src/scripting/script_variables.o \
	${OBJECTDIR}/src/scripting/script.o \
	${OBJECTDIR}/src/scripting/script_base.o \
	${OBJECTDIR}/src/utils/serialize.o \
	${OBJECTDIR}/src/obj/object.o \
	${OBJECTDIR}/src/utils/clock.o


# C Compiler Flags
CFLAGS=-O3

# CC Compiler Flags
CCFLAGS=-O3
CXXFLAGS=-O3

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../hamlibs/dist/WIN32 -lallegro-5.0.7-monolith-md -lsfml-system

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_PLATFORM}/libharbinger_debug.dll

${CND_DISTDIR}/${CND_PLATFORM}/libharbinger_debug.dll: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_PLATFORM}
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_PLATFORM}/libharbinger_debug.dll ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/events/message.o: src/events/message.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/events
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/events/message.o src/events/message.cpp

${OBJECTDIR}/src/scripting/script_functions.o: src/scripting/script_functions.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/scripting
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/scripting/script_functions.o src/scripting/script_functions.cpp

${OBJECTDIR}/src/obj/resource.o: src/obj/resource.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/obj
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/obj/resource.o src/obj/resource.cpp

${OBJECTDIR}/src/scripting/script_variables.o: src/scripting/script_variables.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/scripting
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/scripting/script_variables.o src/scripting/script_variables.cpp

${OBJECTDIR}/src/scripting/script.o: src/scripting/script.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/scripting
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/scripting/script.o src/scripting/script.cpp

${OBJECTDIR}/src/scripting/script_base.o: src/scripting/script_base.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/scripting
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/scripting/script_base.o src/scripting/script_base.cpp

${OBJECTDIR}/src/utils/serialize.o: src/utils/serialize.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/utils
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/utils/serialize.o src/utils/serialize.cpp

${OBJECTDIR}/src/obj/object.o: src/obj/object.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/obj
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/obj/object.o src/obj/object.cpp

${OBJECTDIR}/src/utils/clock.o: src/utils/clock.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/utils
	${RM} $@.d
	$(COMPILE.cc) -g -O -Werror -DSFML_DYNAMIC -I../hamlibs/include  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/utils/clock.o src/utils/clock.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_PLATFORM}/libharbinger_debug.dll

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
