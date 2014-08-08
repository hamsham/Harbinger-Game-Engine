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
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug_WIN32_DLL
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/application.o \
	${OBJECTDIR}/src/billboard.o \
	${OBJECTDIR}/src/bitmap.o \
	${OBJECTDIR}/src/camera.o \
	${OBJECTDIR}/src/display.o \
	${OBJECTDIR}/src/drawable.o \
	${OBJECTDIR}/src/font.o \
	${OBJECTDIR}/src/gBuffer.o \
	${OBJECTDIR}/src/geometry.o \
	${OBJECTDIR}/src/harbinger.o \
	${OBJECTDIR}/src/input.o \
	${OBJECTDIR}/src/light.o \
	${OBJECTDIR}/src/mesh.o \
	${OBJECTDIR}/src/pipeline.o \
	${OBJECTDIR}/src/primitives.o \
	${OBJECTDIR}/src/renderer.o \
	${OBJECTDIR}/src/resource.o \
	${OBJECTDIR}/src/shader.o \
	${OBJECTDIR}/src/shadowMap.o \
	${OBJECTDIR}/src/skybox.o \
	${OBJECTDIR}/src/stockShaders.o \
	${OBJECTDIR}/src/text.o \
	${OBJECTDIR}/src/transformations.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	../../bin/hge_deferred_test_debug

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 -ggdb -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -ffast-math -s -shared -fPIC
CXXFLAGS=-m32 -ggdb -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -ffast-math -s -shared -fPIC

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lglew32 -lglfw3dll -lopengl32 -lFreeImage -lassimp.dll -lfreetype ../HamLibs/bin/WIN32/libhamlibs.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ./bin/WIN32/harbinger-debug.dll

./bin/WIN32/harbinger-debug.dll: ../HamLibs/bin/WIN32/libhamlibs.a

./bin/WIN32/harbinger-debug.dll: ${OBJECTFILES}
	${MKDIR} -p ./bin/WIN32
	${LINK.cc} -o ./bin/WIN32/harbinger-debug.dll ${OBJECTFILES} ${LDLIBSOPTIONS} -Wl,--out-implib,./bin/WIN32/libharbinger-debug.dll.a -shared

${OBJECTDIR}/src/application.o: nbproject/Makefile-${CND_CONF}.mk src/application.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/application.o src/application.cpp

${OBJECTDIR}/src/billboard.o: nbproject/Makefile-${CND_CONF}.mk src/billboard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/billboard.o src/billboard.cpp

${OBJECTDIR}/src/bitmap.o: nbproject/Makefile-${CND_CONF}.mk src/bitmap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitmap.o src/bitmap.cpp

${OBJECTDIR}/src/camera.o: nbproject/Makefile-${CND_CONF}.mk src/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera.o src/camera.cpp

${OBJECTDIR}/src/display.o: nbproject/Makefile-${CND_CONF}.mk src/display.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/display.o src/display.cpp

${OBJECTDIR}/src/drawable.o: nbproject/Makefile-${CND_CONF}.mk src/drawable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drawable.o src/drawable.cpp

${OBJECTDIR}/src/font.o: nbproject/Makefile-${CND_CONF}.mk src/font.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/font.o src/font.cpp

${OBJECTDIR}/src/gBuffer.o: nbproject/Makefile-${CND_CONF}.mk src/gBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/gBuffer.o src/gBuffer.cpp

${OBJECTDIR}/src/geometry.o: nbproject/Makefile-${CND_CONF}.mk src/geometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/geometry.o src/geometry.cpp

${OBJECTDIR}/src/harbinger.o: nbproject/Makefile-${CND_CONF}.mk src/harbinger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/harbinger.o src/harbinger.cpp

${OBJECTDIR}/src/input.o: nbproject/Makefile-${CND_CONF}.mk src/input.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/input.o src/input.cpp

${OBJECTDIR}/src/light.o: nbproject/Makefile-${CND_CONF}.mk src/light.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/light.o src/light.cpp

${OBJECTDIR}/src/mesh.o: nbproject/Makefile-${CND_CONF}.mk src/mesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mesh.o src/mesh.cpp

${OBJECTDIR}/src/pipeline.o: nbproject/Makefile-${CND_CONF}.mk src/pipeline.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pipeline.o src/pipeline.cpp

${OBJECTDIR}/src/primitives.o: nbproject/Makefile-${CND_CONF}.mk src/primitives.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/primitives.o src/primitives.cpp

${OBJECTDIR}/src/renderer.o: nbproject/Makefile-${CND_CONF}.mk src/renderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/renderer.o src/renderer.cpp

${OBJECTDIR}/src/resource.o: nbproject/Makefile-${CND_CONF}.mk src/resource.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resource.o src/resource.cpp

${OBJECTDIR}/src/shader.o: nbproject/Makefile-${CND_CONF}.mk src/shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/shader.o src/shader.cpp

${OBJECTDIR}/src/shadowMap.o: nbproject/Makefile-${CND_CONF}.mk src/shadowMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/shadowMap.o src/shadowMap.cpp

${OBJECTDIR}/src/skybox.o: nbproject/Makefile-${CND_CONF}.mk src/skybox.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/skybox.o src/skybox.cpp

${OBJECTDIR}/src/stockShaders.o: nbproject/Makefile-${CND_CONF}.mk src/stockShaders.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stockShaders.o src/stockShaders.cpp

${OBJECTDIR}/src/text.o: nbproject/Makefile-${CND_CONF}.mk src/text.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/text.o src/text.cpp

${OBJECTDIR}/src/transformations.o: nbproject/Makefile-${CND_CONF}.mk src/transformations.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/transformations.o src/transformations.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
../../bin/hge_deferred_test_debug: ${TESTDIR}/tests/Deferred_Test/blankNormalMap.o ${TESTDIR}/tests/Deferred_Test/logicApp.o ${TESTDIR}/tests/Deferred_Test/main.o ${TESTDIR}/tests/Deferred_Test/renderApp.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ../../bin
	${LINK.cc}   -o ../../bin/hge_deferred_test_debug $^ ${LDLIBSOPTIONS} ../HamLibs/bin/WIN32/libhamlibs.a 


${TESTDIR}/tests/Deferred_Test/blankNormalMap.o: tests/Deferred_Test/blankNormalMap.cpp 
	${MKDIR} -p ${TESTDIR}/tests/Deferred_Test
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/Deferred_Test/blankNormalMap.o tests/Deferred_Test/blankNormalMap.cpp


${TESTDIR}/tests/Deferred_Test/logicApp.o: tests/Deferred_Test/logicApp.cpp 
	${MKDIR} -p ${TESTDIR}/tests/Deferred_Test
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/Deferred_Test/logicApp.o tests/Deferred_Test/logicApp.cpp


${TESTDIR}/tests/Deferred_Test/main.o: tests/Deferred_Test/main.cpp 
	${MKDIR} -p ${TESTDIR}/tests/Deferred_Test
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/Deferred_Test/main.o tests/Deferred_Test/main.cpp


${TESTDIR}/tests/Deferred_Test/renderApp.o: tests/Deferred_Test/renderApp.cpp 
	${MKDIR} -p ${TESTDIR}/tests/Deferred_Test
	${RM} "$@.d"
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/Deferred_Test/renderApp.o tests/Deferred_Test/renderApp.cpp


${OBJECTDIR}/src/application_nomain.o: ${OBJECTDIR}/src/application.o src/application.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/application.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/application_nomain.o src/application.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/application.o ${OBJECTDIR}/src/application_nomain.o;\
	fi

${OBJECTDIR}/src/billboard_nomain.o: ${OBJECTDIR}/src/billboard.o src/billboard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/billboard.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/billboard_nomain.o src/billboard.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/billboard.o ${OBJECTDIR}/src/billboard_nomain.o;\
	fi

${OBJECTDIR}/src/bitmap_nomain.o: ${OBJECTDIR}/src/bitmap.o src/bitmap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/bitmap.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitmap_nomain.o src/bitmap.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/bitmap.o ${OBJECTDIR}/src/bitmap_nomain.o;\
	fi

${OBJECTDIR}/src/camera_nomain.o: ${OBJECTDIR}/src/camera.o src/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/camera.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_nomain.o src/camera.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/camera.o ${OBJECTDIR}/src/camera_nomain.o;\
	fi

${OBJECTDIR}/src/display_nomain.o: ${OBJECTDIR}/src/display.o src/display.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/display.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/display_nomain.o src/display.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/display.o ${OBJECTDIR}/src/display_nomain.o;\
	fi

${OBJECTDIR}/src/drawable_nomain.o: ${OBJECTDIR}/src/drawable.o src/drawable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/drawable.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drawable_nomain.o src/drawable.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/drawable.o ${OBJECTDIR}/src/drawable_nomain.o;\
	fi

${OBJECTDIR}/src/font_nomain.o: ${OBJECTDIR}/src/font.o src/font.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/font.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/font_nomain.o src/font.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/font.o ${OBJECTDIR}/src/font_nomain.o;\
	fi

${OBJECTDIR}/src/gBuffer_nomain.o: ${OBJECTDIR}/src/gBuffer.o src/gBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/gBuffer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/gBuffer_nomain.o src/gBuffer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/gBuffer.o ${OBJECTDIR}/src/gBuffer_nomain.o;\
	fi

${OBJECTDIR}/src/geometry_nomain.o: ${OBJECTDIR}/src/geometry.o src/geometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/geometry.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/geometry_nomain.o src/geometry.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/geometry.o ${OBJECTDIR}/src/geometry_nomain.o;\
	fi

${OBJECTDIR}/src/harbinger_nomain.o: ${OBJECTDIR}/src/harbinger.o src/harbinger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/harbinger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/harbinger_nomain.o src/harbinger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/harbinger.o ${OBJECTDIR}/src/harbinger_nomain.o;\
	fi

${OBJECTDIR}/src/input_nomain.o: ${OBJECTDIR}/src/input.o src/input.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/input.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/input_nomain.o src/input.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/input.o ${OBJECTDIR}/src/input_nomain.o;\
	fi

${OBJECTDIR}/src/light_nomain.o: ${OBJECTDIR}/src/light.o src/light.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/light.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/light_nomain.o src/light.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/light.o ${OBJECTDIR}/src/light_nomain.o;\
	fi

${OBJECTDIR}/src/mesh_nomain.o: ${OBJECTDIR}/src/mesh.o src/mesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/mesh.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mesh_nomain.o src/mesh.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/mesh.o ${OBJECTDIR}/src/mesh_nomain.o;\
	fi

${OBJECTDIR}/src/pipeline_nomain.o: ${OBJECTDIR}/src/pipeline.o src/pipeline.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/pipeline.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pipeline_nomain.o src/pipeline.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/pipeline.o ${OBJECTDIR}/src/pipeline_nomain.o;\
	fi

${OBJECTDIR}/src/primitives_nomain.o: ${OBJECTDIR}/src/primitives.o src/primitives.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/primitives.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/primitives_nomain.o src/primitives.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/primitives.o ${OBJECTDIR}/src/primitives_nomain.o;\
	fi

${OBJECTDIR}/src/renderer_nomain.o: ${OBJECTDIR}/src/renderer.o src/renderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/renderer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/renderer_nomain.o src/renderer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/renderer.o ${OBJECTDIR}/src/renderer_nomain.o;\
	fi

${OBJECTDIR}/src/resource_nomain.o: ${OBJECTDIR}/src/resource.o src/resource.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/resource.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resource_nomain.o src/resource.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/resource.o ${OBJECTDIR}/src/resource_nomain.o;\
	fi

${OBJECTDIR}/src/shader_nomain.o: ${OBJECTDIR}/src/shader.o src/shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/shader.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/shader_nomain.o src/shader.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/shader.o ${OBJECTDIR}/src/shader_nomain.o;\
	fi

${OBJECTDIR}/src/shadowMap_nomain.o: ${OBJECTDIR}/src/shadowMap.o src/shadowMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/shadowMap.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/shadowMap_nomain.o src/shadowMap.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/shadowMap.o ${OBJECTDIR}/src/shadowMap_nomain.o;\
	fi

${OBJECTDIR}/src/skybox_nomain.o: ${OBJECTDIR}/src/skybox.o src/skybox.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/skybox.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/skybox_nomain.o src/skybox.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/skybox.o ${OBJECTDIR}/src/skybox_nomain.o;\
	fi

${OBJECTDIR}/src/stockShaders_nomain.o: ${OBJECTDIR}/src/stockShaders.o src/stockShaders.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/stockShaders.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stockShaders_nomain.o src/stockShaders.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/stockShaders.o ${OBJECTDIR}/src/stockShaders_nomain.o;\
	fi

${OBJECTDIR}/src/text_nomain.o: ${OBJECTDIR}/src/text.o src/text.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/text.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/text_nomain.o src/text.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/text.o ${OBJECTDIR}/src/text_nomain.o;\
	fi

${OBJECTDIR}/src/transformations_nomain.o: ${OBJECTDIR}/src/transformations.o src/transformations.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/transformations.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../../../../../../MinGW32/include/freetype2 -I../HamLibs/include -Iinclude -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/transformations_nomain.o src/transformations.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/transformations.o ${OBJECTDIR}/src/transformations_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ../../bin/hge_deferred_test_debug || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ./bin/WIN32/harbinger-debug.dll

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
