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
CC=gcc-4.8
CCC=g++-4.8
CXX=g++-4.8
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU_4.8-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug_DLL
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1360937237/billboard.o \
	${OBJECTDIR}/_ext/1360937237/bitmap.o \
	${OBJECTDIR}/_ext/1360937237/camera.o \
	${OBJECTDIR}/_ext/1360937237/display.o \
	${OBJECTDIR}/_ext/1360937237/drawable.o \
	${OBJECTDIR}/_ext/1360937237/font.o \
	${OBJECTDIR}/_ext/1360937237/gBuffer.o \
	${OBJECTDIR}/_ext/1360937237/geometry.o \
	${OBJECTDIR}/_ext/1360937237/harbinger.o \
	${OBJECTDIR}/_ext/1360937237/input.o \
	${OBJECTDIR}/_ext/1360937237/light.o \
	${OBJECTDIR}/_ext/1360937237/mesh.o \
	${OBJECTDIR}/_ext/1360937237/pipeline.o \
	${OBJECTDIR}/_ext/1360937237/primitives.o \
	${OBJECTDIR}/_ext/1360937237/resource.o \
	${OBJECTDIR}/_ext/1360937237/shader.o \
	${OBJECTDIR}/_ext/1360937237/shadowMap.o \
	${OBJECTDIR}/_ext/1360937237/skybox.o \
	${OBJECTDIR}/_ext/1360937237/stockShaders.o \
	${OBJECTDIR}/_ext/1360937237/text.o \
	${OBJECTDIR}/_ext/1360937237/transformations.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC
CXXFLAGS=-m32 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lglew32 -lglfw3dll -lopengl32 -lFreeImage -lassimp.dll -lfreetype.dll ../../hamlibs/HamLibs_NetBeans/../bin/WIN32/libhamlibs-debug.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../bin/WIN32/harbinger-debug.dll

../bin/WIN32/harbinger-debug.dll: ../../hamlibs/HamLibs_NetBeans/../bin/WIN32/libhamlibs-debug.a

../bin/WIN32/harbinger-debug.dll: ${OBJECTFILES}
	${MKDIR} -p ../bin/WIN32
	${LINK.cc} -o ../bin/WIN32/harbinger-debug.dll ${OBJECTFILES} ${LDLIBSOPTIONS} -Wl,--out-implib,../bin/WIN32/libharbinger-debug.dll.a -shared -fPIC

${OBJECTDIR}/_ext/1360937237/billboard.o: nbproject/Makefile-${CND_CONF}.mk ../src/billboard.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/billboard.o ../src/billboard.cpp

${OBJECTDIR}/_ext/1360937237/bitmap.o: nbproject/Makefile-${CND_CONF}.mk ../src/bitmap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/bitmap.o ../src/bitmap.cpp

${OBJECTDIR}/_ext/1360937237/camera.o: nbproject/Makefile-${CND_CONF}.mk ../src/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/camera.o ../src/camera.cpp

${OBJECTDIR}/_ext/1360937237/display.o: nbproject/Makefile-${CND_CONF}.mk ../src/display.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/display.o ../src/display.cpp

${OBJECTDIR}/_ext/1360937237/drawable.o: nbproject/Makefile-${CND_CONF}.mk ../src/drawable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/drawable.o ../src/drawable.cpp

${OBJECTDIR}/_ext/1360937237/font.o: nbproject/Makefile-${CND_CONF}.mk ../src/font.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/font.o ../src/font.cpp

${OBJECTDIR}/_ext/1360937237/gBuffer.o: nbproject/Makefile-${CND_CONF}.mk ../src/gBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/gBuffer.o ../src/gBuffer.cpp

${OBJECTDIR}/_ext/1360937237/geometry.o: nbproject/Makefile-${CND_CONF}.mk ../src/geometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/geometry.o ../src/geometry.cpp

${OBJECTDIR}/_ext/1360937237/harbinger.o: nbproject/Makefile-${CND_CONF}.mk ../src/harbinger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/harbinger.o ../src/harbinger.cpp

${OBJECTDIR}/_ext/1360937237/input.o: nbproject/Makefile-${CND_CONF}.mk ../src/input.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/input.o ../src/input.cpp

${OBJECTDIR}/_ext/1360937237/light.o: nbproject/Makefile-${CND_CONF}.mk ../src/light.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/light.o ../src/light.cpp

${OBJECTDIR}/_ext/1360937237/mesh.o: nbproject/Makefile-${CND_CONF}.mk ../src/mesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/mesh.o ../src/mesh.cpp

${OBJECTDIR}/_ext/1360937237/pipeline.o: nbproject/Makefile-${CND_CONF}.mk ../src/pipeline.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/pipeline.o ../src/pipeline.cpp

${OBJECTDIR}/_ext/1360937237/primitives.o: nbproject/Makefile-${CND_CONF}.mk ../src/primitives.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/primitives.o ../src/primitives.cpp

${OBJECTDIR}/_ext/1360937237/resource.o: nbproject/Makefile-${CND_CONF}.mk ../src/resource.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/resource.o ../src/resource.cpp

${OBJECTDIR}/_ext/1360937237/shader.o: nbproject/Makefile-${CND_CONF}.mk ../src/shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/shader.o ../src/shader.cpp

${OBJECTDIR}/_ext/1360937237/shadowMap.o: nbproject/Makefile-${CND_CONF}.mk ../src/shadowMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/shadowMap.o ../src/shadowMap.cpp

${OBJECTDIR}/_ext/1360937237/skybox.o: nbproject/Makefile-${CND_CONF}.mk ../src/skybox.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/skybox.o ../src/skybox.cpp

${OBJECTDIR}/_ext/1360937237/stockShaders.o: nbproject/Makefile-${CND_CONF}.mk ../src/stockShaders.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/stockShaders.o ../src/stockShaders.cpp

${OBJECTDIR}/_ext/1360937237/text.o: nbproject/Makefile-${CND_CONF}.mk ../src/text.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/text.o ../src/text.cpp

${OBJECTDIR}/_ext/1360937237/transformations.o: nbproject/Makefile-${CND_CONF}.mk ../src/transformations.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} $@.d
	$(COMPILE.cc) -w -DDEBUG -DHGE_BUILD_DYNAMIC -I../include -I../../hamlibs/include -I../../../../../../mingw4.8.1/mingw32/i686-w64-mingw32/include/freetype2 -std=c++11 -g -Wall -Werror -Wextra -pedantic -mmmx -msse -msse2 -msse3 -mfpmath=sse -O2 -s -shared -fPIC -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360937237/transformations.o ../src/transformations.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../bin/WIN32/harbinger-debug.dll

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
