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
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1360937237/assert.o \
	${OBJECTDIR}/_ext/1360937237/hash.o \
	${OBJECTDIR}/_ext/1360937237/logger.o \
	${OBJECTDIR}/_ext/1360937237/randomNum.o \
	${OBJECTDIR}/_ext/1360937237/string.o


# C Compiler Flags
CFLAGS=-m32 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W

# CC Compiler Flags
CCFLAGS=-m32 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W -Wextra
CXXFLAGS=-m32 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W -Wextra

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=--32

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../bin/WIN32/libhamlibs.a

../bin/WIN32/libhamlibs.a: ${OBJECTFILES}
	${MKDIR} -p ../bin/WIN32
	${RM} ../bin/WIN32/libhamlibs.a
	${AR} -rvc ../bin/WIN32/libhamlibs.a ${OBJECTFILES} 
	$(RANLIB) ../bin/WIN32/libhamlibs.a

${OBJECTDIR}/_ext/1360937237/assert.o: nbproject/Makefile-${CND_CONF}.mk ../src/assert.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -Wall -s -std=c++11 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/assert.o ../src/assert.cpp

${OBJECTDIR}/_ext/1360937237/hash.o: nbproject/Makefile-${CND_CONF}.mk ../src/hash.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -Wall -s -std=c++11 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/hash.o ../src/hash.cpp

${OBJECTDIR}/_ext/1360937237/logger.o: nbproject/Makefile-${CND_CONF}.mk ../src/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -Wall -s -std=c++11 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/logger.o ../src/logger.cpp

${OBJECTDIR}/_ext/1360937237/randomNum.o: nbproject/Makefile-${CND_CONF}.mk ../src/randomNum.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -Wall -s -std=c++11 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/randomNum.o ../src/randomNum.cpp

${OBJECTDIR}/_ext/1360937237/string.o: nbproject/Makefile-${CND_CONF}.mk ../src/string.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -Wall -s -std=c++11 -mieee-fp -mmmx -msse -msse2 -mfpmath=sse -O3 -pedantic -Werror -Wall -W -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/string.o ../src/string.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../bin/WIN32/libhamlibs.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
