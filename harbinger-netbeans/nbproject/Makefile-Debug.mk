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
CND_PLATFORM=MinGW_TDM-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1360937237/bitmap.o \
	${OBJECTDIR}/_ext/1360937237/camera.o \
	${OBJECTDIR}/_ext/1360937237/display.o \
	${OBJECTDIR}/_ext/1360937237/harbinger.o \
	${OBJECTDIR}/_ext/1360937237/image.o \
	${OBJECTDIR}/_ext/1360937237/input.o \
	${OBJECTDIR}/_ext/1360937237/light.o \
	${OBJECTDIR}/_ext/1360937237/mesh.o \
	${OBJECTDIR}/_ext/1360937237/object.o \
	${OBJECTDIR}/_ext/1360937237/pipeline.o \
	${OBJECTDIR}/_ext/1360937237/resource.o \
	${OBJECTDIR}/_ext/1360937237/shader.o \
	${OBJECTDIR}/_ext/1360937237/timeObject.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse
CXXFLAGS=-m32 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../bin/WIN32/libharbinger-debug.a

../bin/WIN32/libharbinger-debug.a: ${OBJECTFILES}
	${MKDIR} -p ../bin/WIN32
	${RM} ../bin/WIN32/libharbinger-debug.a
	${AR} -rv ../bin/WIN32/libharbinger-debug.a ${OBJECTFILES} 
	$(RANLIB) ../bin/WIN32/libharbinger-debug.a

${OBJECTDIR}/_ext/1360937237/bitmap.o: nbproject/Makefile-${CND_CONF}.mk ../src/bitmap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/bitmap.o ../src/bitmap.cpp

${OBJECTDIR}/_ext/1360937237/camera.o: nbproject/Makefile-${CND_CONF}.mk ../src/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/camera.o ../src/camera.cpp

${OBJECTDIR}/_ext/1360937237/display.o: nbproject/Makefile-${CND_CONF}.mk ../src/display.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/display.o ../src/display.cpp

${OBJECTDIR}/_ext/1360937237/harbinger.o: nbproject/Makefile-${CND_CONF}.mk ../src/harbinger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/harbinger.o ../src/harbinger.cpp

${OBJECTDIR}/_ext/1360937237/image.o: nbproject/Makefile-${CND_CONF}.mk ../src/image.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/image.o ../src/image.cpp

${OBJECTDIR}/_ext/1360937237/input.o: nbproject/Makefile-${CND_CONF}.mk ../src/input.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/input.o ../src/input.cpp

${OBJECTDIR}/_ext/1360937237/light.o: nbproject/Makefile-${CND_CONF}.mk ../src/light.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/light.o ../src/light.cpp

${OBJECTDIR}/_ext/1360937237/mesh.o: nbproject/Makefile-${CND_CONF}.mk ../src/mesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/mesh.o ../src/mesh.cpp

${OBJECTDIR}/_ext/1360937237/object.o: nbproject/Makefile-${CND_CONF}.mk ../src/object.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/object.o ../src/object.cpp

${OBJECTDIR}/_ext/1360937237/pipeline.o: nbproject/Makefile-${CND_CONF}.mk ../src/pipeline.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/pipeline.o ../src/pipeline.cpp

${OBJECTDIR}/_ext/1360937237/resource.o: nbproject/Makefile-${CND_CONF}.mk ../src/resource.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/resource.o ../src/resource.cpp

${OBJECTDIR}/_ext/1360937237/shader.o: nbproject/Makefile-${CND_CONF}.mk ../src/shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/shader.o ../src/shader.cpp

${OBJECTDIR}/_ext/1360937237/timeObject.o: nbproject/Makefile-${CND_CONF}.mk ../src/timeObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -g -O -w -DDEBUG -I../include -I../../hamlibs/include -I. -I. -std=c++11 -W -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/timeObject.o ../src/timeObject.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../bin/WIN32/libharbinger-debug.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
