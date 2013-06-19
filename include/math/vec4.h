/*
 * 4d vector structure
 * recommended for use with non-integral types

 * NOTES:
 * Orientation is as follows:
 * ----------
 *		X
 *		Y
 *		Z
 *		W
 * ----------
*/

#ifndef __HL_MATH_VEC4_H__
#define __HL_MATH_VEC4_H__

#include "../utils/assert.h"

namespace hamLibs {
namespace math {

template <typename numType>
struct vec4_t {
	union {
		numType v[4];
		struct {
			numType x, y, z, w;
		} index;
	};

	//constructors
	vec4_t			();
	vec4_t			( numType n );
	vec4_t			(const vec4_t<numType>&);
	vec4_t			(vec4_t<numType>&&);
	vec4_t			(numType inX, numType inY, numType inZ, numType inW);
	~vec4_t			(){}

	//array operators
	numType			operator		[]		(int) const;
	numType&		operator		[]		(int);

	//vector-vector operators
	vec4_t&			operator		= 		(const vec4_t<numType>&);
	vec4_t&			operator		= 		(vec4_t<numType>&&);
	vec4_t			operator 		+ 		(const vec4_t<numType>&) const;
	vec4_t			operator 		- 		(const vec4_t<numType>&) const;
	vec4_t			operator 		- 		() const;
	vec4_t			operator 		* 		(const vec4_t<numType>&) const;
	vec4_t			operator 		/ 		(const vec4_t<numType>&) const;
	vec4_t&			operator 		+= 		(const vec4_t<numType>&);
	vec4_t&			operator 		-= 		(const vec4_t<numType>&);
	vec4_t&			operator 		*= 		(const vec4_t<numType>&);
	vec4_t&			operator 		/= 		(const vec4_t<numType>&);
	vec4_t&			operator 		++		(); //prefix operators
	vec4_t&			operator 		--		();
	vec4_t			operator 		++		(int); //postfix operators
	vec4_t			operator 		--		(int);
	bool			operator		==		(const vec4_t<numType>& compare) const; //comparisons
	bool			operator		!=		(const vec4_t<numType>& compare) const;
	bool			operator		<		(const vec4_t<numType>& compare) const;
	bool			operator		>		(const vec4_t<numType>& compare) const;
	bool			operator		<=		(const vec4_t<numType>& compare) const;
	bool			operator		>=		(const vec4_t<numType>& compare) const;

	//vector-matrix operators
	mat4_t<numType>	operator		+		(const mat4_t<numType>&) const;
	mat4_t<numType>	operator		-		(const mat4_t<numType>&) const;
	vec4_t<numType>	operator		*		(const mat4_t<numType>&) const;
	vec4_t&			operator		*=		(const mat4_t<numType>&);

	//vector-scalar operators
	vec4_t			operator 		= 		(numType);
	vec4_t			operator 		+ 		(numType) const;
	vec4_t			operator 		- 		(numType) const;
	vec4_t			operator 		* 		(numType) const;
	vec4_t			operator 		/ 		(numType) const;
	vec4_t&			operator 		+= 		(numType);
	vec4_t&			operator 		-= 		(numType);
	vec4_t&			operator 		*= 		(numType);
	vec4_t&			operator 		/= 		(numType);
};

//---------------------------------------------------------------------
//	Vector Constructors
//---------------------------------------------------------------------
template <typename numType> inline
vec4_t<numType>::vec4_t() :
	vec4_t(
		numType(0), numType(0), numType(0), numType(0)
	)
{}
template <typename numType> inline
vec4_t<numType>::vec4_t( numType n ) :
	vec4_t( n, n, n, n )
{}

template <typename numType> inline
vec4_t<numType>::vec4_t(const vec4_t<numType>& input) :
	vec4_t(
		input.v[0], input.v[1], input.v[2], input.v[3]
	)
{}

template <typename numType> inline
vec4_t<numType>::vec4_t(vec4_t<numType>&& input) :
	vec4_t(
		input.v[0], input.v[1], input.v[2], input.v[3]
	)
{}

template <typename numType> inline
vec4_t<numType>::vec4_t(numType inX, numType inY, numType inZ, numType inW) :
	v{ inX, inY, inZ, inW }
{}

//---------------------------------------------------------------------
//	Array Operators
//---------------------------------------------------------------------
template <typename numType> inline
numType vec4_t<numType>::operator[](const int index) const {
	HL_DEBUG_ASSERT((index >= 0) && (index < 4));
	return v[ index ];
}

template <typename numType> inline
numType& vec4_t<numType>::operator[](const int index) {
	HL_DEBUG_ASSERT((index >= 0) && (index < 4));
	return v[ index ];
}

//---------------------------------------------------------------------
//	Vector-Vector Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator = (const vec4_t<numType>& input) {
	v[0] = input.v[0];
	v[1] = input.v[1];
	v[2] = input.v[2];
	v[3] = input.v[3];
	return *this;
}

template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator = (vec4_t<numType>&& input) {
	v[0] = input.v[0];
	v[1] = input.v[1];
	v[2] = input.v[2];
	v[3] = input.v[3];
	return *this;
}

template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator + (const vec4_t<numType>& input) const {
	return vec4_t(
		v[0] + input.v[0],
		v[1] + input.v[1],
		v[2] + input.v[2],
		v[3] + input.v[3]
	);
}

template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator - (const vec4_t<numType>& input) const {
	return vec4_t(
		v[0] - input.v[0],
		v[1] - input.v[1],
		v[2] - input.v[2],
		v[3] - input.v[3]
	);
}

//for operations like "vectA = -vectB"
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator - () const {
	return vec4_t<numType>(-v[0], -v[1], -v[2], -v[3]);
}

template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator * (const vec4_t<numType>& input) const {
	return vec4_t<numType>(
		v[0] * input.v[0],
		v[1] * input.v[1],
		v[2] * input.v[2],
		v[3] * input.v[3]
	);
}

template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator / (const vec4_t<numType>& input) const {
	return vec4_t<numType>(
		v[0] / input.v[0],
		v[1] / input.v[1],
		v[2] / input.v[2],
		v[3] / input.v[3]
	);
}

template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator += (const vec4_t<numType>& input) {
    v[0] += input.v[0];
    v[1] += input.v[1];
    v[2] += input.v[2];
    v[3] += input.v[3];
	return *this;
}

template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator -= (const vec4_t<numType>& input) {
    v[0] -= input.v[0];
    v[1] -= input.v[1];
    v[2] -= input.v[2];
    v[3] -= input.v[3];
	return *this;
}

template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator *= (const vec4_t<numType>& input) {
    v[0] *= input.v[0];
    v[1] *= input.v[1];
    v[2] *= input.v[2];
    v[3] *= input.v[3];
	return *this;
}

template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator /= (const vec4_t<numType>& input) {
    v[0] /= input.v[0];
    v[1] /= input.v[1];
    v[2] /= input.v[2];
    v[3] /= input.v[3];
	return *this;
}

// prefix operations
template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator++ () {
	++v[0];
	++v[1];
	++v[2];
	++v[3];
	return *this;
}
template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator-- () {
	--v[0];
	--v[1];
	--v[2];
	--v[3];
	return *this;
}

//postfix operations
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator++ (int) {
	return vec4_t<numType>(
		++v[0],
		++v[1],
		++v[2],
		++v[3]
	);
}
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator-- (int) {
	return vec4_t<numType>(
		--v[0],
		--v[1],
		--v[2],
		--v[3]
	);
}

//comparisons
template <typename numType> inline
bool vec4_t<numType>::operator== (const vec4_t<numType>& compare) const {
	return (	v[0] == compare.v[0] &&
				v[1] == compare.v[1] &&
				v[2] == compare.v[2] &&
				v[3] == compare.v[3]
			);
}

template <typename numType> inline
bool vec4_t<numType>::operator!= (const vec4_t<numType>& compare) const {
	return (	v[0] != compare.v[0] &&
				v[1] != compare.v[1] &&
				v[2] != compare.v[2] &&
				v[3] != compare.v[3]
			);
}

template <typename numType> inline
bool vec4_t<numType>::operator< (const vec4_t<numType>& compare) const {
	return (	v[0] < compare.v[0] &&
				v[1] < compare.v[1] &&
				v[2] < compare.v[2] &&
				v[3] < compare.v[3]
			);
}

template <typename numType> inline
bool vec4_t<numType>::operator> (const vec4_t<numType>& compare) const {
	return (	v[0] > compare.v[0] &&
				v[1] > compare.v[1] &&
				v[2] > compare.v[2] &&
				v[3] > compare.v[3]
			);
}

template <typename numType> inline
bool vec4_t<numType>::operator<= (const vec4_t<numType>& compare) const {
	return (	v[0] <= compare.v[0] &&
				v[1] <= compare.v[1] &&
				v[2] <= compare.v[2] &&
				v[3] <= compare.v[3]
			);
}

template <typename numType> inline
bool vec4_t<numType>::operator>= (const vec4_t<numType>& compare) const {
	return (	v[0] >= compare.v[0] &&
				v[1] >= compare.v[1] &&
				v[2] >= compare.v[2] &&
				v[3] >= compare.v[3]
			);
}

//---------------------------------------------------------------------
//	Vector-Matrix Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
mat4_t<numType> vec4_t<numType>::operator + (const mat4_t<numType>& m) const {
	return mat4_t<numType>(
		v[0] + m.m[0][0], v[0] + m.m[0][1], v[0] + m.m[0][2], v[0] + m.m[0][3],
		v[1] + m.m[1][0], v[1] + m.m[1][1], v[1] + m.m[1][2], v[1] + m.m[1][3],
		v[2] + m.m[2][0], v[2] + m.m[2][1], v[2] + m.m[2][2], v[2] + m.m[2][3],
		v[3] + m.m[3][0], v[3] + m.m[3][1], v[3] + m.m[3][2], v[3] + m.m[3][3]
	);
}

template <typename numType> inline
mat4_t<numType> vec4_t<numType>::operator - (const mat4_t<numType>& m) const {
	return mat4_t<numType>(
		v[0] - m.m[0][0], v[0] - m.m[0][1], v[0] - m.m[0][2], v[0] - m.m[0][3],
		v[1] - m.m[1][0], v[1] - m.m[1][1], v[1] - m.m[1][2], v[1] - m.m[1][3],
		v[2] - m.m[2][0], v[2] - m.m[2][1], v[2] - m.m[2][2], v[2] - m.m[2][3],
		v[3] - m.m[3][0], v[3] - m.m[3][1], v[3] - m.m[3][2], v[3] - m.m[3][3]
	);
}

template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator * (const mat4_t<numType>& m) const {
	return vec4_t<numType>(
		(m.m[0][0] * v[0]) + (m.m[0][1] * v[1]) + (m.m[0][2] * v[2]) + (m.m[0][3] * v[3]),
		(m.m[1][0] * v[0]) + (m.m[1][1] * v[1]) + (m.m[1][2] * v[2]) + (m.m[1][3] * v[3]),
		(m.m[2][0] * v[0]) + (m.m[2][1] * v[1]) + (m.m[2][2] * v[2]) + (m.m[2][3] * v[3]),
		(m.m[3][0] * v[0]) + (m.m[3][1] * v[1]) + (m.m[3][2] * v[2]) + (m.m[3][3] * v[3])
	);
}

template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator *= (const mat4_t<numType>& m) {
	*this = *this * m;
	return *this;
}

//---------------------------------------------------------------------
//	Vector-Scalar Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator = (numType input) {
	v[0] = input;
	v[1] = input;
	v[2] = input;
	v[3] = input;
	return *this;
}
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator + (numType input) const {
	return vec4_t<numType>(
		v[0] + input,
		v[1] + input,
		v[2] + input,
		v[3] + input
	);
}
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator - (numType input) const {
	return vec4_t<numType>(
		v[0] - input,
		v[1] - input,
		v[2] - input,
		v[3] - input
	);
}
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator * (numType input) const {
	return vec4_t<numType>(
		v[0] * input,
		v[1] * input,
		v[2] * input,
		v[3] * input
	);
}
template <typename numType> inline
vec4_t<numType> vec4_t<numType>::operator / (numType input) const {
	return vec4_t<numType>(
		v[0] / input,
		v[1] / input,
		v[2] / input,
		v[3] / input
	);
}
template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator += (numType input) {
    v[0] += input.v[0];
    v[1] += input.v[1];
    v[2] += input.v[2];
    v[3] += input.v[3];
	return *this;
}
template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator -= (numType input) {
    v[0] -= input.v[0];
    v[1] -= input.v[1];
    v[2] -= input.v[2];
    v[3] -= input.v[3];
	return *this;
}
template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator *= (numType input) {
    v[0] *= input.v[0];
    v[1] *= input.v[1];
    v[2] *= input.v[2];
    v[3] *= input.v[3];
	return *this;
}
template <typename numType> inline
vec4_t<numType>& vec4_t<numType>::operator /= (numType input) {
    v[0] /= input.v[0];
    v[1] /= input.v[1];
    v[2] /= input.v[2];
    v[3] /= input.v[3];
	return *this;
}

} //end math namespace
} //end hamlibs namespace
#endif /* __HL_MATH_VEC4_H__*/
