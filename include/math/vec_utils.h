/* 
 * File:   vec_utils.h
 * Author: hammy
 *
 * Created on January 8, 2013, 3:14 PM
 */

#ifndef __HL_VEC_UTILS_H__
#define	__HL_VEC_UTILS_H__

#include "math.h"

namespace hamLibs {

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
	namespace math {
//-----------------------------------------------------------------------------
// 2D Vectors
//-----------------------------------------------------------------------------
template <typename numType> inline numType			dot( const vec2_t<numType>&, const vec2_t<numType>& );

template <typename numType> inline vec2_t<numType>	normalize( const vec2_t<numType>& );

template <typename numType> inline numType			magnitude( const vec2_t<numType>& );

template <typename numType> inline vec2_t<numType>	rotate( const vec2_t<numType>&, numType );

template <typename numType> inline numType			angleBetween( const vec2_t<numType>&, const vec2_t<numType>& );
template <typename numType> inline numType			angleBetween( const vec2_t<numType>&, const vec2_t<numType>&, const vec2_t<numType>& origin );

//-----------------------------------------------------------------------------
// 3D Vectors
//-----------------------------------------------------------------------------
template <typename numType> inline numType			dot( const vec3_t<numType>&, const vec3_t<numType>& );

template <typename numType> inline vec3_t<numType>	cross( const vec3_t<numType>&, const vec3_t<numType>& );

template <typename numType> inline vec3_t<numType>	normalize( const vec3_t<numType>& );

template <typename numType> inline numType			magnitude( const vec3_t<numType>& );

template <typename numType> inline vec3_t<numType>	xRotation( numType );
template <typename numType> inline vec3_t<numType>	yRotation( numType );
template <typename numType> inline vec3_t<numType>	zRotation( numType );

template <typename numType> inline numType			angleBetween( const vec3_t<numType>&, const vec3_t<numType>& );
template <typename numType> inline numType			angleBetween( const vec3_t<numType>&, const vec3_t<numType>&, const vec3_t<numType>& origin );

//-----------------------------------------------------------------------------
// 4D Vectors
//-----------------------------------------------------------------------------
template <typename numType> inline numType			dot( const vec4_t<numType>&, const vec4_t<numType>& );

template <typename numType> inline vec4_t<numType>	normalize( const vec4_t<numType>& );

template <typename numType> inline numType			magnitude( const vec4_t<numType>& );

template <typename numType> inline numType			angleBetween( const vec4_t<numType>&, const vec4_t<numType>& );
template <typename numType> inline numType			angleBetween( const vec3_t<numType>&, const vec3_t<numType>&, const vec3_t<numType>& origin );

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
	} // end math namespace
//-----------------------------------------------------------------------------
// 2D Vectors
//-----------------------------------------------------------------------------
template <typename numType> inline
numType math::dot( const vec2_t<numType>& v1, const vec2_t<numType>& v2 ) {
	return (v1.v[0]*v2.v[0]) + (v1.v[1]*v2.v[1]);
}

template <typename numType> inline
math::vec2_t<numType> math::normalize( const vec2_t<numType>& v ) {
	numType magInv(
		numType(1) / HL_SQRT(
			(v.v[0] * v.v[0])+
			(v.v[1] * v.v[1])
		)
	);
	return vec2_t<numType>(
		v.v[0]*magInv,
		v.v[1]*magInv
	);
}

template <typename numType> inline
numType math::magnitude( const vec2_t<numType>& v ) {
	return numType(
		HL_SQRT(
			(v.v[0] * v.v[0]) +
			(v.v[1] * v.v[1])
		)
	);
}

template <typename numType> inline
math::vec2_t<numType> math::rotate( const vec2_t<numType>& v, numType angle ) {
	numType s = sin(angle);
	numType c = cos(angle);

	return vec2_t<numType>(
		(v.v[0]*c) - (v.v[1]*s),
		(v.v[0]*s) + (v.v[1]*c)
	);
}

template <typename numType> inline
numType math::angleBetween( const vec2_t<numType>& v1, const vec2_t<numType>& v2 ) {
	return numType( acos(
			dot( v1, v2 ) / ( magnitude( v1 )*magnitude( v2 ) )
	) );
}

template <typename numType> inline
numType math::angleBetween( const vec2_t<numType>& v1, const vec2_t<numType>& v2, const vec2_t<numType>& origin ) {
	return numType( acos(
			dot( v1-origin, v2-origin ) / ( magnitude( v1 )*magnitude( v2 ) )
	) );
}

//-----------------------------------------------------------------------------
// 3D Vectors
//-----------------------------------------------------------------------------
template <typename numType> inline
numType math::dot( const vec3_t<numType>& v1, const vec3_t<numType>& v2 ) {
	return	(v1.v[0] * v2.v[0]) + (v1.v[1] * v2.v[1]) + (v1.v[2] * v2.v[2]);
}

template <typename numType> inline
math::vec3_t<numType> math::cross( const vec3_t<numType>& v1, const vec3_t<numType>& v2 ) {
	return vec3_t<numType>(
		(v1.v[1]*v2.v[2]) - (v1.v[2]*v2.v[1]),
		(v1.v[2]*v2.v[0]) - (v1.v[0]*v2.v[2]),
		(v1.v[0]*v2.v[1]) - (v1.v[1]*v2.v[0])
	);
}

template <typename numType> inline
math::vec3_t<numType> math::normalize( const vec3_t<numType>& v ) {
	numType magInv(
		numType(1) / HL_SQRT(
			(v.v[0] * v.v[0]) +
			(v.v[1] * v.v[1]) +
			(v.v[2] * v.v[2])
		)
	);
	return vec3_t<numType>(
		v.v[0]*magInv,
		v.v[1]*magInv,
		v.v[2]*magInv
	);
}

template <typename numType> inline
numType math::magnitude( const vec3_t<numType>& v ) {
	return numType(
		HL_SQRT(
			(v.v[0] * v.v[0]) +
			(v.v[1] * v.v[1]) +
			(v.v[2] * v.v[2])
		)
	);
}

template <typename numType> inline
math::vec3_t<numType> math::xRotation( numType angle ) {
	numType s = sin(angle);
	numType c = cos(angle);

	return vec3_t<numType>(
		numType(1),
		c + s,
		c - s
	);
}

template <typename numType> inline
math::vec3_t<numType> math::yRotation( numType angle ) {
	numType s = sin(angle);
	numType c = cos(angle);

	return vec3_t<numType>(
		c - s,
		numType(1),
		s + c
	);
}

template <typename numType> inline
math::vec3_t<numType> math::zRotation( numType angle ) {
	numType s = sin(angle);
	numType c = cos(angle);

	return vec3_t<numType>(
		c + s,
		c - s,
		numType(1)
	);
}

template <typename numType> inline
numType math::angleBetween( const vec3_t<numType>& v1, const vec3_t<numType>& v2 ) {
	return numType( acos(
			dot( v1, v2 ) / ( magnitude( v1 )*magnitude( v2 ) )
	) );
}

template <typename numType> inline
numType math::angleBetween( const vec3_t<numType>& v1, const vec3_t<numType>& v2, const vec3_t<numType>& origin ) {
	return numType( acos(
			dot( v1-origin, v2-origin ) / ( magnitude( v1 )*magnitude( v2 ) )
	) );
}

//-----------------------------------------------------------------------------
// 4D Vectors
//-----------------------------------------------------------------------------
template <typename numType> inline
numType math::dot( const vec4_t<numType>& v1, const vec4_t<numType>& v2 ) {
	return	(v1.v[0] * v2.v[0]) + (v1.v[1] * v2.v[1]) + (v1.v[2] * v2.v[2]) + (v1.v[3] * v2.v[3]);
}

template <typename numType> inline
math::vec4_t<numType> math::normalize( const vec4_t<numType>& v ) {
	numType magInv(
		numType(1) / HL_SQRT(
			(v.v[0] * v.v[0]) +
			(v.v[1] * v.v[1]) +
			(v.v[2] * v.v[2]) +
			(v.v[3] * v.v[3])
		)
	);
	return vec4_t<numType>(
		v.v[0]*magInv,
		v.v[1]*magInv,
		v.v[2]*magInv,
		v.v[3]*magInv
	);
}

template <typename numType> inline
numType math::magnitude( const vec4_t<numType>& v ) {
	return numType(
		HL_SQRT(
			(v.v[0] * v.v[0]) +
			(v.v[1] * v.v[1]) +
			(v.v[2] * v.v[2]) +
			(v.v[3] * v.v[3])
		)
	);
}

template <typename numType> inline
numType math::angleBetween( const vec4_t<numType>& v1, const vec4_t<numType>& v2 ) {
	return numType( acos(
			dot( v1, v2 ) / ( magnitude( v1 )*magnitude( v2 ) )
	) );
}

template <typename numType> inline
numType math::angleBetween( const vec4_t<numType>& v1, const vec4_t<numType>& v2, const vec4_t<numType>& origin ) {
	return numType( acos(
			dot( v1-origin, v2-origin ) / ( magnitude( v1 )*magnitude( v2 ) )
	) );
}

} // hamLibs namespace

#endif	/* __HL_VEC_UTILS_H__ */
