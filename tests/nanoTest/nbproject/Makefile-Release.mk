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
	${OBJECTDIR}/_ext/968701377/file.o \
	${OBJECTDIR}/_ext/968701377/vfs.o \
	${OBJECTDIR}/_ext/1109276195/mempool.o \
	${OBJECTDIR}/_ext/35041009/membase.o \
	${OBJECTDIR}/_ext/35041009/strings.o \
	${OBJECTDIR}/_ext/683473409/list.o \
	${OBJECTDIR}/_ext/683473409/tree.o \
	${OBJECTDIR}/kerror_mocked.o \
	${OBJECTDIR}/kmalloc_mocked.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/print_mocked.o \
	${OBJECTDIR}/vfs_test.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nanotest

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nanotest: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nanotest ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/968701377/file.o: ../../kernel/fs/file.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/968701377
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/968701377/file.o ../../kernel/fs/file.c

${OBJECTDIR}/_ext/968701377/vfs.o: ../../kernel/fs/vfs.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/968701377
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/968701377/vfs.o ../../kernel/fs/vfs.c

${OBJECTDIR}/_ext/1109276195/mempool.o: ../../kernel/mempool.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1109276195
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1109276195/mempool.o ../../kernel/mempool.c

${OBJECTDIR}/_ext/35041009/membase.o: ../../kernel/std/membase.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/35041009
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/35041009/membase.o ../../kernel/std/membase.c

${OBJECTDIR}/_ext/35041009/strings.o: ../../kernel/std/strings.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/35041009
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/35041009/strings.o ../../kernel/std/strings.c

${OBJECTDIR}/_ext/683473409/list.o: ../../kernel/utils/list.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/683473409
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/683473409/list.o ../../kernel/utils/list.c

${OBJECTDIR}/_ext/683473409/tree.o: ../../kernel/utils/tree.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/683473409
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/683473409/tree.o ../../kernel/utils/tree.c

${OBJECTDIR}/kerror_mocked.o: kerror_mocked.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/kerror_mocked.o kerror_mocked.c

${OBJECTDIR}/kmalloc_mocked.o: kmalloc_mocked.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/kmalloc_mocked.o kmalloc_mocked.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/print_mocked.o: print_mocked.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/print_mocked.o print_mocked.c

${OBJECTDIR}/vfs_test.o: vfs_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -I../../include -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/vfs_test.o vfs_test.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nanotest

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
