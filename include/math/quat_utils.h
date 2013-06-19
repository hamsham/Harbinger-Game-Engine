/* 
 * File:   quat_utils.h
 * Author: hammy
 *
 * Created on January 8, 2013, 8:55 AM
 */

#ifndef __HL_QUAT_UTILS_H__
#define	__HL_QUAT_UTILS_H__

#include "math.h"


namespace hamLibs {

///////////////////////////////////////////////////////////////////////////////
//		Prototypes
///////////////////////////////////////////////////////////////////////////////
	namespace math {
//-----------------------------------------------------------------------------
//		Quaternion Functions
//-----------------------------------------------------------------------------
template <typename numType> inline numType			dot( const quat_t<numType>&, const quat_t<numType>& );

template <typename numType> inline numType			magnitude( const quat_t<numType>& );

template <typename numType> inline quat_t<numType>	inverse( const quat_t<numType>& );

template <typename numType> inline quat_t<numType>	conjugate( const quat_t<numType>& );

template <typename numType> inline quat_t<numType>	normalize( const quat_t<numType>& );

//-----------------------------------------------------------------------------
//		Quaternions & Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline mat3_t<numType>	quatToMat3( const quat_t<numType>& );
template <typename numType> inline mat4_t<numType>	quatToMat4( const quat_t<numType>& );

template <typename numType> inline quat_t<numType>	matToQuat( const mat3_t<numType>& );
template <typename numType> inline quat_t<numType>	matToQuat( const mat4_t<numType>& );

//-----------------------------------------------------------------------------
//		Quaternions & Euler Angles
//-----------------------------------------------------------------------------
template <typename numType> inline vec3_t<numType>   getAxisX( const quat_t<numType>& ); // Right-Vector
template <typename numType> inline vec3_t<numType>   getAxisY( const quat_t<numType>& ); // Up-Vector
template <typename numType> inline vec3_t<numType>   getAxisZ( const quat_t<numType>& ); // Forwards Vector

template <typename numType> inline numType			getAngle( const quat_t<numType>& );

template <typename numType> inline vec3_t<numType>	toEuler( const quat_t<numType>& );
template <typename numType> inline quat_t<numType>	fromEuler( const vec3_t<numType>& );
template <typename numType> inline quat_t<numType>	fromEuler( numType, numType, numType );

template <typename numType> inline void				toAxisAngle( const quat_t<numType>&, vec3_t<numType>&, numType& );
template <typename numType> inline vec4_t<numType>	toAxisAngle( const quat_t<numType>& );

template <typename numType> inline quat_t<numType>	fromAxisAngle( const vec3_t<numType>&, numType );
template <typename numType> inline quat_t<numType>	fromAxisAngle( const vec4_t<numType>& );

///////////////////////////////////////////////////////////////////////////////
//		Definitions
///////////////////////////////////////////////////////////////////////////////
	} // end math namespace
//-----------------------------------------------------------------------------
//		Quaternion Functions
//-----------------------------------------------------------------------------
template <typename numType> inline
numType math::dot( const quat_t<numType>& q1, const quat_t<numType>& q2 ) {
	return (q1.q[0]*q2.q[0]) + (q1.q[1]*q2.q[1]) + (q1.q[2]*q2.q[2]) + (q1.q[3]*q2.q[3]);
}

template <typename numType> inline
numType math::magnitude( const quat_t<numType>& q ) {
	return HL_SQRT( (q.q[0]*q.q[0]) + (q.q[1]*q.q[1]) + (q.q[2]*q.q[2]) + (q.q[3]*q.q[3]) );
}

template <typename numType> inline
math::quat_t<numType> math::inverse( const quat_t<numType>& q ) {
	numType dotInv(
		numType(1) / ( (q.q[0]*q.q[0]) + (q.q[1]*q.q[1]) + (q.q[2]*q.q[2]) + (q.q[3]*q.q[3]) )
	);
	return quat_t<numType>(
		-q.q[0] * dotInv,
		-q.q[1] * dotInv,
		-q.q[2] * dotInv,
		q.q[3] * dotInv
	);
}

template <typename numType> inline
math::quat_t<numType> math::conjugate( const quat_t<numType>& q ) {
	return quat_t<numType>( -q.q[0], -q.q[1], -q.q[2], q.q[3] );
}

template <typename numType> inline
math::quat_t<numType> math::normalize( const quat_t<numType>& q ) {
	numType magInv(
		numType(1) / HL_SQRT( (q.q[0]*q.q[0]) + (q.q[1]*q.q[1]) + (q.q[2]*q.q[2]) + (q.q[3]*q.q[3]) )
	);
	return quat_t<numType>(
		q.q[0] * magInv,
		q.q[1] * magInv,
		q.q[2] * magInv,
		q.q[3] * magInv
	);
}

//-----------------------------------------------------------------------------
//		Quaternions & Matrices
//-----------------------------------------------------------------------------
template <typename numType> inline
math::mat3_t<numType> math::quatToMat3( const quat_t<numType>& q ) {
	numType xx = q.q[0] * q.q[0] * numType(numType(2));
	numType yy = q.q[1] * q.q[1] * numType(numType(2));
	numType zz = q.q[2] * q.q[2] * numType(numType(2));
	numType xy = q.q[0] * q.q[1];
	numType xz = q.q[0] * q.q[2];
	numType xw = q.q[0] * q.q[3];
	numType yz = q.q[1] * q.q[2];
	numType yw = q.q[1] * q.q[3];
	numType zw = q.q[2] * q.q[3];
	
	return mat3_t<numType>(
		numType(1)-(yy+zz),	numType(numType(2))*(xy+zw),	numType(numType(2))*(xz-yw),
		numType(numType(2))*(xy-zw),	numType(1)-(xx+zz),	numType(numType(2))*(yz+xw),
		numType(numType(2))*(xz+yw),	numType(numType(2))*(yz-xw),	numType(1)-(xx+yy)
	);
}

template <typename numType> inline
math::mat4_t<numType> math::quatToMat4( const quat_t<numType>& q ) {
	numType xx = q.q[0] * q.q[0] * numType(numType(2));
	numType yy = q.q[1] * q.q[1] * numType(numType(2));
	numType zz = q.q[2] * q.q[2] * numType(numType(2));
	numType xy = q.q[0] * q.q[1];
	numType xz = q.q[0] * q.q[2];
	numType xw = q.q[0] * q.q[3];
	numType yz = q.q[1] * q.q[2];
	numType yw = q.q[1] * q.q[3];
	numType zw = q.q[2] * q.q[3];
	
	return mat4_t<numType>(
		numType(1)-(yy+zz),	numType(numType(2))*(xy+zw),	numType(numType(2))*(xz-yw), numType(0),
		numType(numType(2))*(xy-zw),	numType(1)-(xx+zz),	numType(numType(2))*(yz+xw), numType(0),
		numType(numType(2))*(xz+yw),	numType(numType(2))*(yz-xw),	numType(1)-(xx+yy), numType(0),
		numType(0), numType(0), numType(0), numType(1)
	);
}

template <typename numType> inline
math::quat_t<numType> math::matToQuat( const mat3_t<numType>& m ) {
    numType s( 0 );
    numType trace( m.m[0][0] + m.m[1][1] + m.m[2][2] );
	quat_t<numType> q;

	if (trace > numType(0)) {
        s = HL_SQRT(trace + numType(1));
        q.q[3] = s * numType(0.5);
        s = numType(0.5) / s;
        q.q[0] = (m.m[1][2] - m.m[2][1]) * s;
        q.q[1] = (m.m[2][0] - m.m[0][2]) * s;
        q.q[2] = (m.m[0][1] - m.m[1][0]) * s;
    }
    else {
        int nxt[3] = {1, 2, 0};
        int i(0), j(0), k(0);

        if (m.m[1][1] > m.m[0][0])
            i = 1;

        if (m.m[2][2] > m.m[i][i])
            i = 2;

        j = nxt[i];
        k = nxt[j];
        s = HL_SQRT((m.m[i][i] - (m.m[j][j] + m.m[k][k])) + numType(1));

        q[i] = s * numType(0.5);
        s = numType(0.5) / s;
        q[3] = (m.m[j][k] - m.m[k][j]) * s;
        q[j] = (m.m[i][j] + m.m[j][i]) * s;
        q[k] = (m.m[i][k] + m.m[k][i]) * s;
    }
	return q;
}

template <typename numType> inline
math::quat_t<numType> math::matToQuat( const mat4_t<numType>& m ) {
    numType s( 0 );
    numType trace( m.m[0][0] + m.m[1][1] + m.m[2][2] );
	quat_t<numType> q;

	if (trace > numType(0)) {
        s = HL_SQRT(trace + numType(1));
        q.q[3] = s * numType(0.5);
        s = numType(0.5) / s;
        q.q[0] = (m.m[1][2] - m.m[2][1]) * s;
        q.q[1] = (m.m[2][0] - m.m[0][2]) * s;
        q.q[2] = (m.m[0][1] - m.m[1][0]) * s;
    }
    else {
        int nxt[3] = {1, 2, 0};
        int i(0), j(0), k(0);

        if (m.m[1][1] > m.m[0][0])
            i = 1;

        if (m.m[2][2] > m.m[i][i])
            i = 2;

        j = nxt[i];
        k = nxt[j];
        s = HL_SQRT((m.m[i][i] - (m.m[j][j] + m.m[k][k])) + numType(1));

        q[i] = s * numType(0.5);
        s = numType(0.5) / s;
        q[3] = (m.m[j][k] - m.m[k][j]) * s;
        q[j] = (m.m[i][j] + m.m[j][i]) * s;
        q[k] = (m.m[i][k] + m.m[k][i]) * s;
    }
	return q;
}
//-----------------------------------------------------------------------------
//		quat_t<numType>s & Euler Angles
//-----------------------------------------------------------------------------
template <typename numType> inline
math::vec3_t<numType> getAxisX( const math::quat_t<numType>& q ) {
    return math::vec3_t<numType>(
        numType(1) - numType(2) * (q[1] * q[1] + q[2] * q[2]),
        numType(2) * (q[0] * q[1] + q[3] * q[2]),
        numType(2) * (q[0] * q[2] - q[3] * q[1])
    );
}

template <typename numType> inline
math::vec3_t<numType> getAxisY( const math::quat_t<numType>& q ) {
    return math::vec3_t<numType>(
        numType(2) * (q[0] * q[1] - q[3] * q[2]),
        numType(1) - numType(2) * (q[0] * q[0] + q[2] * q[2]),
        numType(2) * (q[1] * q[2] + q[3] * q[0])
    );
}

template <typename numType> inline
math::vec3_t<numType> getAxisZ( const math::quat_t<numType>& q ) {
    return math::vec3_t<numType>(
        numType(2) * (q[0] * q[2] + q[3] * q[1]),
        numType(2) * (q[1] * q[0] - q[3] * q[0]),
        numType(1) - numType(2) * (q[0] * q[0] + q[1] * q[1])
    );
}

template <typename numType> inline
numType math::getAngle( const quat_t<numType>& q ) {
	return numType( std::acos( q.q[3] * numType(2) ) );
}

template <typename numType> inline
math::vec3_t<numType> math::toEuler( const quat_t<numType>& q ) {
	numType xx( q.q[0] * q.q[0] );
	numType yy( q.q[1] * q.q[1] );
	numType zz( q.q[2] * q.q[2] );
	numType pitch(
		std::atan2(
			numType(2) * ((q.q[3]*q.q[0]) * (q.q[1]*q.q[2])),
			numType(1) - (numType(2) * (xx+yy))
		)
	);
	numType yaw(
		std::asin(
			numType(2) * ((q.q[3]*q.q[1]) - (q.q[2]*q.q[0]))
		)
	);
	numType roll(
		std::atan2(
			numType(2) * ((q.q[3]*q.q[2]) + (q.q[0]*q.q[1])),
			numType(1) - (numType(2) * (yy+zz))
		)
	);
	return vec3_t<numType>( pitch, yaw, roll );
}

template <typename numType> inline
math::quat_t<numType> math::fromEuler( const math::vec3_t<numType>& angles ) {
	numType cp ( cos( angles.v[0] * numType(0.5) ) );
	numType sp ( sin( angles.v[0] * numType(0.5) ) );
	
	numType cy ( cos( angles.v[1] * numType(0.5) ) );
	numType sy ( sin( angles.v[1] * numType(0.5) ) );
	
	numType cr ( cos( angles.v[2] * numType(0.5) ) );
	numType sr ( sin( angles.v[2] * numType(0.5) ) );
	
	return math::quat_t<numType>(
		(sy*cp*cr) - (cy*sp*sr),
		(cy*sp*cr) + (sy*cp*sr),
		(cy*cp*sr) - (sy*sp*cr),
		(cy*cp*cr) + (sy*sp*sr)
	);
}

template <typename numType> inline
math::quat_t<numType> math::fromEuler( numType pitch, numType yaw, numType roll ) {
	numType cp ( cos( pitch * numType(0.5) ) );
	numType sp ( sin( pitch * numType(0.5) ) );
	
	numType cy ( cos( yaw * numType(0.5) ) );
	numType sy ( sin( yaw * numType(0.5) ) );
	
	numType cr ( cos( roll * numType(0.5) ) );
	numType sr ( sin( roll * numType(0.5) ) );
	
	return math::quat_t<numType>(
		(sy*cp*cr) - (cy*sp*sr),
		(cy*sp*cr) + (sy*cp*sr),
		(cy*cp*sr) - (sy*sp*cr),
		(cy*cp*cr) + (sy*sp*sr)
	);
}

template <typename numType> inline
void math::toAxisAngle( const quat_t<numType>& q, vec3_t<numType>& v, numType& a ) {
	numType s( HL_SQRT( numType(1) - (q.q[3]*q.q[3])) );
	
	if (std::abs( s ) <= numType(0) )
		s = numType(1);
	s = numType(1) / s;
	
	v.v[0] = q.q[0] * s,
	v.v[1] = q.q[1] * s,
	v.v[2] = q.q[2] * s,
	a = std::acos( q.q[3] ) * numType(2);
}

template <typename numType> inline
math::vec4_t<numType> math::toAxisAngle( const quat_t<numType>& q ) {
	numType c( q.q[3] );
	numType s( HL_SQRT( numType(1) - (q.q[3]*q.q[3])) );
	
	if (std::abs( s ) <= numType(0) )
		s = numType(1);
	s = numType(1) / s;
	
	return vec4_t<numType>(
		q.q[0] * s,
		q.q[1] * s,
		q.q[2] * s,
		std::acos( q.q[3] ) * numType(2)
	);
}

template <typename numType> inline
math::quat_t<numType> math::fromAxisAngle( const vec3_t<numType>& axis, numType angle ) {
	numType a( angle * numType(0.5) );
	numType s( std::sin( a ));
	return quat_t<numType>(
			s * axis.v[0],
			s * axis.v[1],
			s * axis.v[2],
			std::cos( a )
	);
}

template <typename numType> inline
math::quat_t<numType> math::fromAxisAngle( const vec4_t<numType>& v ) {
	numType a( v.v[3] * numType(0.5) );
	numType s( std::sin( a ));
	return quat_t<numType>(
			s * v.v[0],
			s * v.v[1],
			s * v.v[2],
			std::cos( a )
	);
}


} // hamLibs namespace

#endif	/* __HL_QUAT_UTILS_H__ */
