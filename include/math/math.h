//various math operations

#ifndef __HL_MATH_H__
#define __HL_MATH_H__

/*
 * NOTES:
 * All angle measurements used by this library are in radians
 * I also use constructors rather than assignments when creating objects and
 * variables, so this might make readability slightly difficult at first.
 */

#include <cmath>
#include <cstdint>

/*
 * Floating Point Values & Precision
 */
#ifdef HL_HIGH_PRECISION
	#define HL_FLOAT        double
    #define HL_INT          int64_t

    #define HL_ROUND(x)		(std::floor(x + 0.5))
    #define HL_DEG2RAD(x)	(x * 0.01745329251994329577) // x * (PI / 180)
    #define HL_RAD2DEG(x)	(x * 57.2957795130823208768) // x * (180 / PI)
    #define HL_SQRT( x )    sqrt( x )

    #define HL_PI           3.14159265358979323846
    #define HL_TWO_PI       6.28318530717958647693
    #define HL_PI_OVR2      1.57079632679489661923
    #define HL_PI_OVR3      1.04719755119659774615
    #define HL_PI_OVR4      0.78539816339744830962
    #define HL_PI_OVR6      0.52359877559829887307
    #define HL_PI_INV       0.31830988618379067153 // 1 / pi
    #define HL_E            2.71828182845904523536
    #define HL_EPSILON      1.0e-9

#else
	#define HL_FLOAT        float
    #define HL_INT          int32_t

    #define HL_ROUND(x)		(std::floor(x + 0.5f))
    #define HL_DEG2RAD(x)	(x * 0.01745329251994329577f) // x * (PI / 180)
    #define HL_RAD2DEG(x)	(x * 57.2957795130823208768f) // x * (180 / PI)
    #define HL_SQRT( x )    sqrtf( x )

    #define HL_PI           3.14159265358979323846f
    #define HL_TWO_PI       6.28318530717958647693f
    #define HL_PI_OVR2      1.57079632679489661923f
    #define HL_PI_OVR3      1.04719755119659774615f
    #define HL_PI_OVR4      0.78539816339744830962f
    #define HL_PI_OVR6      0.52359877559829887307f
    #define HL_PI_INV       0.31830988618379067153f // 1 / pi
    #define HL_E            2.71828182845904523536f
    #define HL_EPSILON      1.0e-5f
#endif

namespace hamLibs {
namespace math {

	//-------------------------------------------------------------
	//				Prototypes & Declarations
	//-------------------------------------------------------------
    // Fixed-point numbers
    template <typename fixedBase, unsigned fracDigits> class fixed_t;
    
	//Multidimensional math classes
	template <class numType>        class mat2_t;
	template <class numType>        class mat3_t;
	template <class numType>        class mat4_t;
	template <class numType>        class quat_t;
	template <class numType>        class vec2_t;
	template <class numType>        class vec3_t;
	template <class numType>        class vec4_t;

	//[Hopefully] Useful Typedefs
    typedef fixed_t<int32_t, 7>     lowp_t;         // 24.7 (-1 for the sign-bit)
    typedef fixed_t<int32_t, 15>    medp_t;         // 16.15
    typedef fixed_t<int32_t, 23>    highp_t;        // 8.23
    
    typedef fixed_t<int64_t, 15>    long_lowp_t;    // 48.16
    typedef fixed_t<int64_t, 31>    long_medp_t;    // 32.32
    typedef fixed_t<int64_t, 47>    long_highp_t;   // 16.48
    
	typedef quat_t	<float>         quatf;		//Quaternions
	typedef quat_t	<double>        quatd;
	typedef quat_t	<int>           quati;
	typedef quat_t	<medp_t>        quatFixed;
	typedef vec2_t	<float>         vec2f;		//2D vectors
	typedef vec2_t	<double>        vec2d;
	typedef vec2_t	<int>           vec2i;
	typedef vec2_t	<medp_t>        vec2Fixed;
	typedef vec3_t	<float>         vec3f;		//3D vectors
	typedef vec3_t	<double>        vec3d;
	typedef vec3_t	<int>           vec3i;
	typedef vec3_t	<medp_t>        vec3Fixed;
	typedef vec4_t	<float>         vec4f;		//4D vectors
	typedef vec4_t	<double>        vec4d;
	typedef vec4_t	<int>           vec4i;
	typedef vec4_t	<medp_t>        vec4Fixed;
	typedef mat2_t	<float>         mat2f;		//2x2 matrices
	typedef mat2_t	<double>        mat2d;
	typedef mat2_t	<int>           mat2i;
	typedef mat2_t	<medp_t>        mat2Fixed;
	typedef mat3_t	<float>         mat3f;		//3x3 matrices
	typedef mat3_t	<double>        mat3d;
	typedef mat3_t	<int>           mat3i;
	typedef mat3_t	<medp_t>        mat3Fixed;
	typedef mat4_t	<float>         mat4f;		//4x4 matrices
	typedef mat4_t	<double>        mat4d;
	typedef mat4_t	<int>           mat4i;
	typedef mat4_t	<medp_t>        mat4Fixed;
    
    typedef fixed_t <HL_INT, 7>     lowp;       // 24.7 (-1 for the sign-bit)
    typedef fixed_t <HL_INT, 15>    medp;       // 16.15
    typedef fixed_t <HL_INT, 23>    highp;      // 8.23
    
	typedef quat_t	<HL_FLOAT>      quat;
	typedef vec2_t	<HL_FLOAT>      vec2;
	typedef vec3_t	<HL_FLOAT>      vec3;
	typedef vec4_t	<HL_FLOAT>      vec4;
	typedef mat2_t	<HL_FLOAT>      mat2;
	typedef mat3_t	<HL_FLOAT>      mat3;
	typedef mat4_t	<HL_FLOAT>      mat4;
	
	template <typename numType>	inline      numType fastSqrt(numType);
	template <typename numType>	inline      numType fastInvSqrt(numType);
	template <>                 inline      float   fastSqrt< float >(float);
	template <>                 inline      float   fastInvSqrt< float >(float);
    
	template <typename numType>	constexpr   numType degToRad(numType);
	template <typename numType>	constexpr   numType radToDeg(numType);
    
    template <typename numType> inline      numType fastLog2( numType );
    template <>                 inline      float   fastLog2< float >( float );
    template <typename numType> inline      numType fastLog( numType );
    template <typename numType> inline      numType fastLogBase( numType base, numType );

	//-------------------------------------------------------------
	//				Definitions
	//-------------------------------------------------------------
	/* Fast inverse square root method adopted for regular square rooting.
	 * this method was found at:
	 *		http://rrrola.wz.cz/inv_sqrt.html
	 * and:
	 *		http://jheriko-rtw.blogspot.com/2009/04/understanding-and-improving-fast.html
	 */
	template <typename numType> inline
	numType fastInvSqrt(numType input) {
		float x = static_cast<float>(input);
		union { float f; unsigned int u; } y = {x};
		y.u = 0x5F1FFFF9ul - (y.u >> 1);
		return numType(0.703952253f * y.f * (2.38924456f - x * y.f * y.f));
	}

	//-----------------------------------------------------------------
	template <typename numType> inline
	numType fastSqrt(numType input) {
		return numType( 1.0f/fastInvSqrt( input ) );
	}

	//-----------------------------------------------------------------
	template <> inline
	float fastInvSqrt< float >( float x ) {
		union { float f; unsigned int u; } y = {x};
		y.u = 0x5F1FFFF9ul - (y.u >> 1);
		return 0.703952253f * y.f * (2.38924456f - x * y.f * y.f);
	}

	//-----------------------------------------------------------------
	template <> inline
	float fastSqrt< float >(float input) {
		return float( 1.0f/fastInvSqrt( input ) );
	}

	//-----------------------------------------------------------------
	template <typename numType> inline
	constexpr numType degToRad(numType input) {
		return HL_DEG2RAD(input);
	}

	//-----------------------------------------------------------------
	template <typename numType> inline
	constexpr numType radToDeg(numType input) {
		return HL_RAD2DEG(input);
	}
    
	//-----------------------------------------------------------------
    /*
     * Fast Approximate logarithms
     * This method was found on flipcode:
     * http://www.flipcode.com/archives/Fast_log_Function.shtml
     * 
     * Accurate to within 5 decimal places.
     * This method relies on the IEEE floating point specification
     */
    template < typename numType > inline
    numType fastLog2( numType n ) {
        float val = (float)n;
        int* const exp = reinterpret_cast< int* >( &val );
        int x = *exp;

        const int log2 = ( (x >> 23) & 255 ) - 128;

        x &= ~(255 << 23);
        x += 127 << 23;

        *exp = x;
        val = ((-1.f/3.f) * val+2.f) * val - 2.f / 3.f;
        return (numType)val + log2;
    }
    
	//-----------------------------------------------------------------
    template <> inline
    float fastLog2< float >( float n ) {
        int* const exp = reinterpret_cast< int* >( &n );
        int x = *exp;

        const int log2 = ( (x >> 23) & 255 ) - 128;

        x &= ~(255 << 23);
        x += 127 << 23;

        *exp = x;
        n = ((-1.f/3.f) * n+2.f) * n - 2.f / 3.f;
        return n + log2;
    }

	//-----------------------------------------------------------------
    template < typename numType > inline
    numType fastLog( numType n ) {
        return fastLog2( n ) * 0.693147181f; // ln( 2 )
    }
    
	//-----------------------------------------------------------------
    template < typename numType > inline
    numType fastLogBase( numType base, numType n ) {
        return fastLog2( n ) / fastLog2( base );
    }

}//end math namespace
}//end hamlibs namespace

#include "fixed.h"
#include "matrix2.h"
#include "matrix3.h"
#include "matrix4.h"
#include "quat.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

#include "quat_utils.h"
#include "vec_utils.h"
#include "mat_utils.h"

#endif /* __HL_MATH_H__ */
