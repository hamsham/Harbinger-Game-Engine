/* 
 * File:   mat_utils.h
 * Author: hammy
 *
 * Created on January 8, 2013, 3:48 PM
 */

#ifndef __HL_MAT_UTILS_H__
#define	__HL_MAT_UTILS_H__

#include "math.h"

namespace hamLibs {

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
	namespace math {
//-----------------------------------------------------------------------------
//2x2 Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline numType			determinant( const mat2_t<numType>& );

template <typename numType> inline mat2_t<numType>	inverse( const mat2_t<numType>& );

template <typename numType> inline mat2_t<numType>	transpose( const mat2_t<numType>& );

template <typename numType> inline mat2_t<numType>	rotate( const mat2_t<numType>&, numType );

template <typename numType> inline mat2_t<numType>	scale( const mat2_t<numType>&, const vec2_t<numType>& );
	
//-----------------------------------------------------------------------------
// 3x3 Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline numType			determinant( const mat3_t<numType>& );

template <typename numType> inline mat3_t<numType>	inverse( const mat3_t<numType>& );

template <typename numType> inline mat3_t<numType>	transpose( const mat3_t<numType>& );

template <typename numType> inline mat3_t<numType>	rotate( const mat3_t<numType>&, const vec3_t<numType>&, numType );

template <typename numType> inline mat3_t<numType>	scale( const mat3_t<numType>&, const vec3_t<numType>& );

//-----------------------------------------------------------------------------
// 4x4 Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline numType			determinant( const mat4_t<numType>& );

template <typename numType> inline mat4_t<numType>	inverse( const mat4_t<numType>& );

template <typename numType> inline mat4_t<numType>	transpose( const mat4_t<numType>& );

template <typename numType> inline mat4_t<numType>	rotate( const mat4_t<numType>&, const vec3_t<numType>&, numType );

template <typename numType> inline mat4_t<numType>	scale( const mat4_t<numType>&, const vec3_t<numType>& );

template <typename numType> inline mat4_t<numType>	translate( const mat4_t<numType>&, const vec3_t<numType>& );

template <typename numType> inline mat4_t<numType>	perspective( numType fov, numType aspect, numType zNear, numType zFar );

template <typename numType> inline mat4_t<numType>	infinitePerspective( numType fov, numType aspect, numType zNear );

template <typename numType> inline mat4_t<numType>	ortho( numType left, numType right, numType top, numType bottom );

template <typename numType> inline mat4_t<numType>	ortho( numType left, numType right, numType top, numType bottom, numType near, numType far );

template <typename numType> inline mat4_t<numType>	frustum( numType left, numType right, numType top, numType bottom, numType near, numType far );

template <typename numType> inline mat4_t<numType>	lookAt( const vec3_t<numType>& pos, const vec3_t<numType>& target, const vec3_t<numType>& up );

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
	} // end math namespace
//-----------------------------------------------------------------------------
//2x2 Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline
numType math::determinant( const mat2_t<numType>& m ) {
	return (m.m[0][0]*m.m[1][1]) - (m.m[0][1]*m.m[1][0]);
}

template <typename numType> inline
math::mat2_t<numType> math::inverse( const mat2_t<numType>& m ) {
	numType determinantInv(
		numType( 1 ) / ( (m.m[0][0] * m.m[1][1]) - (m.m[0][1] * m.m[1][0]) )
	);
	
	return mat2_t<numType>(
		m.m[1][1] * determinantInv, m.m[0][1] * determinantInv,
		m.m[1][0] * determinantInv, m.m[0][0] * determinantInv
	);
}

template <typename numType> inline
math::mat2_t<numType> math::transpose( const mat2_t<numType>& m ) {
	return mat2_t<numType>(
		m.m[0][0], m.m[1][0],
		m.m[0][1], m.m[1][1]
	);
}

template <typename numType> inline
math::mat2_t<numType> math::rotate( const mat2_t<numType>& m, numType angle ) {
	numType c( std::cos( angle ) );
	numType s( std::sin( angle ) );
	return mat2_t<numType>(
		c,	s,
		-s,	c
	) * m;
}

template <typename numType> inline
math::mat2_t<numType> math::scale( const mat2_t<numType>& m, const vec2_t<numType>& amount ) {
	return mat2_t<numType>(
		m.m[0][0]*amount.v[0], m.m[0][1],
		m.m[1][0], m.m[1][1]*amount.v[1]
	);
}
	
//-----------------------------------------------------------------------------
// 3x3 Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline
numType math::determinant( const mat3_t<numType>& m ) {
	return
		(m.m[0][0]*m.m[1][1]*m.m[2][2]) +
		(m.m[0][1]*m.m[1][2]*m.m[2][0]) +
		(m.m[0][2]*m.m[1][0]*m.m[2][1]) -
		(m.m[0][2]*m.m[1][1]*m.m[2][0]) -
		(m.m[0][1]*m.m[1][0]*m.m[2][2]) -
		(m.m[0][0]*m.m[1][2]*m.m[2][1]);
}

template <typename numType> inline
math::mat3_t<numType> math::inverse( const mat3_t<numType>& m ) {
	numType detInv( numType(1) / determinant( m ) );
	return
	mat3_t<numType>(
		m.m[0][0] * detInv, m.m[1][0] * detInv, m.m[2][0] * detInv,
		m.m[0][1] * detInv, m.m[1][1] * detInv, m.m[2][1] * detInv,
		m.m[0][2] * detInv, m.m[1][2] * detInv, m.m[2][2] * detInv );
}

template <typename numType> inline
math::mat3_t<numType> math::transpose( const mat3_t<numType>& m ) {
	return mat3_t<numType>(
		m.m[0][0], m.m[1][0], m.m[2][0],
		m.m[0][1], m.m[1][1], m.m[2][1],
		m.m[0][2], m.m[1][2], m.m[2][2]
	);
}

template <typename numType> inline
math::mat3_t<numType> math::rotate( const mat3_t<numType>& m, const vec3_t<numType>& axis, numType angle ) {
	numType			c( std::cos(angle) );
	numType			s( std::sin(angle) );
	vec3_t<numType>	a( normalize<numType>(axis) );
	numType			omc( numType(1) - c );
	numType			xy( (a.v[0]*a.v[1])*omc );
	numType			yz( (a.v[1]*a.v[2])*omc );
	numType			zx( (a.v[2]*a.v[0])*omc );
	numType			sx( s*a.v[0] ), sy( s*a.v[1] ), sz( s*a.v[2] );
	
	return m * mat3_t<numType>(
		c+((a.v[0]*a.v[0]) * omc),	xy + sz,				zx - sy,
		xy - sz,				c+((a.v[1]*a.v[1]) * omc),	yz + sx,
		zx + sy,				yz - sx,				c+((a.v[2]*a.v[2]) * omc)
	);
}

template <typename numType> inline
math::mat3_t<numType> math::scale( const mat3_t<numType>& m, const vec3_t<numType>& scale ) {
	return mat3_t<numType>(
		m.m[0][0]*scale.v[0], m.m[0][1], m.m[0][2],
		m.m[1][0], m.m[1][1]*scale.v[1], m.m[1][2],
		m.m[2][0], m.m[2][1], m.m[2][2]*scale.v[2]
	);
}
	
//-----------------------------------------------------------------------------
// 4x4 Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline
numType math::determinant( const mat4_t<numType>& m ) {
	return numType(
		( m.m[0][3]*m.m[1][2]*m.m[2][1]*m.m[3][0] ) - ( m.m[0][2]*m.m[1][3]*m.m[2][1]*m.m[3][0] ) - ( m.m[0][3]*m.m[1][1]*m.m[2][2]*m.m[3][0] ) + ( m.m[0][1]*m.m[1][3]*m.m[2][2]*m.m[3][0] ) +
		( m.m[0][2]*m.m[1][1]*m.m[2][3]*m.m[3][0] ) - ( m.m[0][1]*m.m[1][2]*m.m[2][3]*m.m[3][0] ) - ( m.m[0][3]*m.m[1][2]*m.m[2][0]*m.m[3][1] ) + ( m.m[0][2]*m.m[1][3]*m.m[2][0]*m.m[3][1] ) +
		( m.m[0][3]*m.m[1][0]*m.m[2][2]*m.m[3][1] ) - ( m.m[0][0]*m.m[1][3]*m.m[2][2]*m.m[3][1] ) - ( m.m[0][2]*m.m[1][0]*m.m[2][3]*m.m[3][1] ) + ( m.m[0][0]*m.m[1][2]*m.m[2][3]*m.m[3][1] ) +
		( m.m[0][3]*m.m[1][1]*m.m[2][0]*m.m[3][2] ) - ( m.m[0][1]*m.m[1][3]*m.m[2][0]*m.m[3][2] ) - ( m.m[0][3]*m.m[1][0]*m.m[2][1]*m.m[3][2] ) + ( m.m[0][0]*m.m[1][3]*m.m[2][1]*m.m[3][2] ) +
		( m.m[0][1]*m.m[1][0]*m.m[2][3]*m.m[3][2] ) - ( m.m[0][0]*m.m[1][1]*m.m[2][3]*m.m[3][2] ) - ( m.m[0][2]*m.m[1][1]*m.m[2][0]*m.m[3][3] ) + ( m.m[0][1]*m.m[1][2]*m.m[2][0]*m.m[3][3] ) +
		( m.m[0][2]*m.m[1][0]*m.m[2][1]*m.m[3][3] ) - ( m.m[0][0]*m.m[1][2]*m.m[2][1]*m.m[3][3] ) - ( m.m[0][1]*m.m[1][0]*m.m[2][2]*m.m[3][3] ) + ( m.m[0][0]*m.m[1][1]*m.m[2][2]*m.m[3][3] )
	);
	//umm... wow
}

template <typename numType> inline
math::mat4_t<numType> math::inverse( const mat4_t<numType>& m ) {
	
	//these operations appear only 2 times in the return value
	numType zwwx( m.m[2][3] * m.m[3][0] );
	
	//these operations appear 3 times in the return value
	numType xxyy( m.m[0][0] * m.m[1][1] );
	numType xxyz( m.m[0][0] * m.m[1][2] );
	numType xzyy( m.m[0][2] * m.m[1][1] );
	numType xyyz( m.m[0][1] * m.m[1][2] );
	numType zwwy( m.m[2][3] * m.m[3][1] );
	numType zwwz( m.m[2][3] * m.m[3][2] );
	
	//these operations appear 4 times in the return value
	numType xxyw( m.m[0][0] * m.m[1][3] );
	numType xyyx( m.m[0][1] * m.m[1][0] );
	numType xyyw( m.m[0][1] * m.m[1][3] );
	numType xzyx( m.m[0][2] * m.m[1][0] );
	numType xzyw( m.m[0][2] * m.m[1][3] );
	numType xwyx( m.m[0][3] * m.m[1][0] );
	numType xwyy( m.m[0][3] * m.m[1][1] );
	numType xwyz( m.m[0][3] * m.m[1][2] );
	numType yyzw( m.m[1][1] * m.m[2][3] );
	numType yzzw( m.m[1][2] * m.m[2][3] );
	numType zxwy( m.m[2][0] * m.m[3][1] );
	numType zxwz( m.m[2][0] * m.m[3][2] );
	numType zxww( m.m[2][0] * m.m[3][3] );
	numType zywx( m.m[2][1] * m.m[3][0] );
	numType zywz( m.m[2][1] * m.m[3][2] );
	numType zyww( m.m[2][1] * m.m[3][3] );
	numType zzwx( m.m[2][2] * m.m[3][0] );
	numType zzwy( m.m[2][2] * m.m[3][1] );
	numType zzww( m.m[2][2] * m.m[3][3] );
	
	return mat4_t<numType>(
		( yzzw*m.m[3][1] ) - ( m.m[1][3]*zzwy ) + ( m.m[1][3]*zywz ) - ( yyzw*m.m[3][2] ) - ( m.m[1][2]*zyww ) + ( m.m[1][1]*zzww ),
		( m.m[0][3]*zzwy ) - ( m.m[0][2]*zwwy ) - ( m.m[0][3]*zywz ) + ( m.m[0][1]*zwwz ) + ( m.m[0][2]*zyww ) - ( m.m[0][1]*zzww ),
		( xzyw*m.m[3][1] ) - ( xwyz*m.m[3][1] ) + ( xwyy*m.m[3][2] ) - ( xyyw*m.m[3][2] ) - ( xzyy*m.m[3][3] ) + ( xyyz*m.m[3][3] ),
		( xwyz*m.m[2][1] ) - ( xzyw*m.m[2][1] ) - ( xwyy*m.m[2][2] ) + ( xyyw*m.m[2][2] ) + ( m.m[0][2]*yyzw ) - ( m.m[0][1]*yzzw ),
		( m.m[1][3]*zzwx ) - ( yzzw*m.m[3][0] ) - ( m.m[1][3]*zxwz ) + ( m.m[1][0]*zwwz ) + ( m.m[1][2]*zxww ) - ( m.m[1][0]*zzww ),
		( m.m[0][2]*zwwx ) - ( m.m[0][3]*zzwx ) + ( m.m[0][3]*zxwz ) - ( m.m[0][0]*zwwz ) - ( m.m[0][2]*zxww ) + ( m.m[0][0]*zzww ),
		( xwyz*m.m[3][0] ) - ( xzyw*m.m[3][0] ) - ( xwyx*m.m[3][2] ) + ( xxyw*m.m[3][2] ) + ( xzyx*m.m[3][3] ) - ( xxyz*m.m[3][3] ),
		( xzyw*m.m[2][0] ) - ( xwyz*m.m[2][0] ) + ( xwyx*m.m[2][2] ) - ( xxyw*m.m[2][2] ) - ( xzyx*m.m[2][3] ) + ( m.m[0][0]*yzzw ),
		( yyzw*m.m[3][0] ) - ( m.m[1][3]*zywx ) + ( m.m[1][3]*zxwy ) - ( m.m[1][0]*zwwy ) - ( m.m[1][1]*zxww ) + ( m.m[1][0]*zyww ),
		( m.m[0][3]*zywx ) - ( m.m[0][1]*zwwx ) - ( m.m[0][3]*zxwy ) + ( m.m[0][0]*zwwy ) + ( m.m[0][1]*zxww ) - ( m.m[0][0]*zyww ),
		( xyyw*m.m[3][0] ) - ( xwyy*m.m[3][0] ) + ( xwyx*m.m[3][1] ) - ( xxyw*m.m[3][1] ) - ( xyyx*m.m[3][3] ) + ( xxyy*m.m[3][3] ),
		( xwyy*m.m[2][0] ) - ( xyyw*m.m[2][0] ) - ( xwyx*m.m[2][1] ) + ( xxyw*m.m[2][1] ) + ( xyyx*m.m[2][3] ) - ( m.m[0][0]*yyzw ),
		( m.m[1][2]*zywx ) - ( m.m[1][1]*zzwx ) - ( m.m[1][2]*zxwy ) + ( m.m[1][0]*zzwy ) + ( m.m[1][1]*zxwz ) - ( m.m[1][0]*zywz ),
		( m.m[0][1]*zzwx ) - ( m.m[0][2]*zywx ) + ( m.m[0][2]*zxwy ) - ( m.m[0][0]*zzwy ) - ( m.m[0][1]*zxwz ) + ( m.m[0][0]*zywz ),
		( xzyy*m.m[3][0] ) - ( xyyz*m.m[3][0] ) - ( xzyx*m.m[3][1] ) + ( xxyz*m.m[3][1] ) + ( xyyx*m.m[3][2] ) - ( xxyy*m.m[3][2] ),
		( xyyz*m.m[2][0] ) - ( xzyy*m.m[2][0] ) + ( xzyx*m.m[2][1] ) - ( xxyz*m.m[2][1] ) - ( xyyx*m.m[2][2] ) + ( xxyy*m.m[2][2] )
	) * numType(1) / determinant(m);
	// FML
}

template <typename numType> inline
math::mat4_t<numType> math::transpose( const mat4_t<numType>& m ) {
	return mat4_t<numType>(
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3] );
}

template <typename numType> inline
math::mat4_t<numType> math::rotate( const mat4_t<numType>& m, const vec3_t<numType>& axis, numType angle ) {
	numType			c( std::cos(angle) );
	numType			s( std::sin(angle) );
	vec3_t<numType>	a( normalize<numType>(axis) );
	numType			omc( numType(1) - c );
	numType			xy( (a.v[0]*a.v[1])*omc );
	numType			yz( (a.v[1]*a.v[2])*omc );
	numType			zx( (a.v[2]*a.v[0])*omc );
	numType			sx( s*a.v[0] ), sy( s*a.v[1] ), sz( s*a.v[2] );
	
	return m * mat4_t<numType>(
		c+((a.v[0]*a.v[0]) * omc),	xy + sz,				zx - sy,				numType( 0 ),
		xy - sz,				c+((a.v[1]*a.v[1]) * omc),	yz + sx,				numType( 0 ),
		zx + sy,				yz - sx,				c+((a.v[2]*a.v[2]) * omc),	numType( 0 ),
		numType( 0 ),			numType( 0 ),			numType( 0 ),			numType( 1 )
	);
}

template <typename numType> inline
math::mat4_t<numType> math::scale( const mat4_t<numType>& m, const vec3_t<numType>& scale ) {
	return mat4_t<numType>(
		m.m[0][0]*scale.v[0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1]*scale.v[1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2]*scale.v[2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]
	);
}

template <typename numType> inline
math::mat4_t<numType> math::translate( const mat4_t<numType>& m, const vec3_t<numType>& t ) {
	return mat4_t<numType>(
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0]+t.v[0], m.m[3][1]+t.v[1], m.m[3][2]+t.v[2], m.m[3][3]
	);
}

template <typename numType> inline
math::mat4_t<numType> math::perspective( numType fov, numType aspect, numType zNear, numType zFar ) {
	numType top( tan(HL_DEG2RAD(fov) / numType(2)) * zNear );
	numType bottom( -top );
	numType xMin( bottom * aspect );
	numType xMax( top * aspect );
	numType zDelta( zFar - zNear );

	return mat4_t<numType>(
		(numType(2) * zNear) / (xMax - xMin), numType(0), numType(0), numType(0),
		numType(0), (numType(2) * zNear) / (top - bottom), numType(0), numType(0),
		numType(0), numType(0), -(zFar + zNear) / zDelta, numType(-1),
		numType(0), numType(0), (numType(-2) * zFar * zNear) / zDelta, numType(0)
	);
}

template <typename numType> inline
math::mat4_t<numType> math::infinitePerspective( numType fov, numType aspect, numType zNear ) {
	numType top( tan(HL_DEG2RAD(fov) / numType(2)) * zNear );
	numType bottom( -top );
	numType xMin( bottom * aspect );
	numType xMax( top * aspect );

	return mat4_t<numType>(
		(numType(2) * zNear) / (xMax - xMin), numType(0), numType(0), numType(0),
		numType(0), (numType(2) * zNear) / (top - bottom), numType(0), numType(0),
		numType(0), numType(0), numType(-1), numType(-1),
		numType(0), numType(0), numType(-2) * zNear, numType(0)
	);
}

template <typename numType> inline
math::mat4_t<numType> math::ortho( numType left, numType right, numType top, numType bottom ) {
	numType w( right - left );
	numType h( bottom - top );
	
	return mat4_t<numType>(
		numType(2) / w, numType(0), numType(0), numType(0),
		numType(0), numType(2) / h, numType(0), numType(0),
		numType(0), numType(0), numType(-1), numType(0),
		-(right + left) / w, -(top + bottom) / h, numType(0), numType(1)
	);
}

template <typename numType> inline
math::mat4_t<numType> math::ortho( numType left, numType right, numType top, numType bottom, numType near, numType far ) {
	numType w( right - left );
	numType h( bottom - top );
	numType z( far - near );
	
	return mat4_t<numType>(
		numType(2) / w, numType(0), numType(0), numType(0),
		numType(0), numType(2) / h, numType(0), numType(0),
		numType(0), numType(0), numType(-2) / z, numType(0),
		-(right + left) / w, -(top + bottom) / h, -(far+near) / z, numType(1)
	);
}

template <typename numType> inline
math::mat4_t<numType> math::frustum( numType left, numType right, numType top, numType bottom, numType near, numType far ) {
	numType w( right - left );
	numType h( bottom - top );
	numType z( far - near );
	
	return mat4_t<numType>(
		(numType(2) * near) / w, numType(0), numType(0), numType(0),
		numType(0), (numType(2) * near) / h, numType(0), numType(0),
		(right + left) / w, (top + bottom) / h, -(far + near) / z, numType(-1),
		numType(0), numType(0), (numType(-2) * far * near) / z, numType(0)
	);
}

template <typename numType> inline
math::mat4_t<numType> math::lookAt( const vec3_t<numType>& pos, const vec3_t<numType>& target, const vec3_t<numType>& up ) {
	vec3_t<numType> zAxis( normalize( pos - target ) );
	vec3_t<numType> xAxis( normalize( cross( up, zAxis ) ) );
	vec3_t<numType> yAxis( normalize( cross( zAxis, xAxis ) ) );
	
	return mat4_t<numType>(
		xAxis.v[0], yAxis.v[0], zAxis.v[0], numType(0),
		xAxis.v[1], yAxis.v[1], zAxis.v[1], numType(0),
		xAxis.v[2], yAxis.v[2], zAxis.v[2], numType(0),
		-dot( xAxis, pos ), -dot( yAxis, pos ), -dot( zAxis, pos ), numType(1)
	);
}

} // hamLibs namespace

#endif	/* __HL_MAT_UTILS_H__ */
