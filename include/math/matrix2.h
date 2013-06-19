/*
 * 3d matrix class
 * recommended for use with non-integral types

 * NOTES:
 * Matrix is Row-Major
 * orientation is as follows:
 * ----------------
 *		XX	XY
 *		YX	YY
 * ----------------
*/

#ifndef __HL_MATH_MATRIX2_H__
#define __HL_MATH_MATRIX2_H__

#include "../utils/assert.h"
#include "vec2.h"

namespace hamLibs {
namespace math {

//---------------------------------------------------------------------
//				Begin mat2 class
//---------------------------------------------------------------------
template <typename numType>
struct mat2_t {
	union {
		numType	m[2][2];
		struct {
			numType	xx, xy,
					yx, yy; // these references are attached to the array above during construction
		} index;
        
        struct {
            vec2_t<numType> x;
            vec2_t<numType> y;
        } row;
	};

	//hardhat construction
	mat2_t();
	mat2_t(numType n);
	mat2_t(const mat2_t<numType>&);
	mat2_t(mat2_t<numType>&&);
    mat2_t( const vec2_t<numType>&,
            const vec2_t<numType>& );
	//delegated constructor
	mat2_t(	numType inXX, numType inXY,
			numType inYX, numType inYY
	);
	~mat2_t() {};

	//array operators
	numType*		operator	[]		(int);
	const numType*	operator	[]		(int) const;

	//mat-mat operators
	mat2_t&			operator	++		(); //prefix operators
	mat2_t&			operator	--		();
	mat2_t			operator	++		(int); //postfix operators
	mat2_t			operator	--		(int);
	mat2_t			operator	+		(const mat2_t<numType>& input) const;
	mat2_t			operator	-		(const mat2_t<numType>& input) const;
	mat2_t			operator	-		() const;
	mat2_t			operator	*		(const mat2_t<numType>& input) const;
	mat2_t&			operator	=		(const mat2_t<numType>& input);
	mat2_t&			operator	=		(mat2_t<numType>&& input);
	mat2_t&			operator	+=		(const mat2_t<numType>& input);
	mat2_t&			operator	-=		(const mat2_t<numType>& input);
	mat2_t&			operator	*=		(const mat2_t<numType>& input);
	bool			operator	==		(const mat2_t<numType>& input) const;
	bool			operator	!=		(const mat2_t<numType>& input) const;

	//mat-vector operators
	mat2_t			operator	+		(const vec2_t<numType>&) const;
	mat2_t			operator	-		(const vec2_t<numType>&) const;
	vec2_t<numType>	operator	*		(const vec2_t<numType>&) const;
	mat2_t&			operator	=		(const vec2_t<numType>&);
	mat2_t&			operator	+=		(const vec2_t<numType>&);
	mat2_t&			operator	-=		(const vec2_t<numType>&);

	//mat-scalar operators
	mat2_t			operator	+		(numType) const;
	mat2_t			operator	-		(numType) const;
	mat2_t			operator	*		(numType) const;
	mat2_t			operator	/		(numType) const;
	mat2_t&			operator	=		(numType);
	mat2_t&			operator	+=		(numType);
	mat2_t&			operator	-=		(numType);
	mat2_t&			operator	*=		(numType);
	mat2_t&			operator	/=		(numType);
};

//---------------------------------------------------------------------
//	2x2 Matrix Constructors (mostly delegated)
//---------------------------------------------------------------------
//all constructions use list-initializations and Delegations
template <typename numType> inline
mat2_t<numType>::mat2_t() :
	mat2_t(
		numType(0), numType(0),
		numType(0), numType(0)
	)
{}
template <typename numType> inline
mat2_t<numType>::mat2_t(numType n) :
	mat2_t(
		n, numType(0),
		numType(0), n
	)
{}

template <typename numType> inline
mat2_t<numType>::mat2_t(const mat2_t<numType>& input) :
	mat2_t(
		input.m[0][0], input.m[0][1],
		input.m[1][0], input.m[1][1]
	)
{}

template <typename numType> inline
mat2_t<numType>::mat2_t(mat2_t<numType>&& input) :
	mat2_t(
		input.m[0][0], input.m[0][1],
		input.m[1][0], input.m[1][1]
	)
{}

template <typename numType> inline
mat2_t<numType>::mat2_t(
    const vec2_t<numType>& x,
    const vec2_t<numType>& y
) : mat2_t(
        x.v[0]. x.v[1],
        y.v[0]. y.v[1]
    )
{}

template <typename numType> inline
mat2_t<numType>::mat2_t(	numType inXX, numType inXY,
						numType inYX, numType inYY) :
	m{
		{ inXX, inXY },
		{ inYX, inYY }
	}
{}

//---------------------------------------------------------------------
//	Array Operators
//---------------------------------------------------------------------
template <typename numType> inline
numType* mat2_t<numType>::operator[](const int index) {
	HL_DEBUG_ASSERT((index >= 0) && (index < 2));
	return m[ index ];
}

template <typename numType> inline
const numType* mat2_t<numType>::operator[](const int index) const {
	HL_DEBUG_ASSERT((index >= 0) && (index < 2));
	return m[ index ];
}

//---------------------------------------------------------------------
//	mat-mat Operators
//---------------------------------------------------------------------
// prefix operators
template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator++ () {
	++m[0][0]; ++m[0][1];
	++m[1][0]; ++m[1][1];
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator-- () {
	--m[0][0]; --m[0][1];
	--m[1][0]; --m[1][1];
	return *this;
}

//postfix operators
template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator++ (int) {
	return mat2_t<numType>(
		++m[0][0], ++m[0][1],
		++m[1][0], ++m[1][1]
	);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator-- (int) {
	return mat2_t<numType>(
		--m[0][0], --m[0][1],
		--m[1][0], --m[1][1]
	);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator + (const mat2_t<numType>& input) const {
	return mat2_t<numType>(
		m[0][0] + input.m[0][0], m[0][1] + input.m[0][1],
		m[1][0] + input.m[1][0], m[1][1] + input.m[1][1]
	);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator - (const mat2_t<numType>& input) const {
	return mat2_t<numType>(
		m[0][0] - input.m[0][0], m[0][1] - input.m[0][1],
		m[1][0] - input.m[1][0], m[1][1] - input.m[1][1]
	);
}

//for operations like "mat2a = -mat2b"
template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator - () const {
	return mat2_t<numType>(	-m[0][0], -m[0][1],
							-m[1][0], -m[1][1]);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator * (const mat2_t<numType>& input) const {
	return mat2_t<numType>(
	//X
		(m[0][0]*input.m[0][0]) + (m[0][1]*input.m[1][0]),
		(m[0][0]*input.m[0][1]) + (m[0][1]*input.m[1][1]),
		
	//Y
		(m[1][0]*input.m[0][0]) + (m[1][1]*input.m[1][0]),
		(m[1][0]*input.m[0][1]) + (m[1][1]*input.m[1][1])
	);
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator = (const mat2_t<numType>& input) {
	m[0][0] = input.m[0][0]; m[0][1] = input.m[0][1];
	m[1][0] = input.m[1][0]; m[1][1] = input.m[1][1];
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator = (mat2_t<numType>&& input) {
	m[0][0] = input.m[0][0]; m[0][1] = input.m[0][1];
	m[1][0] = input.m[1][0]; m[1][1] = input.m[1][1];
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator += (const mat2_t<numType>& input) {
	m[0][0] += input.m[0][0]; m[0][1] += input.m[0][1];
	m[1][0] += input.m[1][0]; m[1][1] += input.m[1][1];
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator -= (const mat2_t<numType>& input) {
	m[0][0] -= input.m[0][0]; m[0][1] -= input.m[0][1];
	m[1][0] -= input.m[1][0]; m[1][1] -= input.m[1][1];
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator *= (const mat2_t<numType>& input) {
	return *this = *this * input;
}

template <typename numType> inline
bool mat2_t<numType>::operator == (const mat2_t<numType>& compare) const {
	return	(
				m[0][0] == compare.m[0][0] && m[0][1] == compare.m[0][1] &&
				m[1][0] == compare.m[1][0] && m[1][1] == compare.m[1][1]
			);
}

template <typename numType> inline
bool mat2_t<numType>::operator != (const mat2_t<numType>& compare) const {
	return	(
				m[0][0] != compare.m[0][0] && m[0][1] != compare.m[0][1] &&
				m[1][0] != compare.m[1][0] && m[1][1] != compare.m[1][1]
			);
}

//---------------------------------------------------------------------
//	mat-vector Operators
//---------------------------------------------------------------------
template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator + (const vec2_t<numType>& input) const {
	return mat2_t(
		m[0][0] + input.v[0], m[0][1] + input.v[1],
		m[1][0] + input.v[0], m[1][1] + input.v[1]
	);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator - (const vec2_t<numType>& input) const {
	return mat2_t(
		m[0][0] - input.v[0], m[0][1] - input.v[1],
		m[1][0] - input.v[0], m[1][1] - input.v[1]
	);
}

template <typename numType> inline
vec2_t<numType> mat2_t<numType>::operator * (const vec2_t<numType>& inVec) const {
	return vec2_t<numType>(
		(m[0][0] * inVec.v[0]) + (m[0][1] * inVec.v[1]),
		(m[1][0] * inVec.v[0]) + (m[1][1] * inVec.v[1])
	);
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator = (const vec2_t<numType>& input) {
	m[0][0] = input.v[0]; m[0][1] = input.v[0];
	m[1][0] = input.v[1]; m[1][1] = input.v[1];
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator += (const vec2_t<numType>& input) {
	m[0][0] += input.v[0]; m[0][1] += input.v[1];
	m[1][0] += input.v[0]; m[1][1] += input.v[1];
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator -= (const vec2_t<numType>& input) {
	m[0][0] -= input.v[0]; m[0][1] -= input.v[1];
	m[1][0] -= input.v[0]; m[1][1] -= input.v[1];
	return *this;
}

//---------------------------------------------------------------------
//	mat-Scalar Operators
//---------------------------------------------------------------------
template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator + (numType input) const {
	return mat2_t<numType>(
		m[0][0] + input, m[0][1] + input,
		m[1][0] + input, m[1][1] + input
	);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator - (numType input) const {
	return mat2_t<numType>(
		m[0][0] - input, m[0][1] - input,
		m[1][0] - input, m[1][1] - input
	);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator * (numType input) const {
	return mat2_t<numType>(
		m[0][0] * input, m[0][1] * input,
		m[1][0] * input, m[1][1] * input
	);
}

template <typename numType> inline
mat2_t<numType> mat2_t<numType>::operator / (numType input) const {
	return mat2_t<numType>(
		m[0][0] / input, m[0][1] / input,
		m[1][0] / input, m[1][1] / input
	);
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator = (numType input) {
	m[0][0] = input; m[0][1] = input;
	m[1][0] = input; m[1][1] = input;
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator += (numType input) {
	m[0][0] += input; m[0][1] += input;
	m[1][0] += input; m[1][1] += input;
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator -= (numType input) {
	m[0][0] -= input; m[0][1] -= input;
	m[1][0] -= input; m[1][1] -= input;
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator *= (numType input) {
	m[0][0] *= input; m[0][1] *= input;
	m[1][0] *= input; m[1][1] *= input;
	return *this;
}

template <typename numType> inline
mat2_t<numType>& mat2_t<numType>::operator /= (numType input) {
	m[0][0] /= input; m[0][1] /= input;
	m[1][0] /= input; m[1][1] /= input;
	return *this;
}

} //end math namespace
} //end hamlibs namespace
#endif /*__mat2_H__*/
