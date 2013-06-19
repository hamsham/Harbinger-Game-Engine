/* 
 * File:   setup.h
 * Author: hammy
 *
 * Created on January 9, 2013, 11:49 AM
 */

#ifndef __HL_PREPROCESSOR_H__
#define	__HL_PREPROCESSOR_H__

/******************************************************************************
		C++11 Support
******************************************************************************/
#if (__cplusplus < 201103L) || (!defined(__GXX_EXPERIMENTAL_CXX0X__))
	//#error "A C++11-compliant compiler is required to build Ham's libraries"
#endif

/******************************************************************************
		Compiler Compatibility
******************************************************************************/
#if defined (_MSC_VER)
	#define HL_COMPILER_MSC _MSC_VER

#elif defined (__GNUC__) || defined (__MINGW32__) || defined (__MINGW_64__)
	#define HL_COMPILER_GNU __GNUC__
	#if defined (__clang__)
		#define HL_COMPILER_CLANG 1
		#define HL_COMPILER_CLANG_MAJ __clang_major__
		#define HL_COMPILER_CLANG_MIN __clang_minor__
	#endif

#elif defined (__INTEL_COMPILER)
	#define HL_COMPILER_INTEL __INTEL_COMPILER

#elif defined (__BORLANDC__) || defined (__CODEGEARC__)
	#define HL_COMPILER_BORLAND HL_MAX( (__BORLANDC__), (__CODEGEARC__) )

#elif defined (__CC_ARM)
	#define HL_COMPILER_ARM __ARMCC_VERSION

#elif defined (__IBMCPP__)
	#ifdef(__COMPILER_VER__)
		/* IBM z/OS Compiler */
		#define HL_COMPILER_IBM 0
		#define HL_COMPILER_ZOS __IBMCPP__
	#else
		/* IBM XL Compiler */
		#define HL_COMPILER_IBM 1
		#define HL_COMPILER_XL __IBMCPP__
	#endif

#elif defined (__SNC__)
	#define HL_COMPILER_SN __SNC__

#endif

/******************************************************************************
		Build System Architectures
******************************************************************************/
/*
 * x86/x64 Processor Family
 * 
 * Define HL_ARCH_X86 if an x86 processor is found
 * Define HL_ARCH_X86 as 32 if compiling on a standard 32-bit machine
 * Define HL_ARCH_X86 as 64 if compiling on 64-bit capable hardware
 */
#if defined (_WIN64) || defined (__amd64__) || defined (_M_X64)
	#define HL_ARCH_X86 64
	#define HL_ARCH_X86_VER 6 /* Assuming modern processors */

#elif defined (_WIN32) || defined (__i386__) || defined (_M_IX86_) || defined (__THW_INTEL__)
	#ifndef HL_ARCH_x86
		#define HL_ARCH_X86 32
	#endif

	/* x86 Versioning */
	#ifdef HL_COMPILER_GNU
		#if defined (__i686__)
			#define HL_ARCH_X86_VER 6
		#elif defined (__i586__)
			#define HL_ARCH_X86_VER 5
		#elif defined (__i486__)
			#define HL_ARCH_X86_VER 4
		#elif defined (__i386__)
			#define HL_ARCH_X86_VER 3
		#endif
	#elif defined (HL_COMPILER_MSC)
		#define HL_ARCH_X86_VER ((_M_IX86) / 100)
	#else
		#define HL_ARCH_X86_VER 3
	#endif
#endif

/* Itanium */
#if defined (__IA64__) || defined (_M_IA64) || defined (__itanium__)
	#define HL_ARCH_IA64 1
#endif

/* Arm Processors */
#if defined (__arm__) || defined (_M_ARM) || defined (__TARGET_ARCH_ARM)
	#define HL_ARCH_ARM 1
	#define HL_ARCH_ARM_VER HL_MAX( (__TARGET_ARCH_ARM), (_M_ARM) )

	// Thumb-Mode
	#if defined (__thumb__) || defined (__TARGET_ARCH_THUMB) || defined (_M_ARMT)
		#define HL_ARCH_ARM_THUMB 1
		#define HL_ARCH_ARM_THUMB_VER HL_MAX( (__TARGET_ARCH_THUMB), (_M_ARMT) )

	#endif
#endif

/* PowerPC */
#if defined (__ppc__) || defined (_M_PPC) || defined (_ARCH_PPC)
	#define HL_ARCH_PPC 1
#endif

/* Gaming Consoles */
#if defined (__SNC__)
	#define HL_ARCH_PS3 1
#endif

/******************************************************************************
		Target Operating Systems
******************************************************************************/
/* Android */
#if defined (__ANDROID__)
	#define HL_OS_ANDROID 1
	#define HL_OS_ANDROID_VER __ANDROID_API__
#endif

/* CygWin */
#if defined (__CYGWIN__)
	#define HL_OS_CYGWIN 1
#endif

/* FreeBSD */
#if defined (__FreeBSD__)
	#define HL_OS_BSD 1
	#define HL_OS_BSD_VER __FreeBSD__
#endif

/* GNU/Linux */
#if defined (__GNU__)
	#define HL_OS_GNU 1
#endif

/* Mac OSX */
#if defined (macintosh) || defined (Macintosh) || defined (__APPLE__) || defined (__MACH__)
	#define HL_OS_OSX 1
#endif

/* Unix */
#if defined (__unix__) || defined (__unix)
	#define HL_OS_UNIX 1
#endif

/* Windows */
#if defined (_WIN32) || defined (_WIN64) || defined (__WINDOWS__) || defined (__WIN32__)
	#if defined (_WIN64)
		#define HL_OS_WINDOWS 64
	#else 
		#define HL_OS_WINDOWS 32
	#endif
#endif

/******************************************************************************
		Function Attributes
******************************************************************************/
/*
 * Object Inlining and Calling Conventions to be used in performance-sensitive functions
 */
#if defined (HL_COMPILER_MSC) || defined (HL_COMPILER_BORLAND)
	#define HL_INLINE __forceinline
	#ifdef HL_ARCH_X86
		#define HL_FASTCALL __fastcall
	#endif

#elif defined (HL_COMPILER_GNU)
	#define HL_INLINE inline __attribute__((always_inline))
	#ifdef HL_ARCH_X86
		#if HL_ARCH_X86 == 32
			#define HL_FASTCALL __attribute__((__fastcall__))
		#else
			#define HL_FASTCALL
		#endif
	#endif
#else
	#define HL_INLINE inline
	#define HL_FASTCALL
#endif

#ifndef HL_IMPERATIVE
	#define HL_IMPERATIVE HL_INLINE HL_FASTCALL
#endif

/*
 * Dynamic Library Support
 */
#if defined (HL_BUILD_DYNAMIC) && defined (HL_OS_WINDOWS)
		#define HL_API __declspec( dllexport )
#elif defined (HL_API_DYNAMIC) && defined (HL_OS_WINDOWS)
		#define HL_API __declspec( dllimport )
#else
	#define HL_API
#endif

/******************************************************************************
		HamLibs Macros
******************************************************************************/
/*
 * Preprocessor Stringify
 */
#ifndef HL_STRINGIFY
	#define HL_STRINGIFY( x ) #x
#endif /* HL_STRINGIFY */

/*
 * Max/Min
 */
#define HL_MAX( x, y ) (( x > y) ? x : y )
#define HL_MIN( x, y ) (( x < y) ? x : y )

#endif	/* __HL_PREPROCESSOR_H__ */
